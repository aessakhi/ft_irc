/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codes.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldesnoye <ldesnoye@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/15 14:12:34 by ldesnoye          #+#    #+#             */
/*   Updated: 2023/02/15 18:00:09 by ldesnoye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODES_HPP
# define CODES_HPP

#define RPL_WELCOME(client) ("001 " + client + " :Welcome to ft_irc, " + client)
// #define RPL_YOURHOST(client) ("002 " + client + " :Your host is ft_irc")
// #define RPL_CREATED(client, datetime) ("003 " + client + " :This server was created " + datetime)
// #define RPL_MYINFO(client, available_user_modes, available_channel_modes) (client + " ft_irc 1.0 " + available_user_modes + available_channel_modes)

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

#define RPL_AWAY(client, nick, message) ("301 " + client + " " + nick + " :" + message)
#define RPL_USERHOST(client, replies) ("302 " + client + " :" + replies)
#define RPL_UNAWAY(client) ("305 " + client + " :You are no longer marked as being away")
#define RPL_NOWAWAY(client) ("306 " + client + " :You have been marked as being away")
#define RPL_WHOREPLY(client, channel, username, host, nick, flags, realname) ("352 " + client + ' ' + channel + ' ' + username + ' ' + host + " ft_irc " + nick + ' ' + flags + " :0 " + realname)
#define RPL_ENDOFWHO(client, mask) ("315 " + client + " " + mask + " :End of WHO list")
#define RPL_WHOISREGNICK(client, nick) ("307 " + client + " " + nick + " :has identified for this nick")
#define RPL_WHOISUSER(client, nick, username, host, realname) ("311 " + client + " " + nick + " " + username + " " + host + " * :" + realname)
#define RPL_WHOISSERVER(client, nick) ("312 " + client + " " + nick + " ft_irc :Custom server")
#define RPL_WHOISOPERATOR(client, nick) ("313 " + client + " " + nick + " :is an IRC operator")
#define RPL_WHOWASUSER(client, nick, username, host, realname) ("314 " + client + " " + nick + " " + username + " " + host + " * :" + realname)
#define RPL_WHOISIDLE(client, nick, secs, signon) ("317 " + client + " " + nick + " " + secs + " " + signon + " :seconds idle, signon time")
#define RPL_ENDOFWHOIS(client, nick) ("318 " + client + " " + nick + " :End of /WHOIS list")
#define RPL_WHOISCHANNELS(client, nick, channels) ("319 " + client + " " + nick + " :" + channels)

#define RPL_LISTSTART(client) ("321 " + client + " Channel :Users  Name")
#define RPL_LIST(client, channel, client_count, topic) ("322 " + client + " " + channel + " " + client_count + " :" + topic)
#define RPL_LISTEND(client) ("323 " + client + " :End of /LIST")

#define RPL_CHANNELMODEIS(client, channel, modestring, mode_arguments) ("324 " + client + " " + channel + " " + modestring + " " + mode_arguments)
#define RPL_CREATIONTIME(client, channel, creationtime) ("329 " + client + " " + channel + " " + creationtime)
#define RPL_WHOISACCOUNT(client, nick, account) ("330 " + client + " " + nick + " " + account + " :is logged in as")
#define RPL_NOTOPIC(client, channel) ("331 " + client + " " + channel + " :No topic is set")
#define RPL_TOPIC(client, channel) ("332 " + client + " " + channel + " :" + topic)
#define RPL_TOPICWHOTIME(client, channel, nick, setat) ("333 " + client + " " + channel + " " + nick + " " + setat)
#define RPL_INVITELIST(client, channel) ("336 " + client + " " + channel)
#define RPL_ENDOFINVITELIST(client) ("337 " + client + " :End of /INVITE list")


#endif