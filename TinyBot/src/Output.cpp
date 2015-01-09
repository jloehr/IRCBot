/*
 *
 * Copyright (C) 2013 Julian Löhr
 * All rights reserved.
 *
 * This file is part of TinyBot, an IRC Bot.
 * TinyBot is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * TinyBot is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with TinyBot.  If not, see <http://www.gnu.org/licenses/>.
 *
*/
#include "Output.h"


void Output::Log(std::initializer_list<const char * > Messages)
{
	std::cout << std::endl;

	for (const char * Message : Messages)
        std::cout << Message;

	std::cout << std::endl << std::endl;
}

void Output::Log(const char * Sender, std::initializer_list<const char * > Messages)
{
	std::cout << Sender << ": ";

	for (const char * Message : Messages)
        std::cout << Message;

	std::cout << std::endl;
}

void Output::Error(const char * Sender, std::initializer_list<const char * > Messages)
{
	std::cerr << Sender << ": ";
	
	for (const char * Message : Messages)
        std::cerr << Message;

	std::cerr << std::endl;
}
