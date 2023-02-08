# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aessakhi <aessakhi@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/02/02 21:22:12 by aessakhi          #+#    #+#              #
#    Updated: 2023/02/02 22:15:14 by aessakhi         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ircserv

OBJS = $(SRCS:.cpp=.o)
CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98

SRCS =	srcs/main.cpp \
		srcs/Server/Server.cpp

RM =	rm -rf

$(NAME):	$(OBJS)
			$(CXX) -o $(NAME) $(OBJS) $(CXXFLAGS)

all: $(NAME)

clean:
	$(RM) $(OBJS)

fclean:	clean
		$(RM) $(NAME)

re: clean all

.PHONY: all clean re fclean
