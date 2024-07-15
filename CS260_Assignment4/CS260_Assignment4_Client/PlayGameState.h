//---------------------------------------------------------
// file:	PlayGameState.h
// author:	Matthew Picioccio
// email:	matthew.picioccio@digipen.edu
//
// brief:	Global functions for the "Play Game" game state
//
// Copyright © 2020 DigiPen, All rights reserved.
//---------------------------------------------------------
#pragma once

class BoardGame;


/// <summary>
/// Queue the next game to be played, starting with the next frame.
/// </summary>
void PlayGame(BoardGame* game);

/// <summary>
/// Return to the default game mode.
/// </summary>
void PlayDefaultGame();