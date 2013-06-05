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
#pragma once

#include <iostream> 
#include <initializer_list>

class Output
{
public:
	static void Log(std::initializer_list<const char * > Messages);
	static void Log(const char * Sender, std::initializer_list<const char * > Messages);
	static void Error(const char * Sender, std::initializer_list<const char * > Messages);

};