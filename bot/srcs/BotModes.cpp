#include "BotModes.hpp"

BotModes::BotModes() : parrot(false), greet(false)
{}

BotModes::~BotModes()
{}

BotModes::BotModes(BotModes const & src)
{ *this = src; }

BotModes & BotModes::operator=(BotModes const & src)
{
	parrot = src.parrot;
	greet = src.greet;
	
	return *this;
}
