#include "main.hpp"

void	vectprint(std::ostream & out, std::vector<std::string> vect)
{
	for (size_t i = 0; i < vect.size(); i++)
	{
		out << vect[i];
		if (i + 1 != vect.size())
			out << " ";
	}
}

void	printError(std::string s)
{
	std::cerr << RED << s << RESET << std::endl;
}

void	printReply(std::string s)
{
	std::cout << BBLU << ">> \"" << s << "\"" << RESET << std::endl;
}

void	printFcall(std::string s)
{
	std::cout << YLW << "Calling " << toupper(s) << " function" << RESET << std::endl;
}

void	printRecv(std::string s)
{
	std::cout << BBLU << "<< \"" << no_crlf(s) << "\"" << RESET << std::endl;
}