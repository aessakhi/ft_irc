#include "main.hpp"

bool loop = true;

void	sighandler(int)
{
	loop = false;
}

static bool _error_check(int ac)
{
	if (ac != 4)
	{
		printError("Usage: ./ircbot <address> <port> <password>");
		return true;
	}

	return false;
}

int main(int ac, char *av[])
{
	if (_error_check(ac))
		return -1;

	signal(SIGINT, sighandler);

	std::cout << BRED << "Starting up..." << RESET << std::endl;

	Bot bot("ircbot", "ircbot", "Beep BOOP");

	try
	{
		bot.socket_setup(av[1], av[2]);
		if (bot._fd < 0)
			return -1;
		bot.authentication(av[3]);
		while (loop)
		{
			bot.loop();
		}
	}
	catch (const std::exception& e)
	{
		printError(e.what());
	}

	std::cout << "\r      " << std::endl << BRED << "Shutting down..." << RESET << std::endl;

	return 0;
}
