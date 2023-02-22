#include "main.hpp"

/* Returns true only if the given string could be a port. */
static bool _port_is_digit(char *port)
{
	for (size_t i = 0; port[i]; i++)
	{
		if (!isdigit(port[i]))
			return false;
	}
	return (port[0] != 0);
}


int program_arguments_check(int argc, char **argv)
{
	if (argc != 3)
	{
		printError("Error: Wrong number of arguments. Usage: ./ircsserv <port> <password>");
		return -1;
	}

	if (!_port_is_digit(argv[1]))
	{
		printError("Error: Port should be a numeric value");
		return -1;
	}

	if (!argv[2][0])
	{
		printError("Error: Password cannot be empty.");
		return -1;
	}
	
	return 0;
}

/* Might need to be tweaked a bit, might be an issue when a string has no trailing \r\n */
std::vector<std::string> split(std::string *str, std::string delimiter)
{
	std::vector<std::string> result;

	size_t	end = 0;

	if (str->find(delimiter) == std::string::npos)
		return (std::vector<std::string>());
	while (str->find(delimiter, 0) != std::string::npos)
	{
		end = str->find(delimiter, 0);
		result.push_back(str->substr(0, end));
		str->erase(0, end + delimiter.length());
	}
	return (result);
}

std::vector<std::string> split(std::string str, std::string delimiter)
{
	std::vector<std::string> result;

	size_t	start = 0;
	size_t	end = str.find(delimiter);

	while (end != std::string::npos)
	{
		result.push_back(str.substr(start, end));
		start = end + 2;
		end = str.find(delimiter, start);
	}
	return (result);
}

void	splitCmds(std::vector<Command> *cmd_vector, std::string cmd)
{
	size_t						end	= 0;
	std::string					name;
	std::string					param;
	std::vector<std::string>	param_list;

	if ((end = cmd.find(" ")) != std::string::npos)
	{
		name = cmd.substr(0, end);
		cmd.erase(0, end + 1);
	}
	if ((end = cmd.find(":")) != std::string::npos)
	{
		param = cmd.substr(0, end);
		cmd.erase(0, end + 1);
	}
	while ((end = cmd.find(" ")) != std::string::npos)
	{
		param_list.push_back(cmd.substr(0, end));
		cmd.erase(0, end + 1);
	}
	if (!cmd.empty())
		param_list.push_back(cmd);
	cmd_vector->push_back(Command(name, param, param_list));
}

bool	wildcompare(const char * s1, const char * s2)
{
	for (int i = 0; s1[i] || s2[i]; i++)
	{
		// case-insensitive basic comparison
		if (s1[i] && s2[i] && (toupper(s1[i]) == toupper(s2[i])) )
			continue;
		
		// single char wildcard
		if ( (s1[i] != s2[i]) && (s1[i] == '?' || s2[i] == '?') )
			continue;
		
		// * wildcard on s1
		if ( (s1[i] != s2[i]) && (s1[i] == '*') )
		{
			// skip 0, 1, 2... characters in s2
			for (int test = 0; s2[test]; test++)
			{
				if (wildcompare(s1 + i + 1, s2 + i + test))
					return true;
			}
		}

		// * wildcard on s2
		if ( (s1[i] != s2[i]) && (s2[i] == '*') )
		{
			// skip 0, 1, 2... characters in s1
			for (int test = 0; s1[test]; test++)
			{
				if (wildcompare(s1 + i + test, s2 + i + 1))
					return true;
			}
		}
		
		return false;
	}

	return true;
}

bool	wildcompare(std::string s1, std::string s2)
{
	return wildcompare(s1.data(), s2.data());
}

std::string toupper(std::string s)
{
	std::string copy = s;
	for (size_t i = 0; i < s.size(); i++)
		copy[i] = static_cast<char>(toupper(s[i]));
	return s;
}

std::string	no_crlf(std::string s)
{
	size_t max = s.size();
	if (s[max - 2] == '\r' && s[max - 1] == '\n')
		return s.substr(0, max - 2);
	return s;
}