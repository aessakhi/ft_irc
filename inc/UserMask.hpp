#ifndef USERMASK_HPP
# define USERMASK_HPP

#include "main.hpp"

class UserMask
{
	private:

	/* ----------NAMES---------- */

		/* User's nickname */
		std::string	_nickname;

		/* User's username */
		std::string	_username;

		/* User's hostname */
		std::string	_hostname;

	public:

		UserMask();

		UserMask(std::string nickname, std::string username, std::string hostname);

		UserMask(User * user);

		UserMask(User user);

		UserMask(UserMask const & src);

		void	initFromMask(std::string mask);

		void	initFromNick(std::string nick);

		UserMask & operator=(UserMask const & src);

		~UserMask();

		/* Returns "nickname!username\@hostname" */
		const std::string getMask() const;

};

#endif
