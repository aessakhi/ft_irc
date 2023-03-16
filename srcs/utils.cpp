#include "utils.hpp"

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

bool isAuthcmd(const std::string &cmd)
{
	if (cmd.compare("CAP") == 0 || cmd.compare("PASS") == 0 || cmd.compare("NICK") == 0 || cmd.compare("USER") == 0)
		return (true);
	return (false);
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

std::vector<std::string> split(std::string *str, std::string delimiter)
{
	std::vector<std::string> result;

	size_t	end = str->find(delimiter);

	if (end == std::string::npos)
		return (std::vector<std::string>());
	while (end != std::string::npos)
	{
		result.push_back(str->substr(0, end));
		str->erase(0, end + delimiter.length());
		end = str->find(delimiter);
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
		start = end + delimiter.length();
		end = str.find(delimiter, start);
	}
	return (result);
}

Command	splitCmd(std::string s)
{
	std::string	prefix;
	std::string	command_name;
	std::vector<std::string>	parameter_list;

	// prefix grab
	if (s[0] == ':')
	{
		size_t prefix_end = s.find(' ');
		prefix = s.substr(0, prefix_end);
		s.erase(0, prefix_end);
	}

	// command grab
	size_t cmd_start = s.find_first_not_of(' ');
	size_t cmd_end = s.find(' ', cmd_start);

	if (cmd_start != s.npos)
	{
		command_name = s.substr(cmd_start, cmd_end);
		s.erase(0, cmd_end);
	}

	// param-list grab

	size_t arg_start = s.find_first_not_of(' ');
	size_t arg_end;

	while (arg_start != s.npos)
	{
		if (s[arg_start] == ':')
			arg_end = s.npos;
		else
			arg_end = s.find(' ', arg_start);
		parameter_list.push_back(s.substr(arg_start, arg_end - arg_start));
		s.erase(0, arg_end);
		arg_start = s.find_first_not_of(' ');
	}

	Command cmd(command_name, parameter_list, prefix);
	return (cmd);
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
			int test = 0;
			while (true)
			{
				if (wildcompare(s1 + i + 1, s2 + i + test))
					return true;
				if (!s2[test])
					break;
				test++;
			};
		}

		// * wildcard on s2
		if ( (s1[i] != s2[i]) && (s2[i] == '*') )
		{
			// skip 0, 1, 2... characters in s1
			int test = 0;
			while (true)
			{
				if (wildcompare(s1 + i + test, s2 + i + 1))
					return true;
				if (!s1[test])
					break;
				test++;
			};
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
	return copy;
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

bool	sided_wildcompare(const char * s1, const char * s2)
{
	for (int i = 0; s1[i] || s2[i]; i++)
	{
		// case-insensitive basic comparison
		if (s1[i] && s2[i] && (toupper(s1[i]) == toupper(s2[i])) )
			continue;
		
		// single char wildcard
		if ( (s1[i] != s2[i]) && (s1[i] == '?') )
			continue;
		
		// * wildcard on s1
		if ( (s1[i] != s2[i]) && (s1[i] == '*') )
		{
			// skip 0, 1, 2... characters in s2
			int test = 0;
			while (true)
			{
				if (sided_wildcompare(s1 + i + 1, s2 + i + test))
					return true;
				if (!s2[test])
					break;
				test++;
			};
		}

		return false;
	}

	return true;
}

bool	sided_wildcompare(std::string s1, std::string s2)
{
	return sided_wildcompare(s1.data(), s2.data());
}

std::vector<std::string> actual_split(std::string str, std::string delimiter)
{
	std::vector<std::string> result;

	size_t	start = 0;
	size_t	end = str.find(delimiter);

	while (end != std::string::npos)
	{
		//changed the value from (start, end) to (start, end - start);
		result.push_back(str.substr(start, end - start));
		start = end + delimiter.length();
		end = str.find(delimiter, start);
	}
	result.push_back(str.substr(start, end));
	return (result);
}
