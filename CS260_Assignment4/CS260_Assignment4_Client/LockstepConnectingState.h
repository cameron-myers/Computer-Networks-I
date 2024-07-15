//---------------------------------------------------------
// file:	LockstepConnectingState.h
// author:	Matthew Picioccio
// email:	matthew.picioccio@digipen.edu
//
// brief:	Global functions for the "Connecting to Lockstep" game state
//
// Copyright © 2020 DigiPen, All rights reserved.
//---------------------------------------------------------
#pragma once
#include <WinSock2.h>

/// <summary>
/// Starts the user experience to attempt to connect to a lockstep-networked game.
/// </summary>
void ConnectLockstep(const u_short port);