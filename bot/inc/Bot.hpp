#ifndef BOT_HPP
#define BOT_HPP

#include "main.hpp"

class Bot
{
	private:
		Bot();

	public:
		Bot(std::string nickname, std::string username, std::string realname);
		~Bot();
		Bot(Bot const & src);
		Bot & operator=(Bot const & src);

	/* -------------------- AUTHENTICATION -------------------- */

		void	add_pass(std::string password);
		void	add_nick(std::string nick);
		void	add_user();

	/* Add str to _send_buffer with a \\r\\n. */
		void	add_to_send_buffer(std::string str);
	/* Send the first command in _send_buffer. */
		void	send_buffer();
	/* Sets up _fd. */
		void	socket_setup(char * address, char * port);
	/* Sets up _epoll_fd. */
		void	create_epoll();
	/* Sends PASS, NICK and USER messages. */
		void	authentication(std::string password);
	/* Returns next nickname */
		std::string	next_nickname();
	/* Calls recv and appends what is read to buffer. */
		void	receive_message();
	/* Splits buffer into prefix, command, and arguments.
	Returns true if something was split, false otherwise (empty buffer or incomplete message received). */
		bool	parse_buffer();
	/* Prints parsed elements of the buffer. */
		void	print_parsed_buffer();
	/* epoll_wait and read/reply if necessary. */
		void	epoll_loop();
	/* Clears _prefix, _command and _arguments. */
		void	clear_command();
	/* Adds quit message to _send_buffer, sets _has_quit to true. */
		void	add_quit_message();
	/* Close fds. */
		void	close_fds();

		int		_fd;
		int		_epollfd;

		int		_suffix;

	/* True when QUIT message has been added to the buffer.
	After that, no more data should be added to _read_buffer or _send_buffer. */
		bool	_has_quit;

		std::string		_nickname;
		std::string		_username;
		std::string		_hostname;
		std::string		_realname;

		std::string		_recv_buffer;
		std::string		_send_buffer;

	// Command storage

		std::string					_prefix;
		std::string 				_command;
		std::vector<std::string>	_arguments;
};

#endif