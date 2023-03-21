NAME = ircserv

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98

SRC_DIR 	= srcs
CMD_DIR		= cmds
CLASS_DIR	= classes

# CLASSES
CLASS_NAMES	+=	Channel.cpp \
				Command.cpp \
				Mode.cpp \
				Server.cpp \
				User.cpp \
				History.cpp \

# COMMANDS
CMD_NAMES	+=	AWAY.cpp \
				CAP.cpp \
				INFO.cpp \
				INVITE.cpp \
				JOIN.cpp \
				KICK.cpp \
				KILL.cpp \
				LIST.cpp \
				LUSERS.cpp \
				MODE-channel.cpp \
				MODE-user.cpp \
				MODE.cpp \
				NAMES.cpp \
				NICK.cpp \
				NOTICE.cpp \
				OPER.cpp \
				PART.cpp \
				PASS.cpp \
				PING.cpp \
				PRIVMSG.cpp \
				QUIT.cpp \
				TIME.cpp \
				TOPIC.cpp \
				USER.cpp \
				VERSION.cpp \
				WALLOPS.cpp \
				WHO.cpp \
				WHOIS.cpp \
				WHOWAS.cpp \

# OTHER FILES
SRC_NAMES	+=	main.cpp \
				utils.cpp \
				print.cpp \

CLASSES		= $(addprefix $(CLASS_DIR)/, $(CLASS_NAMES))

CMDS		= $(addprefix $(CMD_DIR)/, $(CMD_NAMES))

SRC_NAMES	+= $(CMDS)
SRC_NAMES	+= $(CLASSES)

SRCS		= $(addprefix $(SRC_DIR)/, $(SRC_NAMES))

OBJ_DIR 	= objs
OBJ_NAMES	= $(SRC_NAMES:.cpp=.o)
OBJS		= $(addprefix $(OBJ_DIR)/, $(OBJ_NAMES))

INC_DIR =	inc

# CLASSES
INC_NAMES +=	Channel.hpp \
				Command.hpp \
				Mode.hpp \
				Server.hpp \
				User.hpp \
				History.hpp \

# OTHER FILES
INC_NAMES +=	main.hpp \
				codes.hpp \
				colors.hpp \
				commandlist.hpp \
				exceptions.hpp \
				print.hpp \
				utils.hpp \

INC = $(addprefix $(INC_DIR)/, $(INC_NAMES))

RM =	rm -rf

all:	$(NAME) bot

$(NAME):	$(OBJS)
			@echo -n "[\033[31mx\033[0m] Compiling $(NAME)\r"
			@$(CXX) -o $(NAME) $(OBJS) $(CXXFLAGS)
			@echo "[\033[92m✓\033[0m] \033[94mCompiled $(NAME) !\033[0m"

$(OBJ_DIR)/%.o :	$(SRC_DIR)/%.cpp $(INC)
					@mkdir -p $(@D)
					@echo -n "[\033[31mx\033[0m] Compiling $<\r"
					@$(CXX) $(CXXFLAGS) -c $< -o $@ -I$(INC_DIR)
					@echo "[\033[32m✓\033[0m] Compiled $<  "

clean: botclean
	@echo "\033[31mDeleting .o files\033[0m"
	@$(RM) $(OBJ_DIR)

fclean:	clean botfclean
	@echo "\033[31mDeleting $(NAME)\033[0m"
	@$(RM) $(NAME)

re:	fclean all

bot:
	@make -s -C bot

botclean:
	@make -s -C bot clean

botfclean:
	@make -s -C bot fclean

.PHONY: all clean re fclean bot