/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aessakhi <aessakhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 19:42:29 by aessakhi          #+#    #+#             */
/*   Updated: 2023/02/13 14:59:52 by aessakhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"

User::User(int fd): _fd(fd), _authenticated(0)
{}

User::~User()
{}

bool	User::getAuth()
{
	return (this->_authenticated);
}

void	User::setAuth(bool state)
{
	this->_authenticated = state;
}
