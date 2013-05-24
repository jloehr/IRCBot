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