//---------------------------------------------------------
// file:	BoardGame.cpp
// author:	Matthew Picioccio
// email:	matthew.picioccio@digipen.edu
//
// brief:	General data and functionality for all game modes.
//
// Copyright © 2020 DigiPen, All rights reserved.
//---------------------------------------------------------
#include "BoardGame.h"


/// <summary>
/// The size of the description text.
/// </summary>
const float kDescriptionSize = 30.0f;

/// <summary>
/// The color of the description text.
/// </summary>
const CP_Color kDescriptionColor = CP_Color_Create(255, 255, 255, 255);


/// <summary>
/// Draws the game.
/// </summary>
/// <remarks>This should be overridden in most game modes.</remarks>
void BoardGame::Draw() const
{
	// draw the board
	board_.Draw();

	// draw the description text in the upper-left corner
	CP_Settings_TextSize(kDescriptionSize);
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_LEFT, CP_TEXT_ALIGN_V_TOP);
	CP_Settings_Fill(kDescriptionColor);
	CP_Font_DrawText(GetDescription().c_str(), 0.0f, 0.0f);
}