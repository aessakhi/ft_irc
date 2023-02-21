/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aessakhi <aessakhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/16 17:26:20 by ldesnoye          #+#    #+#             */
/*   Updated: 2023/02/21 13:27:18 by aessakhi         ###   ########.fr       */
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

size_t	Channel::capacity() const
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

void	Channel::invite(User * user)
{ _invited.push_back(user) ; }

/* -----ATTRIBUTE CHECKS----- */

bool	Channel::isMember(User *user) const
{ return std::find(_members.begin(), _members.end(), user) == _members.end() ; }

bool	Channel::isOp(User *user) const
{ return std::find(_operators.begin(), _operators.end(), user) == _operators.end() ; }

/* The 4 methods below might need to handle user masks */
bool	Channel::isBanned(User *user) const
{ return std::find(_banned.begin(), _banned.end(), user) == _banned.end() ; }

bool	Channel::isBanExcept(User *user) const
{ return std::find(_ban_except.begin(), _ban_except.end(), user) == _ban_except.end() ; }

bool	Channel::isInvited(User *user) const
{ return std::find(_invited.begin(), _invited.end(), user) == _invited.end() ; }

bool	Channel::isInviteExcept(User *user) const
{ return std::find(_invite_except.begin(), _invite_except.end(), user) == _invite_except.end() ; }

bool	Channel::isFull() const
{ return _members.size() == _capacity ; }

bool	Channel::checkKey(std::string s) const
{ return !(s.compare(_key)) ; }
