
#include "Timer.h"
#include "BreakoutGame.h"
#include <iostream>
#include <stdexcept>

using namespace Breakout;

int main(int argc, char** argv)
{
	try
	{
		BreakoutGame game;
		game.Run();
	}
	catch (std::runtime_error& e)
	{
		std::cout << e.what() << "\n";
		return 1;
	}
	return 0;
}