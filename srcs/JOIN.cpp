#include "main.hpp"

void	join(Server *srv, int &userfd, Command &cmd)
{
	std::cout << "JOIN func called" << std::endl;
	(void)srv;
	(void)userfd;
	if (cmd.getParamList().empty())
		std::cout << "Need more params" << std::endl;
}
