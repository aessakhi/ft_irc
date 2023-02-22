#include "Command.hpp"

Command::Command(std::string name, std::vector<std::string> param_list, std::string prefix = ""): _prefix(prefix), _cmd_name(name), _param_list(param_list)
{}

Command::~Command()
{}

std::string	Command::getCmd() const
{
	return (toupper(this->_cmd_name));
}

std::string	Command::getRawCmd() const
{
	return (this->_cmd_name);
}

std::string	Command::getPrefix() const
{
	return (this->_prefix);
}

bool	Command::hasPrefix() const
{
	return (!(this->_prefix.empty()));
}

std::vector<std::string> Command::getParamList() const
{
	return (this->_param_list);
}

std::string	Command::getParam(size_t i) const
{
	return (this->_param_list[i]);
}

size_t	Command::paramNumber() const
{
	return (this->_param_list.size());
}

std::string	Command::getLastParam() const
{
	std::string last = getParam(paramNumber() - 1);
	if (last[0] == ':')
		last.erase(0, 1);
	return (last);
}

std::ostream & operator<<(std::ostream & out, const Command & cmd)
{
	std::vector<std::string> params = cmd.getParamList();
	if (cmd.hasPrefix())
		out << cmd.getPrefix() << " ";
	out << cmd.getCmd() << " ";
	printVect(out, params);
	return out;
}