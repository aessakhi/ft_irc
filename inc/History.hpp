#ifndef HISTORY_HPP
 #define HISTORY_HPP
 #include "main.hpp"

class History
{
	private:
		History();

		std::string	_nickname;
		std::string _username;
		std::string _hostname;
		std::string _realname;

	public:
		History(User &user);
		~History();
		std::string getNickname() const;
		std::string getUsername() const;
		std::string getHostname() const;
		std::string getRealname() const;
};

#endif