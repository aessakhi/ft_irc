CXX			=	c++
CXXFLAGS	=	-Wall -Wextra -Werror -std=c++98

NAME		=	../ircbot

SRC_DIR 	= srcs

SRC_NAMES	+=	Bot.cpp \
				BotCommand.cpp \
				BotModes.cpp \
				Command.cpp \
				bot_main.cpp \
				utils.cpp \

SRCS		= $(addprefix $(SRC_DIR)/, $(SRC_NAMES))

OBJ_DIR 	= objs
OBJ_NAMES	= $(SRC_NAMES:.cpp=.o)
OBJS		= $(addprefix $(OBJ_DIR)/, $(OBJ_NAMES))

INC_DIR =	inc

INC_NAMES +=	Bot.hpp \
				BotCommand.hpp \
				BotModes.hpp \
				Command.hpp \
				colors.hpp \
				main.hpp \
				exceptions.hpp \

RM =	rm -rf

all:	$(NAME)

$(NAME):	$(OBJS)
			@echo -n "[\033[31mx\033[0m] Compiling $(NAME)\r"
			@$(CXX) -o $(NAME) $(OBJS) $(CXXFLAGS)
			@echo "[\033[92m✓\033[0m] \033[94mCompiled $(NAME) !\033[0m"

$(OBJ_DIR)/%.o :	$(SRC_DIR)/%.cpp $(INC)
					@mkdir -p $(@D)
					@echo -n "[\033[31mx\033[0m] Compiling $<\r"
					@$(CXX) $(CXXFLAGS) -c $< -o $@ -I$(INC_DIR)
					@echo "[\033[32m✓\033[0m] Compiled $<  "

clean:
	@echo "\033[31mDeleting bot .o files\033[0m"
	@$(RM) $(OBJ_DIR)

fclean:	clean
		@echo "\033[31mDeleting $(NAME)\033[0m"
		@$(RM) $(NAME)

re:	fclean all

.PHONY: all clean re fclean