/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldesnoye <ldesnoye@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/16 17:26:20 by ldesnoye          #+#    #+#             */
/*   Updated: 2023/02/16 17:54:36 by ldesnoye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel(const std::string & name) : _name(name)
{}

Channel::~Channel() {}

/* -----ACCESSORS----- */

std::string	Channel::getName() const
{ return _name ; }

bool	Channel::banMode() const
{ return _ban_mode ; }

bool	Channel::banExceptMode() const
{ return _ban_except_mode ; }

bool	Channel::limitMode() const
{ return _limit_mode ; }

bool	Channel::inviteMode() const
{ return _invite_mode ; }

bool	Channel::inviteExceptMode() const
{ return _invite_except_mode ; }

bool	Channel::keyMode() const
{ return _key_mode ; }

bool	Channel::moderatedMode() const
{ return _moderated_mode ; }

bool	Channel::secretMode() const
{ return _secret_mode ; }

bool	Channel::protectedTopicMode() const
{ return _protected_topic_mode ; }

bool	Channel::noExternalMessagesMode() const
{ return _no_external_messages_mode ; }

int		Channel::capacity() const
{ return _capacity ; }

/* -----ADDING USERS TO LISTS----- */

void	Channel::addMember(User *user)
{ _members.push_back(user); }

void	Channel::addOperator(User *user)
{ _operators.push_back(user); }

void	Channel::banUser(User *user)
{ _banned.push_back(user); }

void	Channel::addBanExcept(User *user)
{ _ban_except.push_back(user); }

void	Channel::addInviteExcept(User *user)
{ _invite_except.push_back(user); }

