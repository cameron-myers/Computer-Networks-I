//---------------------------------------------------------
// file:	BoardGame.h
// author:	Matthew Picioccio
// email:	matthew.picioccio@digipen.edu
//
// brief:	General data and functionality for all game modes.
//
// Copyright © 2020 DigiPen, All rights reserved.
//---------------------------------------------------------
#pragma once

#include "cprocessing.h"
#include <string>
#include "Board.h"


/// <summary>
/// General data and functionality for all game modes.
/// </summary>
class BoardGame
{
public:
	BoardGame() = default;
	virtual ~BoardGame() = default;

	BoardGame(const BoardGame&) = delete;
	BoardGame(BoardGame&&) = delete;
	BoardGame& operator=(const BoardGame&) = delete;
	BoardGame& operator=(BoardGame&&) = delete;

	/// <summary>
	/// Per-frame update of the game logic.
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// Draws the game.
	/// </summary>
	virtual void Draw() const;

	/// <summary>
	/// Gets the description for this game mode, which will be drawn on-screen.
	/// </summary>
	/// <returns>The description for this game mode, which will be drawn on-screen.</returns>
	virtual std::string GetDescription() const = 0;

protected:
	Board board_;
};