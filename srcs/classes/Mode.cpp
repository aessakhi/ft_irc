#include "Mode.hpp"

Mode::Mode()
{}

Mode::Mode(bool add, char mode) : _add(add), _mode(mode)
{}

Mode::Mode(bool add, char mode, std::string arg) : _add(add), _mode(mode), _arg(arg)
{}

Mode::~Mode()
{}

Mode::Mode(Mode const & src) : _add(src._add), _mode(src._mode), _arg(src._arg)
{}

Mode & Mode::operator=(Mode const & src)
{
	_add = src._add;
	_mode = src._mode;
	_arg = src._arg;
	return *this;
}

bool Mode::getAdd() const
{
	return _add;
}

char Mode::getMode() const
{
	return _mode;
}

std::string Mode::getArg() const
{
	return _arg;
}

std::string Mode::str() const
{
	std::string str;
	if (_add)
		str += '+';
	else
		str += '-';
	str += _mode;
	if (!_arg.empty())
		str += " " + _arg;
	return str;
}