#include "Bot.hpp"

Bot::Bot() : _fd(-1), _epollfd(-1), _suffix(1), _has_quit(false)
{}

Bot::Bot(std::string nickname, std::string username, std::string realname) : _fd(-1), _epollfd(-1), _suffix(1), _has_quit(false), _nickname(nickname), _username(username), _realname(realname)
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

	_has_quit = src._has_quit;

	_nickname = src._nickname;
	_username = src._username;
	_hostname = src._hostname;
	_realname = src._realname;

	_recv_buffer = src._recv_buffer;
	_send_buffer = src._send_buffer;

	_prefix = src._prefix;
	_command = src._command;
	_arguments = src._arguments;

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
	add_pass(password);
	add_nick(new_nick);
	add_user();
	while (true)
	{
		while (!_send_buffer.empty() || _command.empty())
		{
			epoll_loop();
		}
		// ERR_PASSWDMISMATCH
		if (!_command.compare("464"))
		{
			throw IncorrectPasswordException();
		}
		// ERR_NICKNAMEINUSE
		if (!_command.compare("433"))
		{
			new_nick = next_nickname();
			clear_command();
		}
		// RPL_WELCOME
		if (!_command.compare("001"))
		{
			_nickname = new_nick;
			clear_command();
			break;
		}
		add_nick(new_nick);
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

bool Bot::parse_buffer()
{
	printRecv(_recv_buffer, _recv_buffer.size());

	if (_recv_buffer.empty())
		return false;

	// Grab up to the first "\\r\\n"

	std::string buffer;
	size_t	end = _recv_buffer.find("\r\n");
	size_t	sep_size = 2;
	if (end == std::string::npos)
	{
		end = _recv_buffer.find_first_of("\r\n");
		sep_size = 1;
	}
	if (end == std::string::npos)
	{
		return false;
	}
	buffer = _recv_buffer.substr(0, end + 1);
	_recv_buffer.erase(0, end + 1 + sep_size);

	size_t	start = 0;
	end = 0;

	// Prefix

	if (buffer[start] == ':')
	{
		start = 1;
		end = buffer.find(' ', start);
		_prefix = buffer.substr(start, end - start);
	}

	// Command

	start = buffer.find_first_not_of(' ', end);
	end = buffer.find(' ', start);
	if (start == std::string::npos)
		_command = "";
	else
		_command = buffer.substr(start, end - start);

	// Arguments parsing

	std::vector<std::string> args;
	start = buffer.find_first_not_of(' ', end);
	end = buffer.find(' ', start);
	while (start != std::string::npos)
	{
		if (buffer[start] == ':')
		{
			end = std::string::npos;
			start++;
		}
		args.push_back(buffer.substr(start, end - start));
		start = buffer.find_first_not_of(' ', end);
		end = buffer.find(' ', start);
	}
	args[args.size() - 1] = no_crlf(args[args.size() - 1]);
	_arguments = args;
	return true;
}

void Bot::print_parsed_buffer()
{
	std::cout << "Prefix: \'" << _prefix << "\'" << std::endl;
	std::cout << "Command: \'" << _command << "\'" << std::endl;
	std::cout << "Arguments: ";
	for (size_t i = 0; i < _arguments.size();)
	{
		std::cout << "\'" << _arguments[i] << "\'";
		i++;
		if (i == _arguments.size())
			std::cout << std::endl;
		else
			std::cout << ", ";
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
			if (parse_buffer())
			{
				print_parsed_buffer();
			}
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

void Bot::clear_command()
{
	_prefix = "";
	_command = "";
	_arguments.clear();
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