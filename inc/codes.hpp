#ifndef CODES_HPP
# define CODES_HPP

/* REPLIES */

#define RPL_WELCOME(client) ("001 " + client + " :Welcome to ft_irc network, " + client)
#define RPL_YOURHOST(client, servername) ("002 " + client + " :Your host is " + servername + ", running version 1.0")
#define RPL_CREATED(client, datetime) ("003 " + client + " :This server was created " + datetime)
#define RPL_MYINFO(client, servername, available_user_modes, available_channel_modes) (client + " " + servername + " 1.0 " + available_user_modes + available_channel_modes)

// #define RPL_ISUPPORT ???

#define RPL_UMODEIS(client, user_modes) ("221 " + client + " " + user_modes)
#define RPL_LUSERCLIENT(client, u, i) ("251 " + client + " :There are " + u + " users and " + i + " invisible")
#define RPL_LUSEROP(client, ops) ("252 " + client + " " + ops + " :operator(s) online")
#define RPL_LUSERUNKNOWN(client, connections) ("253 " + client + " " + connections + " :unknown connection(s)")
#define RPL_LUSERCHANNELS(client, channels) ("254 " + client + " " + channels + " :channels formed")
#define RPL_LUSERME(client, c) ("255 " + client + " :I have " + c + " clients and 0 servers")

#define RPL_ADMINME(client) ("256 " + client + " :Administrative info")
#define RPL_ADMINLOC1(client, info) ("257 " + client + " " + info)
#define RPL_ADMINLOC2(client, info) ("258 " + client + " " + info)
#define RPL_ADMINEMAIL(client, info) ("259 " + client + " " + info)

#define RPL_TRYAGAIN(client, command) ("263 " + client + " " + command + " :Please wait a while and try again.")
#define RPL_LOCALUSERS(client, u, m) ("265 " + client + " " + u + " " + m + " :Current local users <u>, max <m>")
#define RPL_GLOBALUSERS(client, u, m) ("266 " + client + " " + u + " " + m + " :Current global users <u>, max <m>")
#define RPL_WHOISCERTFP(client, nick, fingerprint) ("276 " + client + " " + nick + " :has certificate fingerprint " + fingerprint)

#define RPL_AWAY(client, nick, message) ("301 " + client + " " + nick + " " + message)
#define RPL_USERHOST(client, replies) ("302 " + client + " :" + replies)
#define RPL_UNAWAY(client) ("305 " + client + " :You are no longer marked as being away")
#define RPL_NOWAWAY(client) ("306 " + client + " :You have been marked as being away")

#define RPL_WHOREPLY(client, channel, username, host, servername, nick, flags, hopcount, realname) ("352 " + client + ' ' + channel + ' ' + username + ' ' + host + " " + servername + " " + nick + ' ' + flags + " :" + hopcount + " " + realname)
#define RPL_ENDOFWHO(client, mask) ("315 " + client + " " + mask + " :End of WHO list")
#define RPL_WHOISREGNICK(client, nick) ("307 " + client + " " + nick + " :has identified for this nick")
#define RPL_WHOISUSER(client, nick, username, host, realname) ("311 " + client + " " + nick + " " + username + " " + host + " * :" + realname)
#define RPL_WHOISSERVER(client, nick, servername) ("312 " + client + " " + nick + " " + servername + " :Custom server")
#define RPL_WHOISOPERATOR(client, nick) ("313 " + client + " " + nick + " :is an IRC operator")
#define RPL_WHOWASUSER(client, nick, username, host, realname) ("314 " + client + " " + nick + " " + username + " " + host + " * :" + realname)
#define RPL_WHOISIDLE(client, nick, secs, signon) ("317 " + client + " " + nick + " " + secs + " " + signon + " :seconds idle, signon time")
#define RPL_ENDOFWHOIS(client, nick) ("318 " + client + " " + nick + " :End of /WHOIS list")
#define RPL_WHOISCHANNELS(client, nick, channels) ("319 " + client + " " + nick + " :" + channels)

#define RPL_LISTSTART(client) ("321 " + client + " Channel :Users  Name")
#define RPL_LIST(client, channel, client_count, topic) ("322 " + client + " " + channel + " " + client_count + " :" + topic)
#define RPL_LISTEND(client) ("323 " + client + " :End of /LIST")

#define RPL_CHANNELMODEIS(client, channel, modestring) ("324 " + client + " " + channel + " " + modestring)
#define RPL_CREATIONTIME(client, channel, creationtime) ("329 " + client + " " + channel + " " + creationtime)
#define RPL_WHOISACCOUNT(client, nick, account) ("330 " + client + " " + nick + " " + account + " :is logged in as")
#define RPL_NOTOPIC(client, channel) ("331 " + client + " " + channel + " :No topic is set")
#define RPL_TOPIC(client, channel, topic) ("332 " + client + " " + channel + " :" + topic)
#define RPL_TOPICWHOTIME(client, channel, nick, setat) ("333 " + client + " " + channel + " " + nick + " " + setat)

