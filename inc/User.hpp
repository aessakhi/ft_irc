#ifndef USER_HPP
# define USER_HPP

#include "main.hpp"

class User
{
	private:
		User();

		/* The fd to use for communication with the client */
		int			_fd;

	/* ----------AUTHENTIFICATION BOOLEANS---------- */

		/* If true, user field has been set */
		bool		_user;

		/* If true, correct password has been given */
		bool		_pwd;

		/* If true, nick field has been set */
		bool		_nick;

		/*  */
		bool		_authenticated;

	/* ----------NAMES---------- */

		/* User's nickname */
		std::string	_nickname;

		/* User's username */
		std::string	_username;

		/* User's hostname */
		std::string	_hostname;

		/* User's real name */
		std::string	_realname;

	/* ----------MODES---------- */

		/* If true, the user is invisible */
		bool	_is_invisible;

		/* If true, the user is a server operator */
		bool	_is_operator;

		/* If true, the user is WALLOPS */
		bool	_is_wallops;

		/* If true, the user is away */
		bool	_is_away;

		/* If user is away, this is the away message */
		std::string _away_msg;

	public:

		User(int fd);

		~User();

	/* ----------ACCESSORS---------- */

		/*  */
		bool	getAuth();

		/* Returns if the user has given the correct password */
		bool	getPwd();

		/* Returns if the user has set his nickname */
		bool	getNick();

		/* Returns if the user has set his username */
		bool	getUser();

		/* Returns nickname */
		std::string	getNickname() const;

		/* Returns username */
		std::string	getUsername() const;

		/* Returns nickname */
		std::string	getHostname() const;

		/* Returns real name */
		std::string	getRealname() const;

		/* Returns if user is invisible */
		bool	isInvisible() const;

		/* Returns if user is a server operator */
		bool	isOperator() const;

		/* Returns if user is wallops */
		bool	isWallops() const;

		/* Returns if user is away */
		bool	isAway() const;

		/* Returns the user's away message */
		const std::string	getAwayMessage() const;

		/* Returns true if _pwd, _nick, _user are true */
		bool	checkAuth();

		/* Returns "nickname!username\@hostname" */
		const std::string getMask() const;


	/* ----------SETTERS---------- */

		/* Sets _authenticated value */
		void	setAuth(bool state);

		/* Sets _pwd value */
		void	setPwd(bool state);

		/* Sets _nick value */
		void	setNick(bool state);

		/* Sets _user value */
		void	setUser(bool state);

		/* Sets user's nickname */
		void	setNickname(std::string nickname);

		/* Sets user's username */
		void	setUsername(std::string username);

		/* Sets user's realname */
		void	setRealname(std::string realname);

		/* Sets user's invisible state */
		void	setInvisible(bool state);

		/* Sets user's operator state */
		void	setOperator(bool state);

		/* Sets user's wallops state */
		void	setWallops(bool state);

		/* Sets user's away state */
		void	setAway(bool state);

	/* Returns the user's mode string */
	const std::string	getModeString() const;

};

#endif
