#include "main.hpp"

bool loop = true;

void	sighandler(int)
{
	loop = false;
	std::cout << "\r  " << std::endl;
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

	Bot bot("ircbot", "ircbot", "Beep BOOP", '>');

	int	ret = 0;

	try
	{
		bot.socket_setup(av[1], av[2]);
		bot.create_epoll();
		bot.authentication(av[3]);

		while (loop)
		{
			bot.epoll_loop();
		}
		
		bot.add_quit_message();
		while (!bot._send_buffer.empty())
		{
			bot.epoll_loop();
		}
	}
	catch (const std::exception& e)
	{
		printError(e.what());
		ret = -1;
	}

	std::cout << BRED << "Shutting down..." << RESET << std::endl;

	return ret;
}
