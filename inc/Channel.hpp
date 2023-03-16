#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include "main.hpp"

class Channel
{
	private:
		
		/* Channel name */
		const std::string & _name;

		/* Founder of the channel */
		User * _founder;

		/* List of all users currently in the channel */
		std::vector<User *> _members;

		/* Mode string */
		std::string	_modestring;

		/* List of all operator masks for this channel */
		std::vector<std::string> _operators;

		/* Topic of the channel */
		std::string	_topic;

		/* List of all user masks that are banned from this channel */
		std::vector<std::string> _banned;

		/* List of user masks that are exempt from being banned */
		std::vector<std::string> _ban_except;

		/* If _limit_mode, max number of clients that can join the channel. */
		size_t	_capacity;

		/* If true, clients cannot join after there are already _capacity members */
		bool	_limit_mode;

		/* If _invite_mode, list of users that have been invited to the channel and can join */
		std::vector<User *> _invited;

		/* If true, clients cannot join unless they have been invited to the channel */
		bool	_invite_mode;

		/* If _invite_except_mode, list of user masks that are exempt from needing an invite to join */
		std::vector<std::string> _invite_except;

		/* If _key_mode, key required to join the channel */
		std::string	_key;

		/* If true, users need to provide the correct key in order to join */
		bool	_key_mode;

		/* If _moderated_mode, list of user masks still able to talk */
		std::vector<std::string> _voiced;

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
		bool	_find_mask(std::vector<std::string> vect, User * user) const;

		/* Set _limit_mode value to state */
		void	_setLimitMode(bool state);

		/* Set _capacity value to limit */
		void	_setLimit(size_t limit);

		/* Set _key_mode value to state */
		void	_setKeyMode(bool state);

		/* Set _key value to new_key */
		void	_setKey(std::string new_key);

		/* Set _creatime to creation time*/
		time_t	_creatime;

		/* Set _topictime to topic update time */
		time_t	_topictime;

	public:
		
		Channel(const std::string & name);
		virtual	~Channel();

	/* ----------ACCESSORS---------- */

		/* Returns channel name */
		const std::string & getName() const;

		/* Returns channel modestring */
		std::string getModes() const;

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

		/* Returns vector of User * */
		std::vector<User *>		getUsers() const;

		/* Returns list of operator masks */
		std::vector<std::string>	getOperators() const;

		/* Returns list of banned masks */
		std::vector<std::string>	getBanned() const;

		/* Returns list of ban-exempt masks */
		std::vector<std::string>	getBanExcept() const;

		/* Returns list of invite-exempt masks */
		std::vector<std::string>	getInviteExcept() const;

		/* Returns list of voiced masks */
		std::vector<std::string>	getVoiced() const;

		/* Returns creation time of Channel */
		time_t getCreatime() const;

		/* Return topic edit time */
		time_t getTopictime() const;

	/* ----------ADDING USERS TO LISTS---------- */

		/* Adds a user to the channel members without checking for permissions. */
		void	addMember(User *user);
		
		/* Adds a user mask to the operators without checking for permissions. */
		void	addOperator(std::string user);

		/* Adds a user mask to the list of banned users. */
		void	banUser(std::string user);

		/* Adds a user mask to the list of banned-exempt users. */
		void	addBanExcept(std::string user);

		/* Adds a user mask to the list of invite-exempt users. */
		void	addInviteExcept(std::string user);

		/* Adds a user to the list of invited users */
		void	addInvite(User *user);

		/* Adds a user mask to the list of voice users */
		void	addVoiced(std::string user);

	/* ----------REMOVING USERMASKS FROM LISTS---------- */

		/* Removes an operator user mask. */
		void	removeOperator(std::string str);

		/* Removes an banned user mask. */
		void	unbanUser(std::string str);

		/* Removes an ban-exempt user mask. */
		void	removeBanExcept(std::string str);

		/* Removes an invited user mask. */
		void	removeInviteExcept(std::string str);

		/* Removes an operator user mask. */
		void	removeVoiced(std::string str);

	/* ----------ATTRIBUTE CHANGES---------- */

		/* Set _limit_mode to state and _capacity (if given) to value */
		void	setLimit(bool state, size_t value = 0);

		/* Set _invite_mode value to state */
		void	setInviteMode(bool state);

		/* Set _key_mode to state and _key (if given) to value */
		void	setKey(bool state, std::string value = "");

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

		/* Checks if user is the founder */
		bool	isFounder(User *user) const ;

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

		/* Checks if user is voiced */
		bool	isVoiced(User *user) const ;

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

		/* Kicks user from the channel */
		void kick(User *to);

		/* Returns the list of names of the users on this channel, with prefixes.
		If a user is invisible, the requesting user has to be in the channel to view them.
		If a user is an operator, their name is prefixed by a '@' */
		std::vector<std::string> namesVect(User *user) const;

		/* Returns the list of names as a string of format "[prefix]<nick>{ [prefix]<nick>}" */
		std::string namesStr(User *user) const;

		/* Sends a string to all joined members, adding a \\r\\n at the end. */
		void	sendToAllMembers(std::string msg) const;

		/* Overload to exclude a fd from receiving the msg */
		void	sendToAllMembers(std::string msg, int	&userfd) const;

		void	up_creatime();

		void	up_topictime();
};

#endif
