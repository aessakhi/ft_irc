/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aessakhi <aessakhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 19:26:20 by aessakhi          #+#    #+#             */
/*   Updated: 2023/02/16 15:33:28 by aessakhi         ###   ########.fr       */
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
	std::string	_nickname;
	std::string	_username;
	std::string	_hostname;
	bool		_pwd;
	bool		_authenticated;
public:
	User(int fd);

	~User();

	bool	getAuth();

	bool	getPwd();

	void	setAuth(bool state);

	void	setPwd(bool state);

};

#endif
