/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exceptions.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldesnoye <ldesnoye@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/16 18:20:39 by ldesnoye          #+#    #+#             */
/*   Updated: 2023/02/21 10:49:55 by ldesnoye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXCEPTIONS_HPP
# define EXCEPTIONS_HPP

#include "main.hpp"

class SocketCreationException : public std::exception
{
	public:
	
		virtual const char *what() const throw()
		{
			return ("Fatal error on socket creation");
		}
};

class EpollCreationException : public std::exception
{
	public:
	
		virtual const char *what() const throw()
		{
			return ("Fatal error on epoll creation");
		}
};

class EpollCtlException : public std::exception
{
	public:
	
		virtual const char *what() const throw()
		{
			return ("Fatal error on a call to epoll_ctl()");
		}
};

class EpollWaitException : public std::exception
{
	public:
	
		virtual const char *what() const throw()
		{
			return ("Fatal error on a call to epoll_wait()");
		}
};

class NewUserAcceptException : public std::exception
{
	public:
	
		virtual const char *what() const throw()
		{
			return ("Fatal error on a call to accept()");
		}
};

class FdCloseException : public std::exception
{
	public:
	
		virtual const char *what() const throw()
		{
			return ("Fatal error on a call to close()");
		}
};

class NotAChannelOperatorException : public std::exception
{
	public:
		
		virtual const char *what() const throw()
		{
			return ("User isnt a channel operator");
		}
};

#endif