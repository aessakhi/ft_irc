#include "main.hpp"

/* Add c to the correct string. */
static void	_add_to_summary(char c, std::map<char, bool> stock, std::string * added, std::string * removed)
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

/* ------------------------------------------------- */
/* --------------------USER MODE-------------------- */
/* ------------------------------------------------- */


/*
User modes :
a = away
i = invisible
o = server oper
w = wallops
*/
static void	_usermode(Server *srv, int &userfd, Command &cmd)
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

	_add_to_summary('o', stock, &added, &removed);
	_add_to_summary('i', stock, &added, &removed);
	_add_to_summary('w', stock, &added, &removed);

	std::string reply_str;
	if (added.size() > 1)
		reply_str.append(added);
	if (added.size() > 1 && removed.size() > 1)
		reply_str.push_back(' ');
	if (removed.size() > 1)
		reply_str.append(removed);

	srv->sendReply(userfd, ":" + user->getNickname() + " MODE " + user->getNickname() + " :" + reply_str);
}

/* ---------------------------------------------------- */
/* --------------------CHANNEL MODE-------------------- */
/* ---------------------------------------------------- */

/* Basic error checks before parsing */
static bool _channel_mode_error(Server *srv, int &userfd, Command &cmd)
{
	std::string target = cmd.getParam(0);
	User * user = srv->getUser(userfd);
	Channel * channel = srv->getChannel(target);

	// Error if target is a channel that doesnt exist
	if (channel == NULL)
	{
		srv->sendReply(userfd, ERR_NOSUCHCHANNEL(user->getNickname(), target));
		return true;
	}

	// If no modestring is given, reply with the modes of the channel.
	if (cmd.paramNumber() == 1)
	{
		srv->sendReply(userfd, RPL_CHANNELMODEIS(user->getNickname(), target, channel->getModes()));
		return true;
	}

	// If modestring is given, check user privileges

	if (!channel->isOp(user))
	{
		srv->sendReply(userfd, ERR_CHANOPRIVSNEEDED(user->getNickname(), target));
		return true;
	}

	return false;
}

/* Returns a vector with the different modes */
static std::vector<Mode> _modestring_parsing(std::string modestring, Server *srv, int &userfd, Command &cmd)
{
	User * user = srv->getUser(userfd);

	// Might have a parameter
	// If no parameter, reply with the list
	std::string typeA("beIov");
	// ALWAYS a parameter
	std::string typeB("k");
	// Parameter when being set, no parameter when unset
	std::string typeC("l");
	// NEVER a parameter
	std::string typeD("imstn");

	// arguments points to the arglist after the modestring
	std::vector<std::string> param_list = cmd.getParamList();
	std::vector<std::string>::const_iterator arguments = param_list.begin();
	arguments += 2;
	std::vector<std::string>::const_iterator arguments_end = param_list.end();
	
	// whether or not the current mode should be added 
	bool add = (modestring[0] == '+');
	size_t npos = std::string::npos;

	std::vector<Mode> mode_list;

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

		if (typeA.find(c) != npos)
		{
			// request list
			if (arguments == arguments_end)
			{
				mode_list.push_back(Mode(add, c));
				continue;
			}
			// mode adds a mask to a list
			else
			{
				mode_list.push_back(Mode(add, c, *arguments));
				arguments++;
				continue;
			}
		}

		if (typeB.find(c) != npos)
		{
			// error
			if (arguments == arguments_end)
			{
				srv->sendReply(userfd, ERR_UMODEUNKNOWNFLAG(user->getNickname()));
				continue;
			}

			// mode change
			mode_list.push_back(Mode(add, c, *arguments));
			arguments++;
			continue;
		}

		if (typeC.find(c) != npos)
		{
			// error
			if (arguments == arguments_end && add)
			{
				srv->sendReply(userfd, ERR_UMODEUNKNOWNFLAG(user->getNickname()));
				continue;
			}

			// mode is being set
			if (add)
			{
				mode_list.push_back(Mode(add, c, *arguments));
				arguments++;
				continue;
			}
			// mode is being unset
			else
			{
				mode_list.push_back(Mode(add, c));
				continue;
			}
		}

		if (typeD.find(c) != npos)
		{
			// mode change
			mode_list.push_back(Mode(add, c));
			continue;
		}

		// If letter is unrecognized, send error
		srv->sendReply(userfd, ERR_UMODEUNKNOWNFLAG(user->getNickname()));
	}

	return mode_list;
}

/* returns true if str is only digits */

