//---------------------------------------------------------
// file:	PlayGameState.cpp
// author:	Matthew Picioccio
// email:	matthew.picioccio@digipen.edu
//
// brief:	Global functions for the "Play Game" game state
//
// Copyright © 2020 DigiPen, All rights reserved.
//---------------------------------------------------------
#include "PlayGameState.h"
#include "SinglePlayerGame.h"
#include "LockstepConfiguringState.h"


BoardGame* current_game = nullptr;
BoardGame* next_game = nullptr;


/// <summary>
/// Handles entry into this game state
/// </summary>
void PlayGameState_Init(void)
{
	// apply the queued game
	current_game = next_game;
	next_game = nullptr;
}


/// <summary>
/// Per-frame update for this game state.
/// </summary>
void PlayGameState_Update(void)
{
	// clear the background
	CP_Settings_Background(CP_Color_Create(0, 0, 40, 255));

	// if the user presses ESC from gameplay, the process will exit.
	if (CP_Input_KeyTriggered(KEY_ESCAPE))
	{
		CP_Engine_Terminate();
		return;
	}

	// transition between game modes
	// -- note that redundant selections will still reset the game state
	if (CP_Input_KeyTriggered(KEY_1))
	{
		// starts a new single-player gates
		PlayGame(new SinglePlayerGame());
	}
	else if (CP_Input_KeyTriggered(KEY_2))
	{
		ConfigureLockstep();
	}

	// update and draw the current game
	if (current_game != nullptr)
	{
		current_game->Update();
		current_game->Draw();
	}
}


/// <summary>
/// Handles departure from this game state.
/// </summary>
void PlayGameState_Exit(void)
{
	// destroy the current game
	delete current_game;
	current_game = nullptr;
}


/// <summary>
/// Queue the next game to be played, starting with the next frame.
/// </summary>
/// <param name="game">The next game to be played</param>
void PlayGame(BoardGame* game)
{
	// ignore null games
	if (game == nullptr)
	{
		return;
	}

	// discard any other queued game
	delete next_game;
	next_game = game;

	// queue this gamestate in the engine
	/// -- "Forced" ensures that the exit/init cycle is followed, even if the engine is already in this game state
	CP_Engine_SetNextGameStateForced(PlayGameState_Init, PlayGameState_Update, PlayGameState_Exit);
}


/// <summary>
/// Return to the default play mode for the game.
/// </summary>
/// <remarks>As of now, the default mode is single-player.</remarks>
void PlayDefaultGame()
{
	// start a new single-player game
	PlayGame(new SinglePlayerGame());
}