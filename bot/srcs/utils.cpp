#include "main.hpp"

void	printDebug(std::string s)
{
	std::cout << MGT << s << RESET << std::endl;
}

void	printCmdBuf(std::vector<Command> v)
{
	std::vector<Command>::const_iterator it = v.begin();
	std::vector<Command>::const_iterator ite = v.end();
	for (; it != ite; it++)
	{
		std::cout << *it << std::endl;
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
	std::cout << BLU << "<< \"" << no_crlf(to_print) << "\"" << RESET << std::endl;
}

std::string toupper(std::string s)
{
	std::string copy = s;
	for (size_t i = 0; i < s.size(); i++)
		copy[i] = static_cast<char>(toupper(s[i]));
	return copy;
}

std::vector<std::string> split(std::string str, std::string delimiter)
{
	std::vector<std::string> result;

	size_t		start = 0;
	size_t		end = str.find(delimiter);
	std::string	small_str;

	while (end != std::string::npos)
	{
		small_str = str.substr(start, end - start);
		if (!small_str.empty())
			result.push_back(small_str);
		start = end + delimiter.length();
		end = str.find(delimiter, start);
	}
	small_str = str.substr(start, end - start);
	if (!small_str.empty())
		result.push_back(small_str);
	return (result);
}

std::string	itoa(int n)
{
	std::stringstream out;
	out << n;
	return out.str();
}