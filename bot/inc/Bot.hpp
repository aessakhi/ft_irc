#ifndef BOT_HPP
#define BOT_HPP

#include "main.hpp"

class SocketException : public std::exception
{
	public:
	
		virtual const char *what() const throw()
		{
			return ("Fatal error on socket setup");
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

class Bot
{
	public:
		Bot();
		~Bot();
		Bot(Bot const & src);
		Bot & operator=(Bot const & src);

	/* Sends str with a \\r\\n. */
		void	send_message(std::string str) const;
	/* Sets up _fd. */
		void	socket_setup(char * address, char * port);
	/* Sends PASS, NICK and USER messages */
		void	authentication(std::string password, std::string nickname, std::string username, std::string realname) const;
	/* Calls recv and appends what is read to buffer.
	Returns true if something was read, false otherwise. */
		bool	receive_message();
	/* Splits buffer into prefix, command, and arguments.
	Returns true if something was read, false otherwise. */
		bool	parse_buffer();
	/* Prints buffer as is. */
		void	print_buffer();
	/* Prints parsed elements of the buffer. */
		void	print_parsed_buffer();

		int		_fd;

		std::string		bot_buffer;

	// Command storage
		std::string					prefix;
		std::string 				command;
		std::vector<std::string>	arguments;
};

#endif