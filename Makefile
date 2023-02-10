# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ldesnoye <ldesnoye@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/02/02 21:22:12 by aessakhi          #+#    #+#              #
#    Updated: 2023/02/10 12:49:24 by ldesnoye         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ircserv

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98

SRC_DIR =	srcs
SRC_NAMES =	main.cpp \
			Server.cpp \

SRCS =	$(addprefix $(SRC_DIR)/, $(SRC_NAMES))

OBJS = $(SRCS:.cpp=.o)

INC_DIR =	include
INC_NAMES =	main.hpp \
			Server.hpp \

INCS = $(addprefix $(INC_DIR)/, $(INC_NAMES))

RM =	rm -rf

$(NAME):	$(OBJS)
			$(CXX) -o $(NAME) $(OBJS) $(CXXFLAGS)

%.o :	%.cpp $(INCS)
		$(CXX) $(CXXFLAGS) -c $< -o $@ -I./include/

all: $(NAME)

clean:
	$(RM) $(OBJS)

fclean:	clean
		$(RM) $(NAME)

re: clean all

.PHONY: all clean re fclean
