#ifndef BOTCOMMAND_HPP
# define BOTCOMMAND_HPP

#include "main.hpp"

class BotCommand
{
	public:
		BotCommand();
		BotCommand(std::string from_nick, std::string original_target, std::string command, std::string raw_args);
		~BotCommand();
		BotCommand(BotCommand const & src);
		BotCommand & operator=(BotCommand const & src);

		std::string	from_nick;
		std::string	original_target;
		std::string	command;
		std::string	raw_args;
		std::vector<std::string> args;
};

#endif