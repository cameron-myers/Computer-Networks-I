//---------------------------------------------------------
// file:	Player.h
// author:	Matthew Picioccio
// email:	matthew.picioccio@digipen.edu
//
// brief:	Data and functionality for the player object, across all game modes.
//
// Copyright © 2020 DigiPen, All rights reserved.
//---------------------------------------------------------
#pragma once

#include "cprocessing.h"
#include "BoardPosition.h"

/// <summary>
/// Data and functionality for the player object, across all game modes.
/// </summary>
struct Player
{
	int size = 25;
	CP_Color color = CP_Color_Create(0, 0, 255, 255);
	
	BoardPosition current_position { 0, 0 };
	BoardPosition uncommitted_position { 0, 0 };
	bool has_uncommitted_move = false;

	/// <summary>
	/// Apply the pending, uncommitted move to the player.
	/// </summary>
	void CommitMove();

	/// <summary>
	/// Draw the player object at the specified location.
	/// </summary>
	/// <param name="center">The coordinate of the center of the player object.</param>
	void Draw(const CP_Vector center) const;
};