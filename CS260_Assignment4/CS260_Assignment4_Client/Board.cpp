//---------------------------------------------------------
// file:	Board.cpp
// author:	Matthew Picioccio
// email:	matthew.picioccio@digipen.edu
//
// brief:	Data, rendering functionality, and calculations related to the board used in all game modes. 
//
// Copyright © 2020 DigiPen, All rights reserved.
//---------------------------------------------------------
#include <cassert>
#include "Board.h"


/// <summary>
/// Calculates the center of the given tile.
/// </summary>
/// <param name="position">The board position of the tile in question.</param>
/// <returns>The coordinate of the center of the tile in question.</returns>
CP_Vector Board::GetCenterOfTile(const BoardPosition position) const
{
	// validate the parameter
	assert(IsValidTile(position));

	// calculate the center of the given tile, taking into account borders as well as the configurable center of the board.
	auto result = CP_Vector_Subtract(center_, CP_Vector_Scale(size_, 0.5f));
	result.x += (border_size_ + (tile_size_ / 2) + (static_cast<float>(position.x) * (tile_size_ + border_size_)));
	result.y += (border_size_ + (tile_size_ / 2) + (static_cast<float>(position.y) * (tile_size_ + border_size_)));
	
	return result;
}


/// <summary>
/// Determines if the provided board position is valid for the current board dimensions.
/// </summary>
/// <param name="position">The board position in question.</param>
/// <returns>True if the board position is within the current board dimensions; false otherwise.</returns>
bool Board::IsValidTile(const BoardPosition position) const
{
	return
		(position.x >= 0) && (position.x < tile_count_x_) &&
		(position.y >= 0) && (position.y < tile_count_y_);
}


/// <summary>
/// Draws the board.
/// </summary>
void Board::Draw() const
{
	// determine the top-left position of the board
	const auto board_top_left = CP_Vector_Subtract(center_, CP_Vector_Scale(size_, 0.5f));

	CP_Settings_NoStroke();

	// draw the board background - i.e., the borders
	CP_Settings_Fill(board_color_);
	CP_Graphics_DrawRect(board_top_left.x, board_top_left.y, size_.x, size_.y);

	// draw each tile, skipping the border dimensions
	CP_Settings_Fill(tile_color_);
	auto tile_top_left_x = board_top_left.x;
	auto tile_top_left_y = board_top_left.y;
	for (auto tile_y = 0; tile_y < tile_count_y_; ++tile_y, tile_top_left_y += tile_size_)
	{
		tile_top_left_y += border_size_;
		for (auto tile_x = 0; tile_x < tile_count_x_; ++tile_x, tile_top_left_x += tile_size_)
		{
			tile_top_left_x += border_size_;
			CP_Graphics_DrawRect(tile_top_left_x, tile_top_left_y, tile_size_, tile_size_);
		}
		tile_top_left_x = board_top_left.x;
	}
}