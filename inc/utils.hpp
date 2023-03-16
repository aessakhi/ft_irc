#ifndef UTILS_HPP
# define UTILS_HPP

#include "main.hpp"

/*	Basic check for input arguments.
	Returns 0 if arguments are formatted correctly, -1 otherwise. */
int program_arguments_check(int argc, char **argv);

/* Check if the cmd is CAP, PASS, USER or NICK */
bool isAuthcmd(const std::string &cmd);

/* Splits str on delimiter.
Erases the beginning of str if there is a \\r\\n in str. */
std::vector<std::string> split(std::string *str, std::string delimiter);

/* Splits str on delimiter. */
std::vector<std::string> split(std::string str, std::string delimiter);

/* Returns a Command created from s.
s should be of format "prefix command command-parameters\\r\\n".
If there is a prefix, s begins with ':'.
command-parameters can be terminated by a message containing spaces, in which case it begins by ':'. */
Command	splitCmd(std::string s);

/* Returns true if s1 and s2 are similar when interpreting wildcards */
bool	wildcompare(std::string s1, std::string s2);

/* Returns true if s1 and s2 are similar when interpreting wildcards */
bool	wildcompare(const char * s1, const char * s2);

/* Returns true if s1 and s2 are similar when interpreting wildcards on s1 only */
bool	sided_wildcompare(std::string s1, std::string s2);

/* Returns true if s1 and s2 are similar when interpreting wildcards on s1 only */
bool	sided_wildcompare(const char * s1, const char * s2);

/* Returns a copy of s without the trailing \\r\\n */
std::string	no_crlf(std::string s);

/* Returns the uppercase version of s. */
std::string toupper(std::string s);

/* Basic split function. */
std::vector<std::string> actual_split(std::string str, std::string delimiter);

#endif
