#ifndef COMMAND_HPP
# define COMMAND_HPP

#include "main.hpp"

class Command
{
private:
	/* Prefix of the command. */
	std::string					_prefix;

	/* Command. */
	std::string					_cmd;

	/* Command arguments. */
	std::vector<std::string>	_args;

public:
	Command();

	Command(std::string name, std::vector<std::string> args, std::string prefix);

	~Command();

	/* Returns upper version of the cmd. */
	std::string getCmd() const;

	/* Returns prefix or empty string */
	std::string getPrefix() const;

	/* Returns arg list. */
	std::vector<std::string>	getArgs() const;

	/* Returns arg at index i. */
	std::string	getArg(size_t i) const;

	/* Returns number of args in _args */
	size_t	argNumber() const;

	/* Returns command as a string */
	std::string getStr() const;
};

std::ostream & operator<<(std::ostream & out, const Command & cmd);

#endif
