#include "main.hpp"

//GO THROUGH ALL CHANNELS AND PART ALL IN THE CLASS METHOD
//Connecion to localhost established, but user not welcomed then still receives arg

void	quit(Server *srv, int &userfd, Command &cmd)
{
	(void)cmd;
	srv->_removeUserfromServer(userfd);
	//ERROR MESSAGE TO BE DONE
}