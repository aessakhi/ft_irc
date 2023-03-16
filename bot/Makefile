CXX			=	c++
CXXFLAGS	=	-Wall -Wextra -Werror -std=c++98

NAME		=	ircbot

SRCS		=	Bot.cpp \
				bot_main.cpp \

OBJS		=	$(SRCS:.cpp=.o)

all:			$(NAME)

$(NAME):		$(OBJS)
				@echo -n "[\033[31mx\033[0m] Compiling $(NAME)\r"
				@$(CXX) -o ../$(NAME) $(OBJS) $(CXXFLAGS)
				@echo "[\033[92m✓\033[0m] \033[94mCompiled $(NAME) !\033[0m"


%.o :	%.cpp
		@mkdir -p $(@D)
		@echo -n "[\033[31mx\033[0m] Compiling $<\r"
		@$(CXX) $(CXXFLAGS) -c $< -o $@
		@echo "[\033[32m✓\033[0m] Compiled $<  "


clean:
	@echo "\033[31mDeleting bot .o files\033[0m"
	@$(RM) $(OBJS)

fclean:	clean
		@echo "\033[31mDeleting $(NAME)\033[0m"
		@$(RM) ../$(NAME)
