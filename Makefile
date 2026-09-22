NAME = webserv

RESET		=	\033[0m
GREEN		=	\033[32m
YELLOW		=	\033[33m
BLUE		=	\033[34m
RED			=	\033[31m
UP			=	\033[A
CUT			=	\033[K

CXX = c++
FLAGS = -Wall -Wextra -Werror -std=c++98 -g3 -fPIE
LDFLAGS = -pie
RM = rm -rf

CONFIG_PARSER_DIR = src/config_parser
REQUEST_PARSER_DIR = src/request
REQUEST_RESPONSE_DIR = src/response
SERVER_DIR = src/server
CGI = src/CGI
SRC = src/main.cpp \
      $(CONFIG_PARSER_DIR)/Print.cpp \
      $(CONFIG_PARSER_DIR)/Tokenization.cpp \
      $(CONFIG_PARSER_DIR)/read.cpp \
      $(CONFIG_PARSER_DIR)/Parser.cpp \
      $(CONFIG_PARSER_DIR)/Config.cpp \
	  $(SERVER_DIR)/Server.cpp \
	  $(SERVER_DIR)/Client.cpp \
	  $(SERVER_DIR)/Helper.cpp \
	  $(SERVER_DIR)/clientHealper.cpp \
	  $(CGI)/CGIhandler.cpp \
	  src/sessions/sessions.cpp\
	  $(REQUEST_PARSER_DIR)/Request.cpp\
	  $(REQUEST_PARSER_DIR)/RequestHandler.cpp\
	  $(REQUEST_RESPONSE_DIR)/Response.cpp

OBJ_DIR = obj/
OBJ = $(SRC:src/%.cpp=$(OBJ_DIR)%.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(FLAGS) $(LDFLAGS) -o $(NAME) $(OBJ)

$(OBJ_DIR)%.o: src/%.cpp
	@mkdir -p $(dir $@)
	@echo "$(YELLOW)Compiling [$@]...$(RESET)"
	@$(CXX) $(FLAGS) -c $< -o $@
	@printf "$(UP)$(CUT)"
	@echo "$(GREEN)Finished [$@]$(RESET)"
	@printf "$(UP)$(CUT)"

clean:
	$(RM) $(OBJ_DIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: clean fclean re all

.SECONDARY: $(OBJS)

run: re
	@./$(NAME)
