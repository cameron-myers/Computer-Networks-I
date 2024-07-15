//---------------------------------------------------------
// file:	LockstepHostingState.h
// author:	Matthew Picioccio
// email:	matthew.picioccio@digipen.edu
//
// brief:	Global functions for the "Hosting Lockstep" game state
//
// Copyright © 2020 DigiPen, All rights reserved.
//---------------------------------------------------------
#pragma once
#include <WinSock2.h>

/// <summary>
/// Starts the user experience to attempt to host a lockstep-networked game.
/// </summary>
void HostLockstep(const u_short port);