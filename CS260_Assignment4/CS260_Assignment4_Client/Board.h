//---------------------------------------------------------
// file:	Board.h
// author:	Matthew Picioccio
// email:	matthew.picioccio@digipen.edu
//
// brief:	Data, rendering functionality, and calculations related to the board used in all game modes. 
//
// Copyright © 2020 DigiPen, All rights reserved.
//---------------------------------------------------------
#pragma once

#include "cprocessing.h"
#include "BoardPosition.h"


/// <summary>
/// Data, rendering functionality, and calculations related to the board used in all game modes. 
/// </summary>
class Board final
{
public:
	/// <summary>
	/// Provides the number of tiles in each row.
	/// </summary>
	/// <returns>The number of tiles in each row.</returns>
	int GetTileCountX() const { return tile_count_x_; }
	
	/// <summary>
	/// Provides the number of tiles in each column.
	/// </summary>
	/// <returns>The number of tiles in each column.</returns>
	int GetTileCountY() const { return tile_count_y_; }

	/// <summary>
	/// Calculates the center of the given tile.
	/// </summary>
	/// <param name="position">The board position of the tile in question.</param>
	/// <returns>The coordinate of the center of the tile in question.</returns>
	CP_Vector GetCenterOfTile(const BoardPosition position) const;

	/// <summary>
	/// Determines if the provided board position is valid for the current board dimensions.
	/// </summary>
	/// <param name="position">The board position in question.</param>
	/// <returns>True if the board position is within the current board dimensions; false otherwise.</returns>
	bool IsValidTile(const BoardPosition position) const;

	/// <summary>
	/// Draws the board.
	/// </summary>
	void Draw() const;

private:
	float tile_size_ = 30.0f;
	float border_size_ = 5.0f;
	int tile_count_x_ = 23, tile_count_y_ = 17;

	CP_Vector center_ = CP_Vector_Set(512.0f, 399.0f);
	CP_Vector size_ = CP_Vector_Set(810.0f, 600.0f);
	
	CP_Color board_color_ = CP_Color_Create(100, 100, 100, 255);
	CP_Color tile_color_ = CP_Color_Create(255, 255, 255, 255);
};
