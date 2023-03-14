#include "main.hpp"

/* ------------------------- */
/* ----------UTILS---------- */
/* ------------------------- */

static bool _is_digits(std::string s)
{
	for (size_t i = 0; s[i]; i++)
	{
		if (!isdigit(s[i]))
			return false;
	}
	return (s[0] != 0);
}

static bool _op_check(Channel * channel, Server *srv, int &userfd, Command &cmd)
{
	User * user = srv->getUser(userfd);
	if (!channel->isOp(user))
	{
		srv->sendReply(userfd, ERR_CHANOPRIVSNEEDED(user->getNickname(), cmd.getParam(0)));
		return false;
	}
	return true;
}

/* ------------------------ */
/* ----------BANS---------- */
/* ------------------------ */

static bool _send_ban_list(std::vector<std::string> mask_list, Server *srv, int &userfd, Command &cmd)
{
	std::string target = cmd.getParam(0);
	User * user = srv->getUser(userfd);
	std::vector<std::string>::const_iterator mask_it = mask_list.begin();
	std::vector<std::string>::const_iterator mask_ite = mask_list.end();
	for (; mask_it != mask_ite; mask_it++)
		srv->sendReply(userfd, RPL_BANLIST(user->getNickname(), target, *mask_it));
	srv->sendReply(userfd, RPL_ENDOFBANLIST(user->getNickname(), target));
	return false;
}

static bool _apply_ban(Mode mode, Channel * channel, Server *srv, int &userfd, Command &cmd)
{
	// Send list
	if (mode.getArg().empty())
	{
		_send_ban_list(channel->getBanned(), srv, userfd, cmd);
		return false ;
	}

	if (!_op_check(channel, srv, userfd, cmd))
		return false;

	// Add to list
	if (mode.getAdd())
	{
		channel->banUser(mode.getArg());
		return true ;
	}

	// Remove all corresponding masks from list
	channel->unbanUser(mode.getArg());
	return true ;
}

/* ------------------------------- */
/* ----------BAN-EXCEPTS---------- */
/* ------------------------------- */

static bool _send_ban_except_list(std::vector<std::string> mask_list, Server *srv, int &userfd, Command &cmd)
{
	std::string target = cmd.getParam(0);
	User * user = srv->getUser(userfd);
	std::vector<std::string>::const_iterator mask_it = mask_list.begin();
	std::vector<std::string>::const_iterator mask_ite = mask_list.end();
	for (; mask_it != mask_ite; mask_it++)
		srv->sendReply(userfd, RPL_EXCEPTLIST(user->getNickname(), target, *mask_it));
	srv->sendReply(userfd, RPL_ENDOFEXCEPTLIST(user->getNickname(), target));
	return false;
}

static bool _apply_ban_except(Mode mode, Channel * channel, Server *srv, int &userfd, Command &cmd)
{
	// Send list
	if (mode.getArg().empty())
	{
		_send_ban_except_list(channel->getBanExcept(), srv, userfd, cmd);
		return false ;
	}

	if (!_op_check(channel, srv, userfd, cmd))
		return false;

	// Add to list
	if (mode.getAdd())
	{
		channel->addBanExcept(mode.getArg());
		return true ;
	}

	// Remove all corresponding masks from list
	channel->removeBanExcept(mode.getArg());
	return true ;
}

/* ---------------------------------- */
/* ----------INVITE-EXCEPTS---------- */
/* ---------------------------------- */

static bool _send_invite_except_list(std::vector<std::string> mask_list, Server *srv, int &userfd, Command &cmd)
{
	std::string target = cmd.getParam(0);
	User * user = srv->getUser(userfd);
	std::vector<std::string>::const_iterator mask_it = mask_list.begin();
	std::vector<std::string>::const_iterator mask_ite = mask_list.end();
	for (; mask_it != mask_ite; mask_it++)
		srv->sendReply(userfd, RPL_INVEXLIST(user->getNickname(), target, *mask_it));
	srv->sendReply(userfd, RPL_ENDOFINVEXLIST(user->getNickname(), target));
	return false;
}

static bool _apply_invite_except(Mode mode, Channel * channel, Server *srv, int &userfd, Command &cmd)
{
	// Send list
	if (mode.getArg().empty())
	{
		_send_invite_except_list(channel->getInviteExcept(), srv, userfd, cmd);
		return false ;
	}

	if (!_op_check(channel, srv, userfd, cmd))
		return false;

	// Add to list
	if (mode.getAdd())
	{
		channel->addInviteExcept(mode.getArg());
		return true ;
	}

	// Remove all corresponding masks from list
	channel->removeInviteExcept(mode.getArg());
	return true ;
}

/* ----------------------------- */
/* ----------OPERATORS---------- */
/* ----------------------------- */

static bool _apply_operator(Mode mode, Channel * channel, Server *srv, int &userfd, Command &cmd)
{
	User * user = srv->getUser(userfd);

	// DONT Send list
	if (mode.getArg().empty())
	{
		srv->sendReply(userfd, ERR_INVALIDMODEPARAM(user->getNickname(), cmd.getParam(0), mode.getMode(), mode.getArg(), "Cannot get channel operator list"));
		return false ;
	}

	if (!_op_check(channel, srv, userfd, cmd))
		return false;

	// Add to list
	if (mode.getAdd())
	{
		channel->addOperator(mode.getArg());
		return true ;
	}

	// Remove all corresponding masks from list
	channel->removeOperator(mode.getArg());
	return true ;
}

/* -------------------------- */
/* ----------VOICED---------- */
/* -------------------------- */

static bool _apply_voiced(Mode mode, Channel * channel, Server *srv, int &userfd, Command &cmd)
{
	User * user = srv->getUser(userfd);

	// DONT Send list
	if (mode.getArg().empty())
	{
		srv->sendReply(userfd, ERR_INVALIDMODEPARAM(user->getNickname(), cmd.getParam(0), mode.getMode(), mode.getArg(), "Cannot get channel voiced list"));
		return false ;
	}

	if (!_op_check(channel, srv, userfd, cmd))
		return false;

	// Add to list
	if (mode.getAdd())
	{
		channel->addVoiced(mode.getArg());
		return true ;
	}

	// Remove all corresponding masks from list
	channel->removeVoiced(mode.getArg());
	return true ;
}

/* ----------------------------------- */
/* ----------OTHER FUNCTIONS---------- */
/* ----------------------------------- */

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
/*
	// If modestring is given, check user privileges

	if (!channel->isOp(user))
	{
		srv->sendReply(userfd, ERR_CHANOPRIVSNEEDED(user->getNickname(), target));
		return true;
	}
*/
	return false;
}

static std::list<Mode> _modestring_parsing(std::string modestring, Server *srv, int &userfd, Command &cmd)
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
	bool add = (modestring[0] != '-');
	size_t npos = std::string::npos;

	std::list<Mode> mode_list;

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

static std::vector<Mode> _apply_mode_changes(std::list<Mode> mode_list, Server *srv, int &userfd, Command &cmd)
{
	std::string target = cmd.getParam(0);
	User * user = srv->getUser(userfd);
	Channel * channel = srv->getChannel(target);

	std::list<Mode>::iterator it = mode_list.begin();
	std::list<Mode>::iterator ite = mode_list.end();

	std::vector<Mode> applied_changes;

	for (; it != ite; it++)
	{
		std::string arg = it->getArg();
		switch (it->getMode())
		{
		// Type A

			case 'b':
			{
				if (_apply_ban(*it, channel, srv, userfd, cmd))
					applied_changes.push_back(*it);
				break;
			}

			// BAN-EXCEPT
			case 'e':
			{
				if (_apply_ban_except(*it, channel, srv, userfd, cmd))
					applied_changes.push_back(*it);
				break;
			}

			// INVITE-EXCEPT
			case 'I':
			{
				if (_apply_invite_except(*it, channel, srv, userfd, cmd))
					applied_changes.push_back(*it);
				break;
			}

			// OPERATORS
			case 'o':
			{
				if (_apply_operator(*it, channel, srv, userfd, cmd))
					applied_changes.push_back(*it);
				break;
			}

			// VOICED
			case 'v':
			{
				if (_apply_voiced(*it, channel, srv, userfd, cmd))
					applied_changes.push_back(*it);
				break;
			}

		// Type B

			// KEY CHANGE
			case 'k':
			{
				Mode _copy(it->getAdd(), it->getMode());
				if (it->getAdd())
				{
					channel->setKey(true, arg);
					applied_changes.push_back(_copy);
				}
				else if (channel->checkKey(arg))
				{
					channel->setKey(false);
					applied_changes.push_back(_copy);
				}
				else
				{
					srv->sendReply(userfd, ERR_INVALIDMODEPARAM(user->getNickname(), target, it->getMode(), arg, "Wrong key supplied"));
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
						break;
					}
					size_t max = atoi(arg.data());
					channel->setLimit(true, max);
				}
				else
				{
					channel->setLimit(false);
				}
				applied_changes.push_back(*it);
				break;
			}

		// Type D

			// INVITE MODE
			case 'i':
			{
				channel->setInviteMode(it->getAdd());
				applied_changes.push_back(*it);
				break;
			}

			// MODERATED MODE
			case 'm':
			{
				channel->setModeratedMode(it->getAdd());
				applied_changes.push_back(*it);
				break;
			}

			// SECRET MODE
			case 's':
			{
				channel->setSecretMode(it->getAdd());
				applied_changes.push_back(*it);
				break;
			}

			// PROTECTED TOPIC MODE
			case 't':
			{
				channel->setProtectedTopicMode(it->getAdd());
				applied_changes.push_back(*it);
				break;
			}

			// NO EXTERNAL MESSAGES MODE
			case 'n':
			{
				channel->setNoExternalMessagesMode(it->getAdd());
				applied_changes.push_back(*it);
				break;
			}
		}
	}

	return applied_changes;
}

static std::string	_get_changed_str(std::vector<Mode> changes)
{
	std::string modes;
	bool last_add;
	std::string args;

	std::vector<Mode>::const_iterator it = changes.begin();
	std::vector<Mode>::const_iterator ite = changes.end();
	if (it != ite)
		last_add = !(it->getAdd());
	for (; it != ite; it++)
	{
		if (it->getAdd() && !last_add)
			modes += '+';
		if (!it->getAdd() && last_add)
			modes += '-';
		last_add = it->getAdd();
		modes += it->getMode();
		if (!it->getArg().empty())
			args += it->getArg() + " ";
	}

	if (!args.empty())
	{
		args.erase(args.end() - 1);
		modes += " " + args;
	}
	
	return modes;
}

void	channelmode(Server *srv, int &userfd, Command &cmd)
{

	if (_channel_mode_error(srv, userfd, cmd))
		return;

	std::string target = cmd.getParam(0);
	User * user = srv->getUser(userfd);
	Channel * channel = srv->getChannel(target);
	std::string modestring = cmd.getParam(1);

	std::list<Mode> mode_list = _modestring_parsing(modestring, srv, userfd, cmd);

	std::vector<Mode> applied_changes = _apply_mode_changes(mode_list, srv, userfd, cmd);

	std::string changed_str = _get_changed_str(applied_changes);

	if (!changed_str.empty())
		channel->sendToAllMembers(":" + user->getMask() + " MODE " + target + " " + changed_str);
}
