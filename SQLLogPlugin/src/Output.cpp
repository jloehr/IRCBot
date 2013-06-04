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
