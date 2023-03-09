#ifndef PRINT_HPP
# define PRINT_HPP

#include "main.hpp"

/* Prints a std::vector\<std::string> element by element. */
void	printVect(std::ostream & out, std::vector<std::string> vect);

/* Print s on std::cerr in red. */
void	printError(std::string s);

/* Print sent replies on std::cout in blue. */
void	printReply(std::string s);

/* Print command function calls on std::cout in yellow. */
void	printFcall(std::string s);

/* Print n chars of received messages on std::cout in blue without trailing \\r\\n. */
void	printRecv(char * s, size_t n);

/* Print n char of received messages on std::cout in blue without trailing \\r\\n. */
void	printRecv(std::string s, size_t n);

/* Print cmd on std::cout in green detailing the different attributes. */
void	printSep(Command cmd);

/* Print vect on std::cout in green with elements visually separated. */
void	printSep(std::vector<std::string> vect);

#endif