#ifndef COMMAND_HPP
# define COMMAND_HPP

#include "main.hpp"

class Command
{
private:

	Command();

	/* Prefix of the command.
	If not empty, begins with ':'. */
	std::string					_prefix;

	/* Name of the command. */
	std::string					_cmd_name;

	/* List of parameters following the command. */
	std::vector<std::string>	_param_list;

public:

	Command(std::string name, std::vector<std::string> param_list, std::string prefix);

	~Command();

	/* Returns upper version of the cmd. */
	std::string getCmd() const;

	/* Returns cmd, might contain lower chars. */
	std::string	getRawCmd() const;

	/* Returns prefix.
	If the command has no prefix, returns empty string. */
	std::string getPrefix() const;

	/* If true, command has a prefix that is not empty. */
	bool	hasPrefix() const;

	/* Returns parameter list. */
	std::vector<std::string>	getParamList() const;

	/* Returns parameter at index i. */
	std::string	getParam(size_t i) const;

	/* Returns number of parameters in the _param_list */
	size_t	paramNumber() const;

	/* Returns the last parameter, without the starting ':' if there was one. */
	std::string	getLastParam() const;

	/* Returns command as a string */
	std::string getStr() const;
};

std::ostream & operator<<(std::ostream & out, const Command & cmd);

#endif
