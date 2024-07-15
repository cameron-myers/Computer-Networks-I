//---------------------------------------------------------
// file:	LockstepGame.h
// author:	Matthew Picioccio
// email:	matthew.picioccio@digipen.edu
//
// brief:	A simple networked game mode with two players, with lock-step turns.
//
// Copyright © 2020 DigiPen, All rights reserved.
//---------------------------------------------------------
#pragma once

#include <WinSock2.h>
#include "BoardGame.h"
#include "Player.h"


/// <summary>
/// A simple networked game mode with two players, with lock-step turns.
/// </summary>
class LockstepGame final : public BoardGame
{
public:
	LockstepGame(const SOCKET socket, const bool is_host);
	~LockstepGame() override;

	LockstepGame(const LockstepGame&) = delete;
	LockstepGame(LockstepGame&&) = delete;
	LockstepGame& operator=(const LockstepGame&) = delete;
	LockstepGame& operator=(LockstepGame&&) = delete;

	/// <summary>
	/// Per-frame update of the game logic.
	/// </summary>
	void Update() override;

	/// <summary>
	/// Draws the game.
	/// </summary>
	void Draw() const override;

	/// <summary>
	/// Gets the description for this game mode, which will be drawn on-screen.
	/// </summary>
	/// <returns>The description for this game mode, which will be drawn on-screen.</returns>
	std::string GetDescription() const override;

private:
	/// <summary>
	/// Serializes the local game state into a buffer, for network transmission.
	/// </summary>
	/// <returns>The number of bytes serialized.</returns>
	size_t SerializeLocalState() const;

	/// <summary>
	/// Deserializes the remote state received over the network.
	/// </summary>
	/// <param name="bytes_received">The number of bytes received over the network.</param>
	void DeserializeRemoteState(const size_t bytes_received);

	/// <summary>
	/// Implements game logic to handle collisions between players.
	/// </summary>
	/// <param name="old_local_position">The previous position of the local player</param>
	/// <param name="old_remote_position">The previous position of the remote player</param>
	/// <remarks>The host player will "win" collisions, forcing the remote player to rewind.  If the remote player had not moved, the host is forced to rewind.</remarks>
	void HandleCollision(const BoardPosition old_local_position, const BoardPosition old_remote_position);

	SOCKET socket_;
	bool is_host_;
	bool is_local_update_sent_;
	int turn_number_;
	char* network_buffer_;
	Player local_player_{ 20, CP_Color_Create(255, 0, 0, 255), {0, 0} };
	Player remote_player_{ 20, CP_Color_Create(0, 255, 0, 255), {1, 0} };
};