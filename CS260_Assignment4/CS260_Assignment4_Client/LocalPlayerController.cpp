//---------------------------------------------------------
// file:	LocalPlayerController.h
// author:	Matthew Picioccio
// email:	matthew.picioccio@digipen.edu
//
// brief:	Functionality to apply local input to a player object.
//
// Copyright © 2020 DigiPen, All rights reserved.
//---------------------------------------------------------
#include "cprocessing.h"
#include "LocalPlayerController.h"
#include "Player.h"
#include "Board.h"


/// <summary>
/// Apply local input to a player object, validated against a board.
/// </summary>
/// <param name="player">The player that will be moved.</param>
/// <param name="board">The board that validates the move.</param>
void LocalPlayerController::ApplyInput(Player& player, const Board& board)
{
	// take no action if there is already an uncommitted move
	if (player.has_uncommitted_move)
	{
		return;
	}

	// SPACE is a "move" with no change in location
	// -- by definition, the current position is always valid, so there's no need to validate again
	if (CP_Input_KeyTriggered(KEY_SPACE))
	{
		player.uncommitted_position = player.current_position;
		player.has_uncommitted_move = true;
		return;
	}

	// arrow keys move the player in each direction
	// -- "trigger" is used instead of "pressed" to avoid persistent double-queuing of moves
	auto new_position = player.current_position;
	if (CP_Input_KeyTriggered(KEY_UP))
	{
		--new_position.y;
	}
	else if (CP_Input_KeyTriggered(KEY_DOWN))
	{
		++new_position.y;
	}
	if (CP_Input_KeyTriggered(KEY_LEFT))
	{
		--new_position.x;
	}
	else if (CP_Input_KeyTriggered(KEY_RIGHT))
	{
		++new_position.x;
	}

	// only store the new position if it's actually valid on the board
	if ((new_position != player.current_position) && board.IsValidTile(new_position))
	{
		player.uncommitted_position = new_position;
		player.has_uncommitted_move = true;
	}
}