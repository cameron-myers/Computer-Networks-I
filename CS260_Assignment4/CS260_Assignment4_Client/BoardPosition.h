//---------------------------------------------------------
// file:	BoardPosition.h
// author:	Matthew Picioccio
// email:	matthew.picioccio@digipen.edu
//
// brief:	Data and operations for the positions on a board.
//
// Copyright © 2020 DigiPen, All rights reserved.
//---------------------------------------------------------
#pragma once
#include <utility>

using namespace std::rel_ops;


/// <summary>
/// Data and operations for the positions on a board.
/// </summary>
struct BoardPosition
{
	int x;
	int y;

	bool operator==(const BoardPosition& other) const
	{
		return (x == other.x) && (y == other.y);
	}
};