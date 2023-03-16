#include "Bot.hpp"

Bot::Bot() : _fd(-1)
{}

Bot::Bot(std::string nickname, std::string username, std::string realname) : _fd(-1), _nickname(nickname), _username(username), _realname(realname)
{}

Bot::~Bot()
{
	if (_fd != -1)
	{
		send_message("QUIT :Bot is shutting off");
		close(_fd);
	}
}

Bot::Bot(Bot const & src) : _fd(src._fd)
{}

Bot & Bot::operator=(Bot const & src)
{
	_fd = src._fd;
	return *this;
}

void Bot::send_message(std::string str) const
{
	str += "\r\n";
	ssize_t sent = send(_fd, str.c_str(), str.size(), 0);
	if (sent == -1)
		throw SendException();
}

void Bot::socket_setup(char * address, char * port)
{
	struct sockaddr_in serv_addr;
	serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(port));
	memset(serv_addr.sin_zero, 0, sizeof serv_addr.sin_zero);
  
    if (inet_pton(AF_INET, address, &serv_addr.sin_addr) <= 0)
	{
        printError("Invalid address/ Address not supported");
		return;
    }

	_fd = socket(PF_INET, SOCK_STREAM, 0);

	if (_fd < 0)
		throw SocketException();

	int yes = 1;
	if (setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &yes, sizeof yes) == -1)
		throw SocketException();

	if (connect(_fd, (struct sockaddr*)&serv_addr, sizeof serv_addr))
		throw ConnectionException();
}

void Bot::authentication(std::string password) const
{
	std::string msg;
	msg += "PASS ";
	msg += password;
	msg += "\r\n";
	msg += "NICK ";
	msg += _nickname;
	msg += "\r\n";
	msg += "USER ";
	msg += _username;
	msg += " 0 * :";
	msg += _realname;

	send_message(msg);
}

bool Bot::receive_message()
{
	char buf[RECV_BUFFER_SIZE];
	ssize_t ret;

	ret = recv(_fd, buf, RECV_BUFFER_SIZE, 0);
	if (ret < 0)
		throw RecvException();
	if (ret <= 0)
		return false;
	bot_buffer.append(buf, ret);
	return (true);
}

bool Bot::parse_buffer()
{
	if (bot_buffer.empty())
		return false;

	// Grab up to the first "\\r\\n"

	std::string buffer;
	size_t	end = bot_buffer.find("\r\n");
	if (end == std::string::npos)
		end = bot_buffer.find_first_of("\r\n");
	if (end == std::string::npos)
		return false;
	buffer = bot_buffer.substr(0, end + 1);
	bot_buffer.erase(0, end + 1);

	size_t	start = 0;
	end = 0;

	// Prefix

	if (buffer[start] == ':')
	{
		start = 1;
		end = buffer.find(' ', start);
		prefix = buffer.substr(start, end - start);
	}

	// Command

	start = buffer.find_first_not_of(' ', end);
	end = buffer.find(' ', start);
	if (start == std::string::npos)
		command = "";
	else
		command = buffer.substr(start, end - start);

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
	arguments = args;
	return true;
}

void	Bot::print_buffer()
{
	std::cout << bot_buffer << std::endl;
}

void	Bot::print_parsed_buffer()
{
	std::cout << "Prefix: \'" << prefix << "\'" << std::endl;
	std::cout << "Command: \'" << command << "\'" << std::endl;
	std::cout << "Arguments: ";
	for (size_t i = 0; i < arguments.size();)
	{
		std::cout << "\'" << arguments[i] << "\'";
		i++;
		if (i == arguments.size())
			std::cout << std::endl;
		else
			std::cout << ", ";
	}
}

void	Bot::loop()
{
	if (receive_message())
	{
		parse_buffer();
		print_parsed_buffer();
	}
}