#ifndef BOTMODES_HPP
# define BOTMODES_HPP

class BotModes
{
	public :
		BotModes();
		~BotModes();
		BotModes(BotModes const & src);
		BotModes & operator=(BotModes const & src);

		bool	parrot;
		bool	greet;
};

#endif