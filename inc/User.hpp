/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aessakhi <aessakhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 19:26:20 by aessakhi          #+#    #+#             */
/*   Updated: 2023/02/21 15:15:03 by aessakhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
# define USER_HPP

#include "main.hpp"

class User
{
private:
	User();

	int			_fd;

	/* These 3 bool will be used to check the authentification of the user */
	/* For instance, if the password is empty, we'll still accept the connection of the user but won't send the reply msg until these 3 bools are set to true */
	bool		_user;
	bool		_pwd;
	bool		_nick;

	bool		_authenticated;

	std::string	_nickname;

	std::string	_username;
	std::string	_hostname;
	std::string	_realname;

public:
	User(int fd);

	~User();

	bool	getAuth();

	bool	getPwd();
	bool	getNick();
	bool	getUser();

	std::string	getNickname() const;


	void	setAuth(bool state);

	void	setPwd(bool state);
	void	setNick(bool state);
	void	setUser(bool state);

	bool	checkAuth();

	void	setNickname(std::string nickname);
	void	setUsername(std::string username);
	void	setRealname(std::string realname);

};

#endif
