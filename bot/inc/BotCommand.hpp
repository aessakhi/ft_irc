#ifndef BOTCOMMAND_HPP
# define BOTCOMMAND_HPP

#include "main.hpp"

class BotCommand
{
	public:
		BotCommand();
		BotCommand(std::string from_mask, std::string original_target, std::string command, std::string raw_args);
		~BotCommand();
		BotCommand(BotCommand const & src);
		BotCommand & operator=(BotCommand const & src);

		std::string	from_mask;
		std::string	original_target;
		std::string	command;
		std::string	raw_args;
		std::vector<std::string>	args;

		void	parse_mask();

		std::string	from_nick;
		std::string	from_user;
		std::string	from_host;

		std::string	reply_target;
};

std::ostream & operator<<(std::ostream & out, const BotCommand & src);

#endif