static bool _is_digits(std::string s)
{
	for (size_t i = 0; s[i]; i++)
	{
		if (!isdigit(s[i]))
			return false;
	}
	return (s[0] != 0);
}


static void _applyModeChanges(std::vector<Mode> * mode_list, Server *srv, int &userfd, Command &cmd)
{
	std::string target = cmd.getParam(0);
	User * user = srv->getUser(userfd);
	Channel * channel = srv->getChannel(target);

	std::vector<Mode>::iterator it = mode_list->begin();
	std::vector<Mode>::iterator ite = mode_list->end();

	UserMask usermask;

	for (; it != ite; )
	{
		std::string arg = it->getArg();
		std::vector<UserMask> mask_list;
		std::vector<UserMask>::iterator mask_it;
		std::vector<UserMask>::iterator mask_ite;
		switch (it->getMode())
		{
			// Type A

			// BAN
			case 'b':
			{
				mask_list = channel->getBanned();
				mask_it = mask_list.begin();
				mask_ite = mask_list.end();

				// Send ban list
				if (it->getArg().empty())
				{
					for (; mask_it != mask_ite; mask_it++)
						srv->sendReply(userfd, RPL_BANLIST(user->getNickname(), target, mask_it->getMask()));
					srv->sendReply(userfd, RPL_ENDOFBANLIST(user->getNickname(), target));
				}
				// Add to ban list
				if (it->getAdd())
				{
					if (isMask(arg))
						usermask.initFromMask(arg);
					else
						usermask.initFromNick(arg);
					channel->banUser(usermask);
				}
				// Remove all corresponding masks from ban list
				else
				{
					for (; mask_it != mask_ite; mask_it++)
					{
						if (sided_wildcompare(arg, mask_it->getMask()))
						{
							mask_list.erase(mask_it);
							mask_it--;
							mask_ite--;
						}
					}
				}
				break;
			}

			// BAN-EXCEPT
			case 'e':
			{
				mask_list = channel->getBanExcept();
				mask_it = mask_list.begin();
				mask_ite = mask_list.end();

				// Send ban-except list
				if (it->getArg().empty())
				{
					for (; mask_it != mask_ite; mask_it++)
						srv->sendReply(userfd, RPL_EXCEPTLIST(user->getNickname(), target, mask_it->getMask()));
					srv->sendReply(userfd, RPL_ENDOFEXCEPTLIST(user->getNickname(), target));
				}
				// Add to ban-except list
				if (it->getAdd())
				{
					if (isMask(arg))
						usermask.initFromMask(arg);
					else
						usermask.initFromNick(arg);
					channel->addBanExcept(usermask);
				}
				// Remove all corresponding masks from ban-except list
				else
				{
					for (; mask_it != mask_ite; mask_it++)
					{
						if (sided_wildcompare(arg, mask_it->getMask()))
						{
							mask_list.erase(mask_it);
							mask_it--;
							mask_ite--;
						}
					}
				}
				break;
			}

			// INVITE-EXCEPT
			case 'I':
			{
				mask_list = channel->getInviteExcept();
				mask_it = mask_list.begin();
				mask_ite = mask_list.end();

				// Send invite-except list
				if (it->getArg().empty())
				{
					for (; mask_it != mask_ite; mask_it++)
						srv->sendReply(userfd, RPL_INVEXLIST(user->getNickname(), target, mask_it->getMask()));
					srv->sendReply(userfd, RPL_ENDOFINVEXLIST(user->getNickname(), target));
				}
				// Add to invite-except list
				if (it->getAdd())
				{
					if (isMask(arg))
						usermask.initFromMask(arg);
					else
						usermask.initFromNick(arg);
					channel->addInviteExcept(usermask);
				}
				// Remove all corresponding masks from invite-except list
				else
				{
					for (; mask_it != mask_ite; mask_it++)
					{
						if (sided_wildcompare(arg, mask_it->getMask()))
						{
							mask_list.erase(mask_it);
							mask_it--;
							mask_ite--;
						}
					}
				}
				break;
			}

			// OPERATORS
			case 'o':
			{
				mask_list = channel->getOperators();
				mask_it = mask_list.begin();
				mask_ite = mask_list.end();

				// DONT Send operator list
				if (it->getArg().empty())
				{
					srv->sendReply(userfd, ERR_INVALIDMODEPARAM(user->getNickname(), target, it->getMode(), arg, "Cannot get channel operator list"));
					mode_list->erase(it);
					it--;
					ite--;
					break;
				}
				// Add to operator list
				if (it->getAdd())
				{
					if (isMask(arg))
						usermask.initFromMask(arg);
					else
						usermask.initFromNick(arg);
					channel->addOperator(usermask);
				}
				// Remove all corresponding masks from operator list
				else
				{
					for (; mask_it != mask_ite; mask_it++)
					{
						if (sided_wildcompare(arg, mask_it->getMask()))
						{
							mask_list.erase(mask_it);
							mask_it--;
							mask_ite--;
						}
					}
				}
				break;
			}

			// VOICED
			case 'v':
			{
				mask_list = channel->getVoiced();
				mask_it = mask_list.begin();
				mask_ite = mask_list.end();

				// DONT Send voiced list
				if (it->getArg().empty())
				{
					srv->sendReply(userfd, ERR_INVALIDMODEPARAM(user->getNickname(), target, it->getMode(), arg, "Cannot get channel voiced list"));
					mode_list->erase(it);
					it--;
					ite--;
					break;
				}
				// Add to voiced list
				if (it->getAdd())
				{
					if (isMask(arg))
						usermask.initFromMask(arg);
					else
						usermask.initFromNick(arg);
					channel->addVoiced(usermask);
				}
				// Remove all corresponding masks from voiced list
				else
				{
					for (; mask_it != mask_ite; mask_it++)
					{
						if (sided_wildcompare(arg, mask_it->getMask()))
						{
							mask_list.erase(mask_it);
							mask_it--;
							mask_ite--;
						}
					}
				}
				break;
			}

			// Type B

			// KEY CHANGE
			case 'k':
			{
				if (it->getAdd())
				{
					channel->setKey(true, arg);
				}
				else if (channel->checkKey(arg))
				{
					channel->setKey(false);
				}
				else
				{
					srv->sendReply(userfd, ERR_INVALIDMODEPARAM(user->getNickname(), target, it->getMode(), arg, "Wrong key supplied"));
					mode_list->erase(it);
					it--;
					ite--;
				}
				break;
			}

			// Type C

			// LIMIT CHANGE
			case 'l':
			{
				if (it->getAdd())
				{
					if (!_is_digits(arg))
					{
						srv->sendReply(userfd, ERR_INVALIDMODEPARAM(user->getNickname(), target, it->getMode(), arg, "Not a numeric limit"));
						mode_list->erase(it);
						it--;
						ite--;
						break;
					}
					size_t max = atoi(arg.data());
					channel->setLimit(true, max);
				}
				else
				{
					channel->setLimit(false);
				}
				break;
			}

			// Type D

			// INVITE MODE
			case 'i':
			{
				channel->setInviteMode(it->getAdd());
				break;
			}

			// MODERATED MODE
			case 'm':
			{
				channel->setModeratedMode(it->getAdd());
				break;
			}

			// SECRET MODE
			case 's':
			{
				channel->setSecretMode(it->getAdd());
				break;
			}

			// PROTECTED TOPIC MODE
			case 't':
			{
				channel->setProtectedTopicMode(it->getAdd());
				break;
			}

			// NO EXTERNAL MESSAGES MODE
			case 'n':
			{
				channel->setNoExternalMessagesMode(it->getAdd());
				break;
			}
		}
		it++;
	}
}


