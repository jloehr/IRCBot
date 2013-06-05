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
