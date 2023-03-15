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

		std::string	_oldnick;
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

		/* Variable that holds creation time */
		time_t _creatime;

		/* Variable that holds last active time */
		time_t _idletime;

		std::string	_userbuf;

	public:

		User(int fd, std::string hostname);

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

		/* Returns old nickname, will be used by NICK in case of nickame change*/
		std::string	getOldNick() const;

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

		std::string	&getUserbuffer();

		/* Return creatime of the user */
		time_t getCreatime() const;

		time_t getIdletime() const;

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

		/* Sets user's old nickname */
		void	setOldnick(std::string oldnick);

		/* Sets user's invisible state */
		void	setInvisible(bool state);

		/* Sets user's operator state */
		void	setOperator(bool state);

		/* Sets user's wallops state */
		void	setWallops(bool state);

		/* Sets user's away state */
		void	setAway(bool state, std::string msg);

		/* Sets user's last action time */
		void	updateIdletime();

	/* Returns the user's mode string */
	const std::string	getModeString() const;

	/* Returns the fd used to communicate with user */
	int	getFd() const;

};

#endif
