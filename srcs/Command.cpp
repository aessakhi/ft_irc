#include "main.hpp"
#include "Command.hpp"

Command::Command(std::string name, std::string param, std::vector<std::string> param_list): _cmd_name(name), _param(param), _paramList(param_list)
{}

Command::~Command()
{}

std::string	Command::getCmd() const
{
	return (this->_cmd_name);
}

std::string	Command::getParam() const
{
	return (this->_param);
}

std::vector<std::string> Command::getParamList() const
{
	return (this->_paramList);
}
