#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include "main.hpp"

class Channel
{
	private:
		
		/* Channel name */
		const std::string & _name;

		/* List of all users currently in the channel */
		std::vector<User *> _members;

		/* List of all users that are operators for this channel */
		std::vector<User *> _operators;

		/* Topic of the channel */
		std::string	_topic;

		/* If true, topic has been cleared.
		This allows for differenciating between no topic and an empty string as topic. */
		bool	_topic_is_set;

		/* If _ban_mode, list of all users that are banned from this channel */
		std::vector<User *> _banned;

		/* If true, users that are in the _banned list will not be able to join the channel */
		bool	_ban_mode;

		/* If _ban_except_mode, list of users that are exempt from being banned */
		std::vector<User *> _ban_except;
		
		/* If true, users that are in the _ban_except list will be able to join the channel even if banned*/
		bool	_ban_except_mode;

		/* If _limit_mode, max number of clients that can join the channel. */
		size_t	_capacity;

		/* If true, clients cannot join after there are already _capacity members */
		bool	_limit_mode;

		/* If _invite_mode, list of users that have been invited to the channel and can join */
		std::vector<User *> _invited;

		/* If true, clients cannot join unless they have been invited to the channel */
		bool	_invite_mode;

		/* If _invite_except_mode, list of users that are exempt from needing an invite to join */
		std::vector<User *> _invite_except;

		/* If true, users that are in the _invite_except list will be able to join the channel even without an invite */
		bool	_invite_except_mode;

		/* If _key_mode, key required to join the channel */
		std::string	_key;

		/* If true, users need to provide the correct key in order to join */
		bool	_key_mode;

		/* If true, users need privileges to send messages on the channel */
		bool	_moderated_mode;

		/* If true, the channel will not appear on LIST-type commands */
		bool	_secret_mode;

		/* If true, users need privileges to change the channel topic */
		bool	_protected_topic_mode;

		/* If true, users need to join the channel to send messages to it */
		bool	_no_external_messages_mode;

		/*  */
		bool	_find_mask(std::vector<User *> vect, User * user) const;

	public:
		
		Channel(const std::string & name);
		virtual	~Channel();

	/* -----ACCESSORS----- */

		/* Returns channel name */
		const std::string & getName() const;

		/* Returns channel topic */
		const std::string & getTopic() const;

		/* Returns if channel is in ban mode */
		bool	banMode() const;

		/* Returns if channel is in ban-except mode */
		bool	banExceptMode() const;

		/* Returns if channel is in limit mode */
		bool	limitMode() const;

		/* Returns if channel is in invite mode */
		bool	inviteMode() const;

		/* Returns if channel is in invite-except mode */
		bool	inviteExceptMode() const;

		/* Returns if channel requires a key to join */
		bool	keyMode() const;

		/* Returns if channel is in moderated mode */
		bool	moderatedMode() const;

		/* Returns if channel is in secret mode */
		bool	secretMode() const;

		/* Returns if only operators can change the topic */
		bool	protectedTopicMode() const;

		/* Returns if users need to join to send messages to the channel */
		bool	noExternalMessagesMode() const;

		/* Returns max number of members */
		size_t	capacity() const;

	/* -----ADDING USERS TO LISTS----- */

		/* Adds a user to the channel members without checking for permissions. */
		void	addMember(User *user);
		
		/* Adds a user to the operators without checking for permissions. */
		void	addOperator(User *user);

		/* Adds a user to the list of banned users. */
		void	banUser(User *user);

		/* Adds a user to the list of banned-exempt users. */
		void	addBanExcept(User *user);

		/* Adds a user to the list of invite-exempt users. */
		void	addInviteExcept(User *user);

		/* Adds a user to the list of invited users */
		void	addInvite(User *user);
		
	/* -----ATTRIBUTE CHECKS----- */

		/* Checks if user is a member */
		bool	isMember(User *user) const ;

		/* Checks if user is an operator */
		bool	isOp(User *user) const ;

		/* Checks if user is banned */
		bool	isBanned(User *user) const ;

		/* Checks if user is ban-exempt */
		bool	isBanExcept(User *user) const ;

		/* Checks if user has been invited */
		bool	isInvited(User *user) const ;

		/* Checks if user is invite-exempt */
		bool	isInviteExcept(User *user) const ;

		/* Returns true if the channel is at max capacity */
		bool	isFull() const;

		/* Returns true if the given string matches the key required to join */
		bool	checkKey(std::string s) const;
		
		/* Returns true if channel has a topic */
		bool	isTopicSet() const;
		
	/* -----COMMANDS----- */
		
		/*
		Tries to add user to the channel.
		Can return :
		- err_badchannelkey
		- err_bannedfromchan
		- err_channelisfull
		- err_inviteonlychan
		- err_noerror on success
		*/
		err_codes join(User *user, std::string s);

		/*
		Tries to remove user from the channel.
		Can return :
		- err_notonchannel
		- err_noerror on success
		*/
		err_codes part(User *user);
		
		/*
		Tries to change the channel's topic.
		Can return :
		- err_notonchannel
		- err_chanoprivsneeded
		- err_noerror on success
		*/
		err_codes changeTopic(User *user, std::string new_topic);

		/*
		Tries to add user to the list of invited users
		Can return :
		- err_notonchannel
		- err_chanoprivsneeded
		- err_useronchannel
		- err_noerror on success
		*/
		err_codes invite(User *from, User *to);

		/*
		Tries to kick user from the channel
		Can return :
		- err_notonchannel
		- err_chanoprivsneeded
		- err_useronchannel
		- err_noerror on success
		*/
		err_codes kick(User *from, User *to);

		/* Returns the list of names of the users on this channel, with prefixes.
		If a user is invisible, the requesting user has to be in the channel to view them.
		If a user is an operator, their name is prefixed by a '@' */
		std::vector<std::string> names(User *user) const;
};

#endif