// Channel modes :
// 	-> requiring str (mask or nick or key):
// 		b = ban
// 		e = ban-except
// 		I = invite-except
// 		o = op user
// 		v = voice user
// 		k = key
// 	-> requiring int:
// 		l = limit
// 	-> no arg:
// 		i = invite only
// 		m = moderated
// 		s = secret
// 		t = protected topic
// 		n = no external messages

static void	_channelmode(Server *srv, int &userfd, Command &cmd)
{
	std::string target = cmd.getParam(0);
	User * user = srv->getUser(userfd);
	// Channel * channel = srv->getChannel(target);

	// Error check
	if (_channel_mode_error(srv, userfd, cmd))
		return;

	// Parsing
	std::string modestring = cmd.getParam(1);

	// If first char isnt + or -, handle error however we want
	if (modestring[0] != '+' && modestring[0] != '-')
	{
		srv->sendReply(userfd, ERR_UMODEUNKNOWNFLAG(user->getNickname()));
		return;
	}

	std::vector<Mode> mode_list = _modestring_parsing(modestring, srv, userfd, cmd);

	// Now mode_list is filled with the different recognized mode changes requested

	_applyModeChanges(&mode_list, srv, userfd, cmd);

	// --> Send message to all joined users to inform them of the changes

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
		_channelmode(srv, userfd, cmd);
		return;
	}
	// Command for users
	else
	{
		_usermode(srv, userfd, cmd);
		return;
	}
}
