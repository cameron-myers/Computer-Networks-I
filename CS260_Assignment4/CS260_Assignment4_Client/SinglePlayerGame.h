//---------------------------------------------------------
// file:	SinglePlayerGame.h
// author:	Matthew Picioccio
// email:	matthew.picioccio@digipen.edu
//
// brief:	An extremely simple single-player "game", intended as a reference implementation.
//
// Copyright © 2020 DigiPen, All rights reserved.
//---------------------------------------------------------
#pragma once

#include "BoardGame.h"
#include "Player.h"


/// <summary>
/// An extremely simple single-player "game", intended as a reference implementation.
/// </summary>
class SinglePlayerGame final : public BoardGame
{
public:
	SinglePlayerGame();
	~SinglePlayerGame() = default;

	SinglePlayerGame(const SinglePlayerGame&) = delete;
	SinglePlayerGame(SinglePlayerGame&&) = delete;
	SinglePlayerGame& operator=(const SinglePlayerGame&) = delete;
	SinglePlayerGame& operator=(SinglePlayerGame&&) = delete;

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
	Player player_ = { 20, CP_Color_Create(255, 0, 0, 255), {0, 0} };
};