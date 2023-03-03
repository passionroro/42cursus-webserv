NAME			=	webserv
SRC_DIR			=	src
OBJ_DIR			=	obj
SOURCE			=	main.cpp			\
					Request.cpp\
					WebServer.cpp		\
					Server.cpp			\
					Config.cpp			\
					Response.cpp		\


CXX				= 	c++
CXXFLAGS		= 	-Wall -Wextra -Werror -pedantic -std=c++98
INC				=	-I./inc
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
	mkdir -p $(dir $(OBJECT))

re: fclean all

.PHONY: all clean fclean re
