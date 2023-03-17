#include "main.hpp"

static void	cpt_users(Server *srv, std::string &num_loc_users, std::string &num_inv_users)
{
	size_t cpt_loc = 0;
	size_t cpt_inv = 0;
	std::map<int, User *> tmp_map(srv->getUserMap());

	for (std::map<int, User *>::iterator it = tmp_map.begin(); it != tmp_map.end(); it++)
	{
		cpt_loc++;
		if (it->second->isInvisible())
			cpt_inv++;
	}
	
	std::stringstream loc;
	loc << cpt_loc;
	num_loc_users = loc.str();

	std::stringstream inv;
	inv << cpt_inv;
	num_inv_users = inv.str();
}

void	lusers(Server *srv, int &userfd, Command &cmd)
{
	User * user = srv->getUser(userfd);
	std::string num_loc_users, num_inv_users;

	cpt_users(srv, num_loc_users, num_inv_users);
	srv->sendReply(userfd, RPL_LUSERCLIENT(user->getNickname(), num_loc_users, num_inv_users));
	(void)cmd;
}