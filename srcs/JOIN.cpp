#include "main.hpp"
#include <utility>

void	join(Server *srv, int &userfd, Command &cmd)
{
	std::cout << "JOIN func called" << std::endl;
	(void)userfd;
	if (cmd.getParamList().empty())
		std::cout << "Need more params" << std::endl;
	/* Like with PRIVMSG will need to split the targets to send it to each individual targets */
	/* Need verification if the target starts with '#', shouldn't happen with irssi but someone might try it with nc */
	if (srv->getChannel(cmd.getParam(0)) == NULL)
		srv->getChannelMap()->insert(std::make_pair(cmd.getParam(0), new Channel(cmd.getParam(0))));
	std::cout << srv->getChannel(cmd.getParam(0))->getName() << std::endl;
	std::cout << srv->getChannel(cmd.getParam(0))->join(srv->getUser(userfd), cmd.getLastParam()) << std::endl;
}
