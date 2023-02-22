#ifndef COMMAND_HPP
# define COMMAND_HPP

#include "main.hpp"

class Command
{
private:
	Command();

	std::string					_cmd_name;
	std::string					_param;
	std::vector<std::string>	_paramList;
public:
	Command(std::string name, std::string param, std::vector<std::string> param_list);

	~Command();

	/* Returns upper version of the cmd. */
	std::string getCmd() const;

	/* Returns cmd, might contain lower chars. */
	std::string	getRawCmd() const;
	std::string getParam() const;
	std::vector<std::string>	getParamList() const;
};

std::ostream & operator<<(std::ostream & out, const Command & cmd);

#endif
