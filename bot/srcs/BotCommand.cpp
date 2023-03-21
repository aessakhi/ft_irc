#include "BotCommand.hpp"

BotCommand::BotCommand()
{}

BotCommand::BotCommand(std::string from_nick, std::string original_target, std::string command, std::string raw_args) : from_nick(from_nick), original_target(original_target), command(command), raw_args(raw_args)
{}

BotCommand::~BotCommand()
{}

BotCommand::BotCommand(BotCommand const & src)
{ *this = src; }

BotCommand & BotCommand::operator=(BotCommand const & src)
{
	from_nick = src.from_nick;
	original_target = src.original_target;
	command = src.command;
	raw_args = src.raw_args;
	args = src.args;

	return *this;
}