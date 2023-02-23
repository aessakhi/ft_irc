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

		/* List of all operator masks for this channel */
		std::vector<UserMask> _operators;

		/* Topic of the channel */
		std::string	_topic;

		/* List of all user masks that are banned from this channel */
		std::vector<UserMask> _banned;

		/* List of user masks that are exempt from being banned */
		std::vector<UserMask> _ban_except;

		/* If _limit_mode, max number of clients that can join the channel. */
		size_t	_capacity;

		/* If true, clients cannot join after there are already _capacity members */
		bool	_limit_mode;

		/* If _invite_mode, list of users that have been invited to the channel and can join */
		std::vector<User *> _invited;

		/* If true, clients cannot join unless they have been invited to the channel */
		bool	_invite_mode;

		/* If _invite_except_mode, list of user masks that are exempt from needing an invite to join */
		std::vector<UserMask> _invite_except;

		/* If _key_mode, key required to join the channel */
		std::string	_key;

		/* If true, users need to provide the correct key in order to join */
		bool	_key_mode;

		/* If _moderated_mode, list of user masks still able to talk */
		std::vector<UserMask> _voiced;

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

		/*  */
		bool	_find_mask(std::vector<UserMask> vect, User * user) const;

	public:
		
		Channel(const std::string & name);
		virtual	~Channel();

	/* ----------ACCESSORS---------- */

		/* Returns channel name */
		const std::string & getName() const;

		/* Returns channel topic */
		const std::string & getTopic() const;

		/* Returns if channel is in limit mode */
		bool	limitMode() const;

		/* Returns if channel is in invite mode */
		bool	inviteMode() const;

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

		/* Returns list of operator masks */
		std::vector<UserMask>	getOperators() const;

		/* Returns list of banned masks */
		std::vector<UserMask>	getBanned() const;

		/* Returns list of ban-exempt masks */
		std::vector<UserMask>	getBanExcept() const;

		/* Returns list of invite-exempt masks */
		std::vector<UserMask>	getInviteExcept() const;

		/* Returns list of voiced masks */
		std::vector<UserMask>	getVoiced() const;

	/* ----------ADDING USERS TO LISTS---------- */

		/* Adds a user to the channel members without checking for permissions. */
		void	addMember(User *user);
		
		/* Adds a user mask to the operators without checking for permissions. */
		void	addOperator(UserMask user);

		/* Adds a user mask to the list of banned users. */
		void	banUser(UserMask user);

		/* Adds a user mask to the list of banned-exempt users. */
		void	addBanExcept(UserMask user);

		/* Adds a user mask to the list of invite-exempt users. */
		void	addInviteExcept(UserMask user);

		/* Adds a user to the list of invited users */
		void	addInvite(User *user);

		/* Adds a user mask to the list of voice users */
		void	addVoice(UserMask user);

	/* ----------ATTRIBUTE CHANGES---------- */

		/* Set _limit_mode value to state */
		void	setLimitMode(bool state);

		/* Set _capacity value to limit */
		void	setLimit(size_t limit);

		/* Set _invite_mode value to state */
		void	setInviteMode(bool state);

		/* Set _key_mode value to state */
		void	setKeyMode(bool state);

		/* Set _key value to new_key */
		void	setKey(std::string new_key);

		/* Set _moderated_mode value to state */
		void	setModeratedMode(bool state);

		/* Set _secret_mode value to state */
		void	setSecretMode(bool state);

		/* Set _protected_topic_mode value to state */
		void	setProtectedTopicMode(bool state);

		/* Set _no_external_messages_mode value to state */
		void	setNoExternalMessagesMode(bool state);

		/* Set _topic value to new_topic */
		void	setTopic(std::string new_topic);

		/* Unsets topic */
		void	unsetTopic();

	/* ----------ATTRIBUTE CHECKS---------- */

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
		
	/* ----------COMMANDS---------- */
		
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
		std::vector<std::string> namesVect(User *user) const;

		/* Returns the list of names as a string of format "[prefix]<nick>{ [prefix]<nick>}" */
		std::string namesStr(User *user) const;
};

#endif
