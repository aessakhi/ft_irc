#include "main.hpp"

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
	std::string str(s, n);
	std::cout << no_crlf(str) << std::endl;
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
