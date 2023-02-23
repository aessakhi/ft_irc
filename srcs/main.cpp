#include "main.hpp"

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

	Server ircserv("ircserv", argv[1], argv[2], HOSTNAME);

	ircserv.init();
}
