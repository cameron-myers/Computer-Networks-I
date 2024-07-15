//---------------------------------------------------------
// file:	LocalPlayerController.h
// author:	Matthew Picioccio
// email:	matthew.picioccio@digipen.edu
//
// brief:	Functionality to apply local input to a player object.
//
// Copyright © 2020 DigiPen, All rights reserved.
//---------------------------------------------------------
#pragma once

class Board;
struct Player;


/// <summary>
/// Functionality to apply local input to a player object.
/// </summary>
class LocalPlayerController final
{
public:
	/// <summary>
	/// Apply local input to a player object, validated against a board.
	/// </summary>
	/// <param name="player">The player that will be moved.</param>
	/// <param name="board">The board that validates the move.</param>
	static void ApplyInput(Player& player, const Board& board);
};