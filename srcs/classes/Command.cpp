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

std::string Command::getStr() const
{
	std::string	ret = getPrefix();
	if (!ret.empty() && !getCmd().empty())
		ret += ' ';
	if (!getCmd().empty())
		ret += getCmd();
	size_t nb = paramNumber();
	if (nb)
	{
		ret += ' ';
		for (size_t i = 0; i < nb; i++)
		{
			ret += getParam(i);
			if (i + 1 != nb)
				ret += ' ';
		}
	}
	return ret;
}
