#include "print.hpp"

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

void	printRecv(std::string s, size_t n)
{
	std::string	to_print(s, 0, n);
	std::cout << BBLU << "<< \"" << no_crlf(to_print) << "\"" << RESET << std::endl;
}

void	printSep(Command cmd)
{
	if (cmd.hasPrefix())
		std::cout << BGRN << "PREFIX = \"" << cmd.getPrefix() << "\"" << RESET << std::endl;
	std::cout << BGRN << "NAME = \"" << cmd.getCmd() << "\"" << RESET << std::endl;
	std::cout << BGRN << "PARAMLIST = \"";
	std::vector<std::string>vect = cmd.getParamList();
	for (size_t i = 0; i < vect.size(); i++)
	{
		std::cout << vect[i];
		if (i + 1 != vect.size())
			std::cout << "\", \"";
	}
	std::cout << "\"" << RESET << std::endl;
}

void	printSep(std::vector<std::string> vect)
{
	std::cout << "(";
	for (size_t i = 0; i < vect.size(); i++)
	{
		std::cout << vect[i];
		if (i + 1 != vect.size())
			std::cout << ", ";
	}
	std::cout << ")";
}