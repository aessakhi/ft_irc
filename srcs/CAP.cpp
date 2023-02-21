#include "CommandList.hpp"

void	cap(Server *srv, int &userfd, Command &cmd)
{
	(void)srv;
	(void)userfd;
	(void)cmd;
	std::cout << "CAP ignored" << std::endl;
}