#define RPL_INVITELIST(client, channel) ("336 " + client + " " + channel)
#define RPL_ENDOFINVITELIST(client) ("337 " + client + " :End of /INVITE list")
#define RPL_WHOISACTUALLY(client, nick) ("338 " + client + " " + nick + " :is actually ")
#define RPL_INVITING(client, nick, channel) ("341 " + client + " " + nick + " " + channel)
#define RPL_INVEXLIST(client, channel, mask) ("346 " + client + " " + channel + " " + mask)
#define RPL_ENDOFINVEXLIST(client, channel) ("347 " + client + " " + channel + " :End of Channel Invite Exception List")
#define RPL_EXCEPTLIST(client, channel, mask) ("348 " + client + " " + channel + " " + mask)
#define RPL_ENDOFEXCEPTLIST(client, channel) ("349 " + client + " " + channel + " :End of Channel exception List")

#define RPL_VERSION(client, version, servername, comments) ("351 " + client + " " + version + " " + servername + " :" + comments)
#define RPL_NAMREPLY(client, symbol, channel, users) ("353 " + client + " " + symbol + " " + channel + " :" + users)
#define RPL_ENDOFNAMES(client, channel) ("366 " + client + " " + channel + " :End of /NAMES list")
#define RPL_LINKS(client, servername) ("364 " + client + " * " + servername + " :0 this server")
#define RPL_ENDOFLINKS(client) ("365 " + client + " * :End of /LINKS list")
#define RPL_BANLIST(client, channel, mask) ("367 " + client + " " + channel + " " + mask)
#define RPL_ENDOFBANLIST(client, channel) ("368 " + client + " " + channel + " :End of channel ban list")
#define RPL_ENDOFWHOWAS(client, nick) ("369 " + client + " " + nick + " :End of WHOWAS")
#define RPL_INFO(client, string) ("371 " + client + " :" + string)
#define RPL_ENDOFINFO(client) ("374 " + client + " :End of INFO list")
#define RPL_MOTDSTART(client, servername) ("375 " + client + "  :- " + servername + " Message of the day - ")
#define RPL_MOTD(client, line) ("372 " + client + " :" line)
#define RPL_ENDOFMOTD(client) ("376 " + client + " :End of /MOTD command.")
#define RPL_WHOISHOST(client, nick, host) ("378 " + client + " " + nick + "  :is connecting from " + host)
#define RPL_WHOISMODES(client, nick, modes) ("379 " + client + " " + nick + " :is using modes " + modes)

#define RPL_YOUREOPER(client) ("381 " + client + " :You are now an IRC operator")
#define RPL_REHASHING(client, config_file) ("382 " + client + " " + config_file + " :Rehashing")
#define RPL_TIME(client, servername, time) ("391 " + client + " " + servername + " :" + time)

#define RPL_WHOISSECURE(client, nick) ("670 " + client + " " + nick + " :is using a secure connection")

#define RPL_HELPSTART(client, subject, line) ("704 " + client + " " + subject + " :" + line)
#define RPL_HELPTXT(client, subject, line) ("705 " + client + " " + subject + " :" + line)
#define RPL_ENDOFHELP(client, subject, line) ("706 " + client + " " + subject + " :" + line)

/* ERRORS */

#define ERR_UNKNOWNERROR(client, command, info) ("400 " + client + " " + command + " :" + info )
#define ERR_NOSUCHNICK(client, nick) ("401 " + client + " " + nick + " :No such nick")
#define ERR_NOSUCHSERVER(client, server) ("402 " + client + " " + server + " :No such server")
#define ERR_NOSUCHCHANNEL(client, channel) ("403 " + client + " " + channel + " :No such channel")
#define ERR_CANNOTSENDTOCHAN(client, channel) ("404 " + client + " " + channel + " :Cannot send to channel")

#define ERR_TOOMANYCHANNELS(client, channel) ("405 " + client + " " + channel + " :You have joined too many channels")
#define ERR_WASNOSUCHNICK(client) ("406 " + client + " :There was no such nickname")
#define ERR_TOOMANYTARGETS(client, target) ("407 " + client + " " + target + " :Duplicate recipients. No message delivered")
#define ERR_NOORIGIN(client) ("409 " + client + " :No origin specified")
#define ERR_NORECIPIENT(client) ("411 " + client + " :No recipient given")

#define ERR_NOTEXTTOSEND(client) ("412 " + client + " :No text to send")
#define ERR_NOTOPLEVEL(client, mask) ("413 " + client + " " + mask + " :No toplevel domain specified")
#define ERR_WILDTOPLEVEL(client, mask) ("414 " + client + " " + mask + " :Wildcard in toplevel domain")
#define ERR_INPUTTOOLONG(client) ("417 " + client + " :Input line was too long")
#define ERR_UNKNOWNCOMMAND(client, command) ("421 " + client + " " + command + " :Unknown command")

#define ERR_NOMOTD(client) ("422 " + client + " :MOTD file is missing")
#define ERR_NOADMININFO(client, server) ("423 " + client + " " + server + " :No administrative info available")
#define ERR_NONICKNAMEGIVEN(client) ("431 " + client + " :No nickname given")
#define ERR_ERRONEUSNICKNAME(client, nick) ("432 " + client + " " + nick + " :Erroneus nickname")
#define ERR_NICKNAMEINUSE(client, nick) ("433 " + client + " " + nick + " :Nickname is already in use")

#define ERR_USERNOTINCHANNEL(client, nick, channel) ("441 " + client + " " + nick + " " + channel + " :They aren't on that channel")
#define ERR_NOTONCHANNEL(client, channel) ("442 " + client + " " + channel + " :You are not on that channel")
#define ERR_USERONCHANNEL(client, nick, channel) ("443 " + client + " " + nick + " " + channel + " :is already on channel")
#define ERR_NOTREGISTERED(client) ("451 " + client + " :You have not registered")
#define ERR_NEEDMOREPARAMS(client, command) ("461 " + client + " " + command + " :Not enough parameters")

#define ERR_ALREADYREGISTERED(client) ("462 " + client + " :You may not reregister")
#define ERR_PASSWDMISMATCH(client) ("464 " + client + " :Password incorrect")
#define ERR_YOUREBANNEDCREEP(client) ("465 " + client + " :You are banned from this server.")
#define ERR_CHANNELISFULL(client, channel) ("471 " + client + " " + channel + " :Cannot join channel (+l)")
#define ERR_UNKNOWNMODE(client, modechar) ("472 " + client + " " + modechar + " :is unknown mode char to me")

#define ERR_INVITEONLYCHAN(client, channel) ("473 " + client + " " + channel + " :Cannot join channel (+i)")
#define ERR_BANNEDFROMCHAN(client, channel) ("474 " + client + " " + channel + " :Cannot join channel (+b)")
#define ERR_BADCHANNELKEY(client, channel) ("475 " + client + " " + channel + " :Cannot join channel (+k)")
#define ERR_BADCHANMASK(channel) ("476 " + channel + " :Bad Channel Mask")
#define ERR_NOPRIVILEGES(client) ("481 " + client + " :Permission Denied- You're not an IRC operator")

#define ERR_CHANOPRIVSNEEDED(client, channel) ("482 " + client + " " + channel + " :You're not channel operator")
#define ERR_CANTKILLSERVER(client) ("483 " + client + " :You cant kill a server!")
#define ERR_NOOPERHOST(client) ("491 " + client + " :No O-lines for your host")
#define ERR_UMODEUNKNOWNFLAG(client) ("501 " + client + " :Unknown MODE flag")
#define ERR_USERSDONTMATCH(client) ("502 " + client + " :Cant change mode for other users")

#define ERR_HELPNOTFOUND(client, subject) ("524 " + client + " " + subject + " :No help available on this topic")
#define ERR_INVALIDKEY(client, channel) ("525 " + client + " " + channel + " :Key is not well-formed")
#define ERR_INVALIDMODEPARAM(client, target, modechar, param, description) ("696 " + client + " " + target + " " + modechar + " " + param + " :" + description)
#define ERR_NOPRIVS(client, priv) ("723 " + client + " " + priv + " :Insufficient oper privileges.")

enum err_codes {
	err_noerror = 0,
	
	err_unknownerror = 400,
	err_nosuchnick = 401,
	err_nosuchserver = 402,
	err_nosuchchannel = 403,
	err_cannotsendtochan = 404,
	
	err_toomanychannels = 405,
	err_wasnosuchnick = 406,
	err_toomanytargets = 407,
	err_noorigin = 409,
	err_norecipient = 411,
	
	err_notexttosend = 412,
	err_notoplevel = 413,
	err_wildtoplevel = 414,
	err_inputtoolong = 417,
	err_unknowncommand = 421,
	
	err_nomotd = 422,
	err_noadmininfo = 423,
	err_nonicknamegiven = 431,
	err_erroneusnickname = 432,
	err_nicknameinuse = 433,
	
	err_usernotinchannel = 441,
	err_notonchannel = 442,
	err_useronchannel = 443,
	err_notregistered = 451,
	err_needmoreparams = 461,

	err_alreadyregistered = 462,
	err_passwdmismatch = 464,
	err_yourebannedcreep = 465,
	err_channelisfull = 471,
	err_unknownmode = 472,

	err_inviteonlychan = 473,
	err_bannedfromchan = 474,
	err_badchannelkey = 475,
	err_badchanmask = 476,
	err_noprivileges = 481,

	err_chanoprivsneeded = 482,
	err_cantkillserver = 483,
	err_nooperhost = 491,
	err_umodeunknownflag = 501,
	err_usersdontmatch = 502,
	
	err_helpnotfound = 524,
	err_invalidkey = 525,
	err_invalidmodeparam = 696,
	err_noprivs = 723
};

#endif
