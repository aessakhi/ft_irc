#include "BotCommand.hpp"

BotCommand::BotCommand()
{}

BotCommand::BotCommand(std::string from_mask, std::string original_target, std::string command, std::string raw_args) : from_mask(from_mask), original_target(original_target), command(command), raw_args(raw_args)
{
	args = split(raw_args, " ");
	parse_mask();
	if (!original_target.empty() && original_target[0] == '#')
		reply_target = original_target;
	else
		reply_target = from_nick;
}

BotCommand::~BotCommand()
{}

BotCommand::BotCommand(BotCommand const & src)
{ *this = src; }

BotCommand & BotCommand::operator=(BotCommand const & src)
{
	from_mask = src.from_mask;
	original_target = src.original_target;
	command = src.command;
	raw_args = src.raw_args;
	args = src.args;

	from_nick = src.from_nick;
	from_user = src.from_user;
	from_host = src.from_host;

	reply_target = src.reply_target;

	return *this;
}

void BotCommand::parse_mask()
{
	size_t	start = 0;
	size_t	end = from_mask.find('!', start);
	
	if (end == std::string::npos)
		return ;

	from_nick = from_mask.substr(start, end);

	start = end + 1;
	end = from_mask.find('@', start);

	from_user = from_mask.substr(start, end);

	start = end + 1;

	from_host = from_mask.substr(start);
}

std::ostream & operator<<(std::ostream & out, const BotCommand & src)
{
	out << "from_mask : " << src.from_mask << ", target = " << src.original_target << ", reply_target = " << src.reply_target << std::endl;
	out << "nick = " << src.from_nick << ", user = " << src.from_user << ", host = " << src.from_host << std::endl;
	out << "cmd = " << src.command << std::endl;
	out << "args = ";
	for (size_t i = 0; i < src.args.size(); i++)
	{
		out << src.args[i];
		if (i + 1 < src.args.size())
			out << ", ";
	}
	out << std::endl;
	return out;
}