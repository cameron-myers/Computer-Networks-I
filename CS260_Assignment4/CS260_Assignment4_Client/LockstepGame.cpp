//---------------------------------------------------------
// file:	LockstepGame.cpp
// author:	Matthew Picioccio
// email:	matthew.picioccio@digipen.edu
//
// brief:	A simple networked game mode with two players, with lock-step turns.
//
// Copyright © 2020 DigiPen, All rights reserved.
//---------------------------------------------------------
#include <cassert>
#include <iostream>
#include "LockstepGame.h"
#include "LocalPlayerController.h"
#include "PlayGameState.h"

constexpr int kNetworkBufferSize = 1500;


/// <summary>
/// Handles errors from socket functions, including transitioning to different game states.
/// </summary>
/// <param name="error_text">Text that describes the error.</param>
/// <remarks>WSAGetLastError will be included in the output by this function.</remarks>
/// <returns>True if the error is "fatal" and the calling function should stop; false if it can be ignored.</returns>
bool LockstepGame_HandleSocketError(const char* error_text)
{
	const auto wsa_error = WSAGetLastError();

	// ignore WSAEWOULDBLOCK
	if (wsa_error == WSAEWOULDBLOCK)
	{
		return false;
	}

	// log unexpected errors and return to the default game mode
	std::cerr << error_text << wsa_error << std::endl;
	PlayDefaultGame();
	return true;
}



/// <summary>
/// Constructs a LockstepGame object
/// </summary>
/// <param name="socket">The socket that the game mode will use</param>
/// <param name="is_host">If true, the local player is the host, so they take precedence on collisions.</param>
LockstepGame::LockstepGame(const SOCKET socket, const bool is_host)
	: socket_(socket)
	, is_host_(is_host)
	, is_local_update_sent_(false)
	, turn_number_(0)
{
	network_buffer_ = new char[kNetworkBufferSize];

	//set the socket as non-blocking
	u_long iMode = 1;
	int result = ioctlsocket(socket, FIONBIO, &iMode);
	if (result == SOCKET_ERROR && LockstepGame_HandleSocketError("Error when setting socket to non-blocking:"))
	{
		return;
	}

	// calculate the center of the board, and move the two players just off of the center in opposite directions
	const auto midpoint_x = board_.GetTileCountX() / 2;
	const auto midpoint_y = board_.GetTileCountY() / 2;
	const BoardPosition host_position{ midpoint_x - 1, midpoint_y - 1 };
	const BoardPosition other_position{ midpoint_x + 1, midpoint_y + 1 };

	// reset player positions
	if (is_host_)
	{
		local_player_.current_position = host_position;
		remote_player_.current_position = other_position;
	}
	else
	{
		remote_player_.current_position = host_position;
		local_player_.current_position = other_position;
	}
}


/// <summary>
/// Handles destruction of a LockstepGame object.
/// </summary>
LockstepGame::~LockstepGame()
{
	// close the socket
	closesocket(socket_);

	delete[] network_buffer_;
}


/// <summary>
/// Per-frame update of the game logic.
/// </summary>
void LockstepGame::Update()
{
	// apply input to the local player
	LocalPlayerController::ApplyInput(local_player_, board_);
	// if the player now has an uncommitted move, AND we haven't sent anything this turn, send an update to the remote machine
	if (!is_local_update_sent_&& local_player_.has_uncommitted_move)
	{
		// serialize the local state
		const auto send_size = SerializeLocalState();
		
		//send the state to the remote player
		int result = send(socket_, network_buffer_, sizeof(network_buffer_), 0);

		if (result == SOCKET_ERROR && LockstepGame_HandleSocketError("Error when sending state data:"))
		{
			return;
		}
		// -- don't forget to handle errors appropriately with the LockstepGame_HandleSocketErrors function!

		// mark that we sent this update, so we do not send it again
		is_local_update_sent_ = true;

		// log that we sent data for this turn
		std::cout << "Sending local update for turn " << turn_number_ << " to remote." << std::endl;
	}

	//attempt to receive data from the remote player
	const int res = recv(socket_, network_buffer_, sizeof(network_buffer_), 0);
	// if we received a packet, deserialize it as remote data
	if (res > 0)
	{
		DeserializeRemoteState(res); // this function will log the receipt of data
	}

	// if both players have moved this turn, then commit both (i.e. keep them in lockstep
	if (local_player_.has_uncommitted_move && remote_player_.has_uncommitted_move)
	{
		// preserve the old locations in case we need to roll this back
		const auto old_local_position = local_player_.current_position;
		const auto old_remote_position = remote_player_.current_position;

		// commit the new moves
		local_player_.CommitMove();
		remote_player_.CommitMove();

		// start a new turn, and mark that we have not yet sent an update for this turn
		++turn_number_;
		is_local_update_sent_ = false;

		// if the players now collide, handle it
		// -- note that this "movement" does not need to be replicated, as the same logic is applied on all machines!
		if (local_player_.current_position == remote_player_.current_position)
		{
			HandleCollision(old_local_position, old_remote_position);
		}
	}
}


/// <summary>
/// Draws the game.
/// </summary>
void LockstepGame::Draw() const
{
	BoardGame::Draw();

	// draw the local player
	auto player_center = board_.GetCenterOfTile(local_player_.current_position);
	local_player_.Draw(player_center);

	// draw the remote player
	player_center = board_.GetCenterOfTile(remote_player_.current_position);
	remote_player_.Draw(player_center);
}


/// <summary>
/// Gets the description for this game mode, which will be drawn on-screen.
/// </summary>
/// <returns>The description for this game mode, which will be drawn on-screen.</returns>
std::string LockstepGame::GetDescription() const
{
	std::string description("Lock-Step Game, Turn ");
	description += is_host_ ? "Host, Turn " : "Non-Host, Turn ";
	description += std::to_string(turn_number_);
	description += local_player_.has_uncommitted_move ? ", Local (Ready)" : ", Local (Waiting)";
	description += remote_player_.has_uncommitted_move ? ", Remote (Ready)" : ", Remote (Waiting)";
	return description;
}


/// <summary>
/// Serializes the local game state into a buffer, for network transmission.
/// </summary>
/// <returns>The number of bytes serialized.</returns>
size_t LockstepGame::SerializeLocalState() const
{
	ZeroMemory(network_buffer_, sizeof(network_buffer_));

	//serialize the turn number, identifying this move
	_itoa_s(turn_number_, network_buffer_, sizeof(int), 10);

	//serialize the uncommitted position of the local player
	_itoa_s(local_player_.current_position.x, network_buffer_ + sizeof(int), sizeof(int), 10);
	_itoa_s(local_player_.current_position.y, network_buffer_ + sizeof(int) * 2, sizeof(int), 10);

	return sizeof(turn_number_) + sizeof(local_player_.current_position);
}


/// <summary>
/// Deserializes the remote state received over the network.
/// </summary>
/// <param name="bytes_received">The number of bytes received over the network.</param>
void LockstepGame::DeserializeRemoteState(const size_t bytes_received)
{
	// validate the parameter - the size of the buffer read
	assert(bytes_received >= sizeof(turn_number_) + sizeof(remote_player_.current_position));

	//deserialize the turn number
	char temp[sizeof(int)];
	ZeroMemory(temp, sizeof(int));
	memcpy(temp, network_buffer_, sizeof(int));

	const auto remote_turn_number = atoi(temp);

	// log the data received
	std::cout << "Received update from remote player's turn " << remote_turn_number << "." << std::endl;

	// if the data received is for the wrong turn, ignore it.
	if (remote_turn_number != turn_number_)
	{
		std::cout << "Ignoring remote update for turn " << remote_turn_number << " because local turn number is " << turn_number_ << std::endl;
		return;
	}

	// if we already have an uncommitted move for the remote player, but it's the right turn, our logic is wrong.  Log it for future investigation.
	if (remote_player_.has_uncommitted_move)
	{
		std::cerr << "Received update for remote player on the correct turn (" << remote_turn_number << "), but the remote player already has an uncommitted move.  How is this possible?" << std::endl;
	}

	//deserialize the uncommitted position of the remote player
	ZeroMemory(temp, sizeof(int));
	memcpy(temp, network_buffer_ + sizeof(int), sizeof(int));
	remote_player_.uncommitted_position.x = atoi(temp);
	ZeroMemory(temp, sizeof(int));
	memcpy(temp, network_buffer_ + (sizeof(int) * 2), sizeof(int));
	remote_player_.uncommitted_position.y = atoi(temp);

	// mark the remote player as having an uncommitted move
	remote_player_.has_uncommitted_move = true;
}


/// <summary>
/// Implements game logic to handle collisions between players.
/// </summary>
/// <param name="old_local_position">The previous position of the local player</param>
/// <param name="old_remote_position">The previous position of the remote player</param>
/// <remarks>The host player will "win" collisions, forcing the remote player to rewind.  If the remote player had not moved, the host is forced to rewind.</remarks>
void LockstepGame::HandleCollision(const BoardPosition old_local_position, const BoardPosition old_remote_position)
{
	if (is_host_)
	{
		std::cout << "Detected collision; local (host) wins, remote (non-host) reverts to previous location" << std::endl;
		remote_player_.current_position = old_remote_position;
		if (local_player_.current_position == remote_player_.current_position)
		{
			std::cout << "Old position of remote (non-host) was also a collision, so reverting local (host) too" << std::endl;
			local_player_.current_position = old_local_position;
		}
	}
	else
	{
		std::cout << "Detected collision; remote (host) wins, local (non-host) reverts to previous location" << std::endl;
		local_player_.current_position = old_local_position;
		if (local_player_.current_position == remote_player_.current_position)
		{
			std::cout << "Old position of local (non-host) was also a collision, so reverting remote (host) too" << std::endl;
			remote_player_.current_position = old_remote_position;
		}
	}
}