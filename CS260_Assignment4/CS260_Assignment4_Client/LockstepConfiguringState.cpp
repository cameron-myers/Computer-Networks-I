//---------------------------------------------------------
// file:	LockstepConfiguringState.cpp
// author:	Matthew Picioccio
// email:	matthew.picioccio@digipen.edu
//
// brief:	Global functions for the "Configuring Lockstep" game state
//
// Copyright © 2020 DigiPen, All rights reserved.
//---------------------------------------------------------
#include <cassert>
#include <string>
#include "cprocessing.h"
#include "LockstepConfiguringState.h"
#include "PortEntryField.h"
#include "PlayGameState.h"
#include "LockstepConnectingState.h"


PortEntryField* port_entry_field = nullptr;


/// <summary>
/// Handles entry into this game state
/// </summary>
void LockstepConfiguringState_Init()
{
	port_entry_field = new PortEntryField();
}


/// <summary>
/// Per-frame update for this game state.
/// </summary>
void LockstepConfiguringState_Update()
{
	assert(port_entry_field != nullptr);

	// update the port-entry "text field
	port_entry_field->Update();

	// if the user presses ESC from this screen, return to the default game mode.
	if (CP_Input_KeyTriggered(KEY_ESCAPE))
	{
		PlayDefaultGame();
	}

	// if the user presses enter and they have entered a valid port, proceed to the next step - attempting to connect to an existing server.
	if (CP_Input_KeyTriggered(KEY_ENTER) && port_entry_field->IsPortValid())
	{
		ConnectLockstep(port_entry_field->GetPort());
	}

	// clear the background
	CP_Settings_Background(CP_Color_Create(0, 0, 40, 255));

	// draw the instructions and the current port value
	CP_Settings_TextSize(30);
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_LEFT, CP_TEXT_ALIGN_V_TOP);
	CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
	CP_Font_DrawText("Enter port for new lock-step game:", 0.0f, 0.0f);
	if (port_entry_field->IsPortValid())
	{
		CP_Font_DrawText("ESC for Single Player, ENTER to continue", 0.0f, 40.0f);
	}
	else
	{
		CP_Font_DrawText("ESC for Single Player", 0.0f, 40.0f);
	}
	CP_Font_DrawText(port_entry_field->GetPortText().c_str(), 0.0f, 80.0f);
}


/// <summary>
/// Handles departure from this game state.
/// </summary>
void LockstepConfiguringState_Exit()
{
	delete port_entry_field;
	port_entry_field = nullptr;
}


/// <summary>
/// Starts the user experience to configure a lockstep-networked game.
/// </summary>
void ConfigureLockstep()
{
	CP_Engine_SetNextGameState(LockstepConfiguringState_Init, LockstepConfiguringState_Update, LockstepConfiguringState_Exit);
}