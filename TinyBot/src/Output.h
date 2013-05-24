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