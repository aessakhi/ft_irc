#ifndef BOT_HPP
#define BOT_HPP

#include "main.hpp"

class Bot
{
	private:
		Bot();

	public:
		Bot(std::string nickname, std::string username, std::string realname, char botchar);
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
	/* Creates Command from string. */
		Command parse_command(std::string command_str) const;
	/* Splits buffer into commands in _cmd_buffer. */
		void	parse_buffer();
	/* epoll_wait and read/reply if necessary. */
		void	epoll_loop();
	/* Adds quit message to _send_buffer, sets _has_quit to true. */
		void	add_quit_message();
	/* Close fds. */
		void	close_fds();
	/* Loop through _cmd_buffer.. */
		void	handle_commands();
	/* Calls bot function corresponding to received command. */
		void	handle_command(Command cmd);
	/* Tries to parse PRIVMSG received as bot commands. */
		void	privmsg(Command cmd);
	/* Executes bot commands. */
		void	exec_botcommand(BotCommand bot_command);

		int		_fd;
		int		_epollfd;

		int		_suffix;

		bool	_registered;

		/* Character for bot commands. */
		char	_botchar;

	/* True when QUIT message has been added to the buffer.
	After that, no more data should be added to _read_buffer or _send_buffer. */
		bool	_has_quit;

		std::string		_nickname;
		std::string		_username;
		std::string		_hostname;
		std::string		_realname;

		std::string		_recv_buffer;
		std::string		_send_buffer;

	/* Command storage */
		std::vector<Command>	_cmd_buffer;
};

#endif