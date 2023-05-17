NAME			=	webserv
SRC_DIR			=	src
OBJ_DIR			=	obj

CXX				= 	c++
CXXFLAGS		= 	-g3 -Wall -Wextra -Werror -pedantic -std=c++98
INC				=	-I./inc

SOURCE			=	main.cpp			\
					Request.cpp			\
					WebServer.cpp		\
					Server.cpp			\
					Response.cpp		\
					config/Config.cpp	\
					config/Array.cpp	\
					config/Value.cpp	\
					config/Object.cpp	\
					Cgi.cpp				\
					MimeTypes.cpp		\
					utils.cpp			\


OBJECT			=	$(patsubst %.cpp, $(OBJ_DIR)/%.o, $(SOURCE))
RM				=	rm -rf

all: $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -Iinc -c $< -o $@

$(NAME): $(OBJECT)
	$(CXX) $(CXXFLAGS) $(OBJECT) -o $(NAME)

clean:
	$(RM) $(OBJ_DIR)

fclean: clean
	$(RM) $(NAME)

$(OBJ_DIR):
	@mkdir -p $(dir $(OBJECT))

re: fclean all

.PHONY: all clean fclean re
