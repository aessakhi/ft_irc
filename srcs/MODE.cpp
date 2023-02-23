#include "main.hpp"

/* Add c to the correct string. */
static void	clc(char c, std::map<char, bool> stock, std::string * added, std::string * removed)
{
	std::map<char, bool>::const_iterator found = stock.find(c);
	if (found != stock.end())
	{
		if (stock[c])
			added->push_back(c);
		else
			removed->push_back(c);
	}
}

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
		srv->sendReply(userfd, RPL_UMODEIS(user->getNickname(), user->getModeString()));
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

		// If letter is unrecognized, send error
		srv->sendReply(userfd, ERR_UMODEUNKNOWNFLAG(user->getNickname()));
	}

	// -----Build the string that should be sent as a reply-----
	std::string added("+");
	std::string removed("-");

	std::map<char, bool>::const_iterator found;

	clc('o', stock, &added, &removed);
	clc('i', stock, &added, &removed);
	clc('w', stock, &added, &removed);

	std::string reply_str;
	if (added.size() > 1)
		reply_str.append(added);
	if (added.size() > 1 && removed.size() > 1)
		reply_str.push_back(' ');
	if (removed.size() > 1)
		reply_str.append(removed);

	srv->sendReply(userfd, ":" + user->getNickname() + " MODE " + user->getNickname() + " :" + reply_str);
}

/*
Channel modes :
	-> requiring mask:
		b = ban
		e = ban-except
		I = invite-except
		o = op user
		v = voice user
	-> requiring other arg:
		l = limit
		k = key
	-> no arg:
		i = invite only
		m = moderated
		s = secret
		t = protected topic
		n = no external messages
*/
static void	channelmode(Server *srv, int &userfd, Command &cmd)
{

	std::string target = cmd.getParam(0);
	User * user = srv->getUser(userfd);
	Channel * channel = srv->getChannel(target);

	// Error if target is a channel that doesnt exist
	if (channel == NULL)
	{
		srv->sendReply(userfd, ERR_NOSUCHCHANNEL(user->getNickname(), target));
		return;
	}

	// If no modestring is given, reply with the modes of the channel.
	if (cmd.paramNumber() == 1)
	{
		//sendreply
		return;
	}

	// -----If modestring is given, check user privileges-----

	if (!channel->isOp(user))
	{
		srv->sendReply(userfd, ERR_CHANOPRIVSNEEDED(user->getNickname(), target));
		return;
	}

	// -----Parse modestring and change modes-----

	// std::string modestring = cmd.getParam(1);
	// // arguments points to the arglist after the modestring
	// std::vector<std::string>::const_iterator arguments = cmd.getParamList().begin();
	// arguments += 2;

	// // If first char isnt + or -, handle error however we want
	// if (modestring[0] != '+' && modestring[0] != '-')
	// {
	// 	srv->sendReply(userfd, ERR_UMODEUNKNOWNFLAG(user->getNickname()));
	// 	return;
	// }

	// bool add = (modestring[0] == '+');
	// std::map<char, bool> stock;
	// for (size_t i = 0; i < modestring.size(); i++)
	// {
	// 	char current = modestring[i];
	// 	if (current == '+' || current == '-')
	// 	{
	// 		add = (current == '+');
	// 		if (i + 1 == modestring.size())
	// 			srv->sendReply(userfd, ERR_UMODEUNKNOWNFLAG(user->getNickname()));
	// 		continue;
	// 	}

	// 	// If letter is unrecognized, send error
	// 	srv->sendReply(userfd, ERR_UMODEUNKNOWNFLAG(user->getNickname()));
	// }

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
		channelmode(srv, userfd, cmd);
		return;
	}
	// Command for users
	else
	{
		usermode(srv, userfd, cmd);
		return;
	}
}
