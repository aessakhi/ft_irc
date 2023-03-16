#include "main.hpp"

bool loop = true;

void	sighandler(int)
{
	loop = false;
}

/*
argv[0] should be the requested port,
argv[1] should be the server password.
*/
int main(int argc, char **argv)
{
	if (program_arguments_check(argc, argv))
	{
		return -1;
	}

	signal(SIGINT, sighandler);

	std::cout << BRED << "Starting up..." << RESET << std::endl;

	Server ircserv("ircserv", argv[1], argv[2], HOSTNAME, "password");

	try
	{

		ircserv.init();

		while (loop)
			ircserv.epoll_loop();

	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}

	std::cout << "\r      " << std::endl << BRED << "Shutting down..." << RESET << std::endl;

	return 0;
}
