#include "Command.hpp"

Command::Command()
{}

Command::Command(std::string name, std::vector<std::string> args, std::string prefix = ""): _prefix(prefix), _cmd(name), _args(args)
{}

Command::~Command()
{}

std::string	Command::getCmd() const
{
	return toupper(_cmd);
}

std::string	Command::getPrefix() const
{
	return _prefix;
}

std::vector<std::string> Command::getArgs() const
{
	return _args;
}

std::string	Command::getArg(size_t i) const
{
	if (i >= _args.size())
		return "";
	return _args[i];
}

size_t	Command::argNumber() const
{
	return _args.size();
}

std::string Command::getStr() const
{
	std::string	ret = getPrefix();
	if (!ret.empty() && !getCmd().empty())
		ret += ' ';
	if (!getCmd().empty())
		ret += getCmd();
	size_t nb = argNumber();
	if (nb)
	{
		ret += ' ';
		for (size_t i = 0; i < nb; i++)
		{
			ret += getArg(i);
			if (i + 1 != nb)
				ret += ' ';
		}
	}
	return ret;
}

std::ostream & operator<<(std::ostream & out, const Command & cmd)
{
	out << cmd.getStr();
	return out;
}