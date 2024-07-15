//---------------------------------------------------------
// file:	SinglePlayerGame.cpp
// author:	Matthew Picioccio
// email:	matthew.picioccio@digipen.edu
//
// brief:	An extremely simple single-player "game", intended as a reference implementation.
//
// Copyright © 2020 DigiPen, All rights reserved.
//---------------------------------------------------------
#include "SinglePlayerGame.h"
#include "LocalPlayerController.h"


/// <summary>
/// Constructs a SinglePlayerGame object.
/// </summary>
SinglePlayerGame::SinglePlayerGame()
{
	player_.current_position = { board_.GetTileCountX() / 2, board_.GetTileCountY() / 2 };
	player_.has_uncommitted_move = false;
}


/// <summary>
/// Per-frame update of the game logic.
/// </summary>
void SinglePlayerGame::Update()
{
	// update the player input
	LocalPlayerController::ApplyInput(player_, board_);

	// immediately commit the move (if there was one)
	player_.CommitMove();
}


/// <summary>
/// Draws the game.
/// </summary>
void SinglePlayerGame::Draw() const
{
	BoardGame::Draw();

	// draw the player in the center of their current position
	const auto player_center = board_.GetCenterOfTile(player_.current_position);
	player_.Draw(player_center);
}


/// <summary>
/// Gets the description for this game mode, which will be drawn on-screen.
/// </summary>
/// <returns>The description for this game mode, which will be drawn on-screen.</returns>
std::string SinglePlayerGame::GetDescription() const
{
	return std::string("Single Player, Press 2 for Lockstep");
}