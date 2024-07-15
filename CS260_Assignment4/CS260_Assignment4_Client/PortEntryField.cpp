//---------------------------------------------------------
// file:	PortEntryField.cpp
// author:	Matthew Picioccio
// email:	matthew.picioccio@digipen.edu
//
// brief:	Provides support for a type-able field to enter network ports, in C-Processing.
//
// Copyright © 2020 DigiPen, All rights reserved.
//---------------------------------------------------------
#include "PortEntryField.h"
#include "cprocessing.h"


/// <summary>
/// Validates if the network port currently entered is valid for use in sockets in this app.
/// </summary>
/// <returns>True if the network port is valid for use in sockets for this app.</returns>
bool PortEntryField::IsPortValid() const
{
	return entered_port_ >= 100;
}


/// <summary>
/// Per-frame update, checking keys for data entry.
/// </summary>
void PortEntryField::Update()
{
	if (CP_Input_KeyTriggered(KEY_0))
	{
		AddToPort('0');
	}
	if (CP_Input_KeyTriggered(KEY_1))
	{
		AddToPort('1');
	}
	if (CP_Input_KeyTriggered(KEY_2))
	{
		AddToPort('2');
	}
	if (CP_Input_KeyTriggered(KEY_3))
	{
		AddToPort('3');
	}
	if (CP_Input_KeyTriggered(KEY_4))
	{
		AddToPort('4');
	}
	if (CP_Input_KeyTriggered(KEY_5))
	{
		AddToPort('5');
	}
	if (CP_Input_KeyTriggered(KEY_6))
	{
		AddToPort('6');
	}
	if (CP_Input_KeyTriggered(KEY_7))
	{
		AddToPort('7');
	}
	if (CP_Input_KeyTriggered(KEY_8))
	{
		AddToPort('8');
	}
	if (CP_Input_KeyTriggered(KEY_9))
	{
		AddToPort('9');
	}
}


/// <summary>
/// Adds the typed character to the port
/// </summary>
/// <param name="digit_char">The character typed in</param>
/// <remarks>Validates the character and avoids overflows.</remarks>
void PortEntryField::AddToPort(const char digit_char)
{
	// validate the parameter
	if ((digit_char < '0') || (digit_char > '9'))
	{
		return;
	}

	// limit to 5 digits
	if (this->entered_port_ >= 10000)
	{
		return;
	}

	// add the digit to the new number
	// -- ensure that the new number doesn't overflow before assigning it!
	const unsigned int new_port =  (10 * entered_port_) + (digit_char - '0');
	if ((new_port > 0) && (new_port <= 65535))
	{
		entered_port_ = static_cast<u_short>(new_port);
		entered_port_text_ += digit_char;
	}
}