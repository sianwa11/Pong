#include "Game.h"
#include <ctime>

int main()
{
	std::srand(static_cast<unsigned int>(std::time(0)));  // Seed the RNG

	Game g("Config/init.ini");
	g.run();
}