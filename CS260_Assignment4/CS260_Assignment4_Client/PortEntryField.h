//---------------------------------------------------------
// file:	PortEntryField.h
// author:	Matthew Picioccio
// email:	matthew.picioccio@digipen.edu
//
// brief:	Provides support for a type-able field to enter network ports, in C-Processing.
//
// Copyright © 2020 DigiPen, All rights reserved.
//---------------------------------------------------------
#pragma once
#include <WinSock2.h>
#include <string>


/// <summary>
/// Provides support for a type-able field to enter network ports, in C-Processing.
/// </summary>
class PortEntryField
{
public:
	/// <summary>
	/// Retrieves the user-entered port, as a u_short.
	/// </summary>
	/// <returns>The user-entered port, as a u_short.</returns>
	u_short GetPort() const { return entered_port_; }

	/// <summary>
	/// Retrieves the user-entered port, as a string.
	/// </summary>
	/// <returns>The user-entered port, as a string.</returns>
	std::string GetPortText() const { return entered_port_text_; }

	/// <summary>
	/// Validates if the network port currently entered is valid for use in sockets in this app.
	/// </summary>
	/// <returns>True if the network port is valid for use in sockets for this app.</returns>
	bool IsPortValid() const;

	/// <summary>
	/// Per-frame update, checking keys for data entry.
	/// </summary>
	void Update();

private:
	/// <summary>
	/// Adds the typed character to the port
	/// </summary>
	/// <param name="digit_char">The character typed in</param>
	void AddToPort(const char digit_char);
	
	u_short entered_port_ = 0;
	std::string entered_port_text_;
};