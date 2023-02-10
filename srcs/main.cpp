/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldesnoye <ldesnoye@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/02 21:24:29 by aessakhi          #+#    #+#             */
/*   Updated: 2023/02/10 17:57:54 by ldesnoye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.hpp"

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

	if (ircserv.begin_listening())
	{
		return -1;
	}

	/*----------------------LOOP----------------------*/

	struct sockaddr_storage their_addr;
	socklen_t sin_size;
	int new_fd;

	while (1)
	{
		sin_size = sizeof their_addr;
		new_fd = accept(ircserv._listen_fd, (struct sockaddr *)&their_addr, &sin_size);
		fcntl(new_fd, F_SETFD, O_NONBLOCK);
		if (new_fd != -1)
		{
			std::cout << "Accepted connection" << std::endl;
			// Basic str receiver and sender
			char buf[1024];
			int nread;
			while (1)
			{
				nread = recv(new_fd, buf, 1024, 0);
				buf[nread] = 0;
				if (nread)
					std::cout << "--[" << buf << "]--" << std::endl;
				if (buf[0] == 'e' && buf[1] == 'x' && buf[2] == 'i' && buf[3] == 't')
					break;
				send(new_fd, buf, nread, 0);
			}
			
			std::cout << "closing fd" << std::endl;
			close(new_fd);
		}
		else
		{
			std::cerr << "Error" << std::endl;
			exit(-1);
		}
	}
}
