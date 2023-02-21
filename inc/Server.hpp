#ifndef SERVER_HPP
# define SERVER_HPP

#include "main.hpp"
#include "User.hpp"
#include "Command.hpp"
#include "CommandList.hpp"
//Need to stock the user list -> pair<int, User*> , channel list -> Map easier? pair<std::string, Channel*> ?

class Server
{
private:

	Server();

	/* Port used for connecting to the server */
	std::string	_port;
	/* Password requested for connecting to the server */
	std::string	_pwd;

	/* Socket on which the server listens for new incoming connection requests */
	int	_listenfd;

	/* Single fd used by epoll to warn for incoming data on every fd */
	int	_epollfd;

	/* List of <fd, User *> pairs */
	std::map<int, User *>	_UserList;

	std::map<int, std::string>	_buffers;

	std::map<std::string, void(*)(Server *srv, int &userfd, Command &cmd)>	_cmdMap;

	void	_initCmdMap();
	/* Initializes _epollfd and adds _listenfd to the list of entries. */
	void	_create_epoll();

	/* Prepares _listenfd for accepting new connection requests. */
	void	_createsocket();

	/* Add new User * to the _UserList and the corresponding fd to the _epollfd. */
	void	_acceptnewUser();

	/* For now, calls recv() once and replies with codes if connection needs to be established */
	void	_receivemessage(struct epoll_event event);

	void	_execCmds(std::vector<Command> &cmds, int userfd);

	void	_removeUserfromServer(int fd);

	/* Easier send() */

public:

	Server(char *port, char *password);

	~Server();

	/* Functions */

	// "Initializes" the server. For now also contains the epoll_wait loop.
	void	init();

	void	sendReply(int fd, std::string s);
	/* Member accessors */

	// Returns password required for a new connection.
	std::string	getpwd() const;

	// Returns port used for connecting to the server.
	std::string	getport() const;

	// Returns User * instance corresponding to fd, NULL if no User * matches that fd.
	User	*getUser(int fd) const;

	User	*getUserbyNickname(const std::string nickname) const;

};

#endif
