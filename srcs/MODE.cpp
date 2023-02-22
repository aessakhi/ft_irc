#include "main.hpp"

/*
User modes :
a = away
i = invisible
o = server oper
w = wallops
*/
static void	usermode(Server *srv, int &userfd, Command &cmd)
{
	std::string target = cmd.getParam(0);
	User * user = srv->getUser(userfd);

	// Error if target is a nick that doesnt exist
	if (srv->getUserbyNickname(target) == NULL)
	{
		srv->sendReply(userfd, ERR_NOSUCHNICK(user->getNickname(), target));
		return;
	}

	// Error if target is a nick that exists but differs from the user
	if (target != user->getNickname())
	{
		srv->sendReply(userfd, ERR_USERSDONTMATCH(user->getNickname()));
		return;
	}

	// If no modestring is given, reply with the modes of the user.
	if (cmd.paramNumber() == 1)
	{
		std::string modes("+");
		if (user->isOperator())
			modes += 'o';
		if (user->isInvisible())
			modes += 'i';
		if (user->isWallops())
			modes += 'w';
		if (user->isAway())
			modes += 'a';
		srv->sendReply(userfd, RPL_UMODEIS(user->getNickname(), modes));
		return;
	}

	// -----If modestring is given, parse modestring and change modes-----

	std::string modestring = cmd.getParam(1);
	// If first char isnt + or -, handle error however we want
	if (modestring[0] != '+' && modestring[0] != '-')
	{
		srv->sendReply(userfd, ERR_UMODEUNKNOWNFLAG(user->getNickname()));
		return;
	}

	bool add = (modestring[0] == '+');
	std::map<char, bool> stock;
	for (size_t i = 0; i < modestring.size(); i++)
	{
		char current = modestring[i];
		if (current == '+' || current == '-')
		{
			add = (current == '+');
			if (i + 1 == modestring.size())
				srv->sendReply(userfd, ERR_UMODEUNKNOWNFLAG(user->getNickname()));
			continue;
		}

		// Skip away mode
		if (current == 'a')
			continue;

		// Remove operator mode if user is one
		if (current == 'o')
		{
			if (!add && user->isOperator())
			{
				user->setOperator(add);
				stock['o'] = add;
			}
			continue;
		}

		// Change wallops mode
		if (current == 'w')
		{
			user->setWallops(add);
			stock['w'] = add;
			continue;
		}

		// Change invisible mode
		if (current == 'i')
		{
			user->setInvisible(add);
			stock['i'] = add;
			continue;
		}
		srv->sendReply(userfd, ERR_UMODEUNKNOWNFLAG(user->getNickname()));
	}

	// -----Build the string that should be sent as a reply-----
	std::string added("+");
	std::string removed("-");

	std::map<char, bool>::const_iterator found;

	found = stock.find('o');
	if (found != stock.end())
	{
		if (stock['o'])
			added.push_back('o');
		else
			removed.push_back('o');
	}

	found = stock.find('i');
	if (found != stock.end())
	{
		if (stock['i'])
			added.push_back('i');
		else
			removed.push_back('i');
	}

	found = stock.find('w');
	if (found != stock.end())
	{
		if (stock['w'])
			added.push_back('w');
		else
			removed.push_back('w');
	}

	std::string reply_str;
	if (added.size() > 1)
		reply_str.append(added);
	if (added.size() > 1 && removed.size() > 1)
		reply_str.push_back(' ');
	if (removed.size() > 1)
		reply_str.append(removed);

	srv->sendReply(userfd, ":" + srv->getName() + " MODE " + user->getNickname() + " :" + reply_str);
}

/*
Channel modes :
b = ban
e = ban-except
l = limit
i = invite only
I = invite-except
k = key
m = moderated
s = secret
t = protected topic
n = no external messages
*/
static void	channelmode(Server *srv, int &userfd, Command &cmd)
{
	(void)srv;
	(void)userfd;
	(void)cmd;
}

void	mode(Server *srv, int &userfd, Command &cmd)
{
	// Number of args check
	if (cmd.paramNumber() == 0)
	{
		srv->sendReply(userfd, ERR_NEEDMOREPARAMS(srv->getUser(userfd)->getNickname(), cmd.getCmd()));
		return;
	}

	// Command for channels
	if (cmd.getParam(0)[0] == '#')
	{
		return channelmode(srv, userfd, cmd);
	}
	// Command for users
	else
	{
		return usermode(srv, userfd, cmd);
	}
}
