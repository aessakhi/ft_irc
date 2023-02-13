/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aessakhi <aessakhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/02 21:24:29 by aessakhi          #+#    #+#             */
/*   Updated: 2023/02/13 15:30:28 by aessakhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.hpp"
#include "Server.hpp"

/*
argv[0] should be the requested port,
argv[1] should be the server password.
*/
int main(int argc, char **argv)
{
	if (program_arguments_check(argc, argv))
	{
		return -1;
	}

	Server ircserv(argv[1], argv[2]);

	ircserv.init();
}
