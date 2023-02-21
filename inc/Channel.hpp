/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldesnoye <ldesnoye@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/16 16:33:22 by ldesnoye          #+#    #+#             */
/*   Updated: 2023/02/21 10:25:36 by ldesnoye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include "main.hpp"
#include "User.hpp"

class Channel
{
	private:
		
		/* Channel name */
		const std::string & _name;

		/* List of all users currently in the channel */
		std::vector<User *> _members;

		/* List of all users that are operators for this channel */
		std::vector<User *> _operators;

		/* If _ban_mode, list of all users that are banned from this channel */
		std::vector<User *> _banned;

		/* If true, users that are in the _banned list will not be able to join the channel */
		bool	_ban_mode;

		/* If _ban_except_mode, list of users that are exempt from being banned */
		std::vector<User *> _ban_except;
		
		/* If true, users that are in the _ban_except list will be able to join the channel even if banned*/
		bool	_ban_except_mode;

		/* If _limit_mode, max number of clients that can join the channel. */
		int		_capacity;

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

	public:
		
		Channel(const std::string & name);
		virtual	~Channel();

	/* -----ACCESSORS----- */

		std::string	getName() const;

		bool	banMode() const;
		bool	banExceptMode() const;
		bool	limitMode() const;
		bool	inviteMode() const;
		bool	inviteExceptMode() const;
		bool	keyMode() const;
		bool	moderatedMode() const;
		bool	secretMode() const;
		bool	protectedTopicMode() const;
		bool	noExternalMessagesMode() const;

		int		capacity() const;

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
		
};

#endif