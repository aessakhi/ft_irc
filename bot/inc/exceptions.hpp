#ifndef EXCEPTIONS_HPP
# define EXCEPTIONS_HPP

#include "main.hpp"

class AddressException : public std::exception
{
	public:
	
		virtual const char *what() const throw()
		{
			return ("Invalid address / Address not supported");
		}
};

class SocketSetupException : public std::exception
{
	public:
	
		virtual const char *what() const throw()
		{
			return ("Fatal error on socket setup");
		}
};

class EpollSetupException : public std::exception
{
	public:
	
		virtual const char *what() const throw()
		{
			return ("Fatal error on epoll setup");
		}
};

class EpollCtlException : public std::exception
{
	public:
	
		virtual const char *what() const throw()
		{
			return ("Fatal error on epoll_ctl()");
		}
};

class EpollWaitException : public std::exception
{
	public:
	
		virtual const char *what() const throw()
		{
			return ("Fatal error on epoll_wait()");
		}
};

class FdCloseException : public std::exception
{
	public:
	
		virtual const char *what() const throw()
		{
			return ("Fatal error on close()");
		}
};

class ConnectionException : public std::exception
{
	public:
	
		virtual const char *what() const throw()
		{
			return ("Fatal error on connect()");
		}
};

class SendException : public std::exception
{
	public:
	
		virtual const char *what() const throw()
		{
			return ("Fatal error on send()");
		}
};

class RecvException : public std::exception
{
	public:
	
		virtual const char *what() const throw()
		{
			return ("Fatal error on recv()");
		}
};

class RDHUPException : public std::exception
{
	public:
	
		virtual const char *what() const throw()
		{
			return ("Distant connection was closed");
		}
};

class IncorrectPasswordException : public std::exception
{
	public:
	
		virtual const char *what() const throw()
		{
			return ("Wrong password supplied");
		}
};

class NoAvailableNicknameException : public std::exception
{
	public:
	
		virtual const char *what() const throw()
		{
			return ("No nickname available");
		}
};

#endif