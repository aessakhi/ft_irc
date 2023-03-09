#ifndef MODE_HPP
# define MODE_HPP

#include "main.hpp"

class Mode
{
	private:

		bool			_add;
		char			_mode;
		std::string		_arg;
	
	public:

		Mode();
		Mode(bool add, char mode);
		Mode(bool add, char mode, std::string arg);

		virtual ~Mode();
		Mode(Mode const & src);
		Mode & operator=(Mode const & src);

	/* ----------ACCESSORS---------- */

		bool getAdd() const;
		char getMode() const;
		std::string getArg() const;
		std::string str() const;
};

#endif