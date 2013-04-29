#include <iostream> 

#include "TinyBotFactory.h"
#include "TinyBot.h"

int main( int argc, const char* argv[] )
{
	std::cout << "Starting TinyBot a tiny IRC Bot" << std::endl << std::endl;

	CTinyBot & BotInstance = CTinyBotFactory::CreateTinyBot(argc, argv);

	BotInstance.Run();

	return 0;
}