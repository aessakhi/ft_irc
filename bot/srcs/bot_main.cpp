#include "Bot.hpp"

bool loop = true;

void	sighandler(int)
{
	loop = false;
}

void	printError(std::string s)
{
	std::cerr << RED << s << RESET << std::endl;
}

void	printReply(std::string s)
{
	std::cout << BBLU << ">> \"" << s << "\"" << RESET << std::endl;
}

void	printRecv(char * s, size_t n)
{
	std::cout << std::endl << BLU << "<< \"";
	if (n >= 2)
	{
		if (s[n - 2] == '\r' && s[n - 1] == '\n')
			n -= 2;
		else if (s[n - 1] == '\n' || s[n - 1] == '\r')
			n--;
	}
	if (n == 1 && (s[0] == '\n' || s[0] == '\r'))
		n--;
	
	for (size_t i = 0; i < n; i++)
		std::cout << s[i];
	std::cout << "\"" << RESET << std::endl;
}

std::string	no_crlf(std::string s)
{
	size_t max = s.size();
	if (s[max - 2] == '\r' && s[max - 1] == '\n')
		return s.substr(0, max - 2);
	if (s[max - 1] == '\r' || s[max - 1] == '\n')
		return s.substr(0, max - 1);
	return s;
}

void	printRecv(std::string s, size_t n)
{
	std::string	to_print(s, 0, n);
	std::cout << BBLU << "<< \"" << no_crlf(to_print) << "\"" << RESET << std::endl;
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

	Bot bot;

	try
	{
		bot.socket_setup(av[1], av[2]);
		if (bot._fd < 0)
			return -1;
		bot.authentication(av[3], "ircbot", "ircbot", "Beep BOOP");
		while (loop)
		{
			if (bot.receive_message())
			{
				bot.parse_buffer();
				bot.print_parsed_buffer();
			}
		}
	}
	catch (const std::exception& e)
	{
		printError(e.what());
	}
}
