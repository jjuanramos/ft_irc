# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: cmunoz-g <cmunoz-g@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/25 10:27:14 by juramos           #+#    #+#              #
#    Updated: 2024/11/25 12:16:37 by cmunoz-g         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = irc
CXX = clang++
CXXFLAGS = -Wall -Werror -Wextra -std=c++98
FOLDER = srcs/
OBJFOLDER = obj/
INCLUDES = includes/
SRCS = main.cpp classes/Server.cpp classes/Message.cpp
OBJS = $(SRCS:%.cpp=$(OBJFOLDER)%.o)

$(OBJFOLDER)%.o: $(FOLDER)%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -I$(INCLUDES) -c $< -o $@

$(NAME): $(OBJS)
	$(CXX) -o $(NAME) $(OBJS)

all: $(NAME)

clean:
	@rm -rf $(OBJFOLDER)

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re