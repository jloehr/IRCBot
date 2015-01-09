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
#include <iostream> 

#include "Output.h"
#include "TinyBotFactory.h"
#include "TinyBot.h"

int main( int argc, const char* argv[] )
{
	//std::cout << "Starting TinyBot a tiny IRC Bot" << std::endl << std::endl;
	Output::Log({"Starting TinyBot a tiny IRC Bot"});

	CTinyBot * BotInstance = CTinyBotFactory::CreateTinyBot(argc, argv);

	BotInstance->Run();

	delete BotInstance;

	return 0;
}