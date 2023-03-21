#include "Bot.hpp"

Bot::Bot() : _fd(-1), _epollfd(-1), _suffix(1), _registered(false), _botchar('>'), _has_quit(false)
{}

Bot::Bot(std::string nickname, std::string username, std::string realname, char botchar) : _fd(-1), _epollfd(-1), _suffix(1), _registered(false), _botchar(botchar), _has_quit(false), _nickname(nickname), _username(username), _realname(realname)
{}

Bot::~Bot()
{
	close_fds();
}

Bot::Bot(Bot const & src)
{
	*this = src;
}

Bot & Bot::operator=(Bot const & src)
{
	_fd = src._fd;
	_epollfd = src._epollfd;

	_suffix = src._suffix;

	_registered = src._registered;

	_botchar = src._botchar;

	_has_quit = src._has_quit;

	_nickname = src._nickname;
	_username = src._username;
	_hostname = src._hostname;
	_realname = src._realname;

	_recv_buffer = src._recv_buffer;
	_send_buffer = src._send_buffer;

	_cmd_buffer = src._cmd_buffer;

	return *this;
}

void Bot::add_to_send_buffer(std::string str)
{
	if (_has_quit)
		return;
	str += "\r\n";
	_send_buffer += str;
}

void Bot::send_buffer()
{
	ssize_t ret;
	printReply(no_crlf(_send_buffer));
	ret = send(_fd, _send_buffer.c_str(), _send_buffer.size(), 0);
	if (ret == -1)
	{
		throw SendException();
	}
	_send_buffer.erase(0, ret);
}

void Bot::socket_setup(char * address, char * port)
{
	struct sockaddr_in serv_addr;
	serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(port));
	memset(serv_addr.sin_zero, 0, sizeof serv_addr.sin_zero);
  
    if (inet_pton(AF_INET, address, &serv_addr.sin_addr) <= 0)
	{
		throw AddressException();
	}
	
	_fd = socket(PF_INET, SOCK_STREAM, 0);

	if (_fd < 0)
	{
		throw SocketSetupException();
	}

	int yes = 1;
	if (setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &yes, sizeof yes) == -1)
	{
		if (close(_fd) == -1)
		{
			throw FdCloseException();
		}
		_fd = -1;
		throw SocketSetupException();
	}

	if (connect(_fd, (struct sockaddr*)&serv_addr, sizeof serv_addr))
	{
		if (close(_fd) == -1)
		{
			throw FdCloseException();
		}
		_fd = -1;
		throw ConnectionException();
	}
}

void Bot::create_epoll()
{
	struct epoll_event	ev;

	if ((_epollfd = epoll_create1(0)) == -1)
	{
		throw EpollSetupException();
	}
	
	memset(&ev, 0, sizeof(struct epoll_event));
	ev.events = EPOLLIN | EPOLLOUT | EPOLLRDHUP;
	ev.data.fd = _fd;
	
	if (epoll_ctl(_epollfd, EPOLL_CTL_ADD, _fd, &ev) == -1)
	{
		if (close(_epollfd) == -1)
		{
			throw FdCloseException();
		}
		_epollfd = -1;
		throw EpollCtlException();
	}
}

void Bot::authentication(std::string password)
{
	std::string new_nick = _nickname;

	std::vector<Command>::const_iterator it;
	std::vector<Command>::const_iterator ite;

	add_pass(password);
	add_nick(new_nick);
	add_user();

	while (!_registered)
	{
		while (!_send_buffer.empty())
		{
			epoll_loop();
		}
		while (_recv_buffer.empty() && _cmd_buffer.empty())
		{
			epoll_loop();
		}
		it = _cmd_buffer.begin();
		ite = _cmd_buffer.end();
		for (; it != ite; it++)
		{
			// ERR_PASSWDMISMATCH
			if (!it->getCmd().compare("464"))
			{
				throw IncorrectPasswordException();
			}
			// ERR_NICKNAMEINUSE
			if (!it->getCmd().compare("433"))
			{
				continue;
			}
			// RPL_WELCOME
			if (!it->getCmd().compare("001"))
			{
				_nickname = new_nick;
				_registered = true;
				break;
			}
		}
		if (!_registered)
		{
			new_nick = next_nickname();
			add_nick(new_nick);
		}
		else
		{
			do
			{
				_cmd_buffer.erase(_cmd_buffer.begin());
			} while (_cmd_buffer.size() && _cmd_buffer[0].getCmd().compare("001")) ;
		}
	}
}

std::string Bot::next_nickname()
{
	std::string suffix;
	std::stringstream out;
	out << _suffix;
	_suffix++;
	suffix = "_" + out.str();

	if (_nickname.size() + suffix.size() > 9)
		throw NoAvailableNicknameException();
	
	return (_nickname + suffix);
}

void Bot::add_pass(std::string password)
{
	std::string msg;
	msg += "PASS ";
	msg += password;

	add_to_send_buffer(msg);
}

void Bot::add_nick(std::string nick)
{
	std::string msg;
	msg += "NICK ";
	msg += nick;

	add_to_send_buffer(msg);
}

void Bot::add_user()
{
	std::string msg;
	msg += "USER ";
	msg += _username;
	msg += " 0 * :";
	msg += _realname;

	add_to_send_buffer(msg);
}

void Bot::receive_message()
{
	if (_has_quit)
		return;

	char buf[RECV_BUFFER_SIZE];
	ssize_t ret;

	ret = recv(_fd, buf, RECV_BUFFER_SIZE, 0);
	if (ret < 0)
		throw RecvException();
	_recv_buffer.append(buf, ret);
}

Command Bot::parse_command(std::string command_str) const
{
	size_t	start = 0;
	size_t	end = 0;

	std::string	prefix;
	std::string	command;
	std::vector<std::string> args;

	// Prefix
	if (command_str[start] == ':')
	{
		start = 1;
		end = command_str.find(' ', start);
		prefix = command_str.substr(start, end - start);
	}

	// Command
	start = command_str.find_first_not_of(' ', end);
	end = command_str.find(' ', start);
	if (start == std::string::npos)
		command = "";
	else
		command = command_str.substr(start, end - start);

	// Arguments parsing
	start = command_str.find_first_not_of(' ', end);
	end = command_str.find(' ', start);
	while (start != std::string::npos)
	{
		if (command_str[start] == ':')
		{
			end = std::string::npos;
			start++;
		}
		args.push_back(command_str.substr(start, end - start));
		start = command_str.find_first_not_of(' ', end);
		end = command_str.find(' ', start);
	}
	args[args.size() - 1] = no_crlf(args[args.size() - 1]);

	return Command(command, args, prefix);
}

void Bot::parse_buffer()
{
	if (_recv_buffer.empty())
		return ;

	printRecv(_recv_buffer, _recv_buffer.size());

	std::string buffer;
	size_t	end = _recv_buffer.find("\r\n");
	size_t	sep_size = 2;
	while (end != std::string::npos)
	{
		if (end == std::string::npos)
		{
			end = _recv_buffer.find_first_of("\r\n");
			sep_size = 1;
		}
		if (end == std::string::npos)
		{
			return ;
		}
		buffer = _recv_buffer.substr(0, end + 1);
		_recv_buffer.erase(0, end + 1 + sep_size);

		_cmd_buffer.push_back(parse_command(buffer));

		end = _recv_buffer.find("\r\n");
		sep_size = 2;
	}
}

void Bot::epoll_loop()
{
	struct	epoll_event ep_events[EPOLL_EVENTS_SIZE];
	int nfds;

	nfds = epoll_wait(_epollfd, ep_events, EPOLL_EVENTS_SIZE, 0);
	if (nfds == -1)
	{
		throw EpollWaitException();
	}
	for (int i = 0; i < nfds; i++)
	{
		if (ep_events[i].events & EPOLLRDHUP)
		{
			throw RDHUPException();
		}
		if (ep_events[i].events & EPOLLIN)
		{
			receive_message();
			parse_buffer();
			if (_registered && !_cmd_buffer.empty())
				handle_commands();
		}
		if (ep_events[i].events & EPOLLOUT)
		{
			if (!_send_buffer.empty())
			{
				send_buffer();
			}
		}
	}
}

void Bot::add_quit_message()
{
	add_to_send_buffer("QUIT :Bot is shutting off");
	_has_quit = true;
}

void Bot::close_fds()
{
	if (_epollfd != -1)
	{
		if (epoll_ctl(_epollfd, EPOLL_CTL_DEL, _fd, NULL) == -1)
		{
			throw EpollCtlException();
		}
		if (close(_epollfd) == -1)
		{
			throw FdCloseException();
		}
	}

	if (_fd != -1)
	{
		if (close(_fd) == -1)
		{
			throw FdCloseException();
		}
	}
}

void Bot::handle_commands()
{
	std::vector<Command>::const_iterator it = _cmd_buffer.begin();
	std::vector<Command>::const_iterator ite = _cmd_buffer.end();
	for (; it != ite; it++)
	{
		handle_command(*it);
	}
	_cmd_buffer.clear();
}

void Bot::handle_command(Command cmd)
{
	if (!cmd.getCmd().compare("PRIVMSG"))
	{
		privmsg(cmd);
	}
}

void Bot::privmsg(Command cmd)
{
	std::string	message = cmd.getArg(1);
	std::string bot_command;
	std::string bot_args;
	BotCommand	command;

	if (!message.empty() && message[0] == _botchar)
	{
		size_t found = message.find(' ');
		bot_command = message.substr(1, found - 1);
		found = message.find_first_not_of(' ', found);
		bot_args = message.substr(found);
		command = BotCommand(cmd.getPrefix(), cmd.getArg(0), bot_command, bot_args);
		exec_botcommand(command);
	}
}

void Bot::exec_botcommand(BotCommand bot_command)
{
	if (!bot_command.command.compare("raw_send") || !bot_command.command.compare("rs"))
	{
		add_to_send_buffer(bot_command.raw_args);
	}
}