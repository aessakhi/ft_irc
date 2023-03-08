#include "main.hpp"

static bool _nick_error(Server *srv, int &userfd, Command &cmd)
{
	std::string target = cmd.getParam(0);
	User * user = srv->getUser(userfd);

	// Error if target is a nick that doesnt exist
	if (srv->getUserbyNickname(target) == NULL)
	{
		srv->sendReply(userfd, ERR_NOSUCHNICK(user->getNickname(), target));
		return true ;
	}

	// Error if target is a nick that exists but differs from the user
	if (target != user->getNickname())
	{
		srv->sendReply(userfd, ERR_USERSDONTMATCH(user->getNickname()));
		return true ;
	}

	return false ;
}

static std::list<Mode> _modestring_parsing(std::string modestring, Server *srv, int &userfd)
{
	User * user = srv->getUser(userfd);
	std::list<Mode> mode_list;
	bool add = (modestring[0] == '+');

	for (size_t i = 0; i < modestring.size(); i++)
	{
		char c = modestring[i];
		if (c == '+' || c == '-')
		{
			add = (c == '+');
			if (i + 1 == modestring.size())
				srv->sendReply(userfd, ERR_UMODEUNKNOWNFLAG(user->getNickname()));
			continue;
		}

		// Skip away mode
		if (c == 'a')
			continue;

		// Add o mode only if removing operator
		if (c == 'o')
		{
			if (!add && user->isOperator())
				mode_list.push_back(Mode(add, c));
			continue;
		}

		// Add w or i mode
		if (c == 'w' || c == 'i')
		{
			mode_list.push_back(Mode(add, c));
			continue;
		}

		// If letter is unrecognized, send error
		srv->sendReply(userfd, ERR_UMODEUNKNOWNFLAG(user->getNickname()));
	}

	return mode_list;
}

static void _apply_mode_changes(std::list<Mode> mode_list, Server *srv, int &userfd)
{
	User * user = srv->getUser(userfd);

	std::list<Mode>::iterator it = mode_list.begin();
	std::list<Mode>::iterator ite = mode_list.end();

	for (; it != ite; it++)
	{
		switch (it->getMode())
		{
			case 'o':
			{
				user->setOperator(it->getAdd());
				break;
			}
			
			case 'w':
			{
				user->setWallops(it->getAdd());
				break;
			}
			
			case 'i':
			{
				user->setInvisible(it->getAdd());
				break;
			}
			
			default:
			{
				break;
			}
		}
	}
}

static std::string _new_modes(std::list<Mode> mode_list)
{
	std::list<Mode>::const_iterator it = mode_list.begin();
	std::list<Mode>::const_iterator ite = mode_list.end();

	std::string ret;
	bool last_add = !(it->getAdd());

	for (; it != ite; it++)
	{
		if (it->getAdd() && !last_add)
			ret += '+';
		else if (!it->getAdd() && last_add)
			ret += '-';
		last_add = it->getAdd();
		ret += it->getMode();
	}

	return ret ;
}

void	usermode(Server *srv, int &userfd, Command &cmd)
{

	if (_nick_error(srv, userfd, cmd))
		return;

	// If no modestring is given, reply with the modes of the user.
	User * user = srv->getUser(userfd);
	if (cmd.paramNumber() == 1)
	{
		srv->sendReply(userfd, RPL_UMODEIS(user->getNickname(), user->getModeString()));
		return;
	}

	std::string modestring = cmd.getParam(1);

	// If first char isnt + or -, handle error however we want
	if (modestring[0] != '+' && modestring[0] != '-')
	{
		srv->sendReply(userfd, ERR_UMODEUNKNOWNFLAG(user->getNickname()));
		return;
	}

	std::list<Mode> mode_list = _modestring_parsing(modestring, srv, userfd);

	_apply_mode_changes(mode_list, srv, userfd);

	// Build the string that should be sent as a reply
	std::string new_modes = _new_modes(mode_list);
	srv->sendReply(userfd, ":" + user->getNickname() + " MODE " + user->getNickname() + " :" + new_modes);
}