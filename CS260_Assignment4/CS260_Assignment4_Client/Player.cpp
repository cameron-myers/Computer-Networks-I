//---------------------------------------------------------
// file:	Player.cpp
// author:	Matthew Picioccio
// email:	matthew.picioccio@digipen.edu
//
// brief:	Data and functionality for the player object, across all game modes.
//
// Copyright © 2020 DigiPen, All rights reserved.
//---------------------------------------------------------
#include "Player.h"


/// <summary>
/// Apply the pending, uncommitted move to the player.
/// </summary>
void Player::CommitMove()
{
	if (has_uncommitted_move)
	{
		current_position = uncommitted_position;
		has_uncommitted_move = false;
	}
}


/// <summary>
/// Draw the player object at the specified location.
/// </summary>
/// <param name="center">The coordinate of the center of the player object.</param>
void Player::Draw(const CP_Vector center) const
{
	CP_Settings_Fill(color);
	if (has_uncommitted_move)
	{
		CP_Settings_Stroke(CP_Color_Create(0, 0, 0, 255));;
		const auto destination_x = center.x + (uncommitted_position.x - current_position.x) * size;
		const auto destination_y = center.y + (uncommitted_position.y - current_position.y) * size;
		CP_Graphics_DrawLine(center.x, center.y, destination_x, destination_y);
	}
	else
	{
		CP_Settings_NoStroke();
	}

	CP_Graphics_DrawCircle(center.x, center.y, static_cast<float>(size));
}