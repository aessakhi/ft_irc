#include "BotCommand.hpp"

BotCommand::BotCommand()
{}

BotCommand::BotCommand(std::string from_mask, std::string original_target, std::string command, std::string raw_args) : from_mask(from_mask), original_target(original_target), command(command), raw_args(raw_args)
{
	args = split(raw_args, " ");
	parse_mask();
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