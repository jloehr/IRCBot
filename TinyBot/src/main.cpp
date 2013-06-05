/*
 *
 * Copyright (C) 2013 Julian Löhr
 * All rights reserved.
 *
 * This file is part of TinyBot, an IRC Bot.
 * TinyBot is licensed under a Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License.
 * To read the full license, please check the LICENSE file.
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