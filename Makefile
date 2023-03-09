NAME = ircserv

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98

SRC_DIR =		srcs

# CLASSES
SRC_NAMES +=	Channel.cpp \
				Command.cpp \
				Mode.cpp \
				Server.cpp \
				User.cpp \

# COMMANDS
SRC_NAMES +=	CAP.cpp \
				MODE.cpp \
				MODE-channel.cpp \
				MODE-user.cpp \
				NICK.cpp \
				PASS.cpp \
				PART.cpp \
				USER.cpp \
				PING.cpp \
				PRIVMSG.cpp \
				AWAY.cpp \
				JOIN.cpp \
				TIME.cpp \
				TOPIC.cpp \
				INFO.cpp \
				VERSION.cpp \
				NAMES.cpp \
				QUIT.cpp \

# OTHER FILES
SRC_NAMES +=	main.cpp \
				utils.cpp \
				print.cpp \

SRCS =	$(addprefix $(SRC_DIR)/, $(SRC_NAMES))

OBJ_DIR =	objs
OBJ_NAMES	= $(SRC_NAMES:.cpp=.o)
OBJS		= $(addprefix $(OBJ_DIR)/, $(OBJ_NAMES))

INC_DIR =	inc

# CLASSES
INC_NAMES +=	Channel.hpp \
				Command.hpp \
				Mode.hpp \
				Server.hpp \
				User.hpp \

# OTHER FILES
INC_NAMES =	main.hpp \
			codes.hpp \
			colors.hpp \
			commandlist.hpp \
			exceptions.hpp \

INC = $(addprefix $(INC_DIR)/, $(INC_NAMES))

RM =	rm -rf

$(NAME):	$(OBJS)
			$(CXX) -o $(NAME) $(OBJS) $(CXXFLAGS)

$(OBJ_DIR)/%.o :	$(SRC_DIR)/%.cpp $(INC) | $(OBJ_DIR)
					$(CXX) $(CXXFLAGS) -c $< -o $@ -I./$(INC_DIR)/

$(OBJ_DIR) :
				mkdir -p $(OBJ_DIR)

all: $(NAME)

clean:
	$(RM) $(OBJS)
	$(RM) $(OBJ_DIR)

fclean:	clean
	$(RM) $(NAME)

re: clean all

.PHONY: all clean re fclean
