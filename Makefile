# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: abambi <abambi@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/03/14 17:00:03 by hhanh             #+#    #+#              #
#    Updated: 2022/08/18 08:58:43 by abambi           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME =		ircserv

CXXFLAGS =	-Wall -Wextra -Werror -std=c++98 

INCDIR =	includes

SRCS =		srcs/Channel.cpp \
			srcs/Client.cpp \
			srcs/Commands.cpp \
			srcs/Parse.cpp \
			srcs/ircserv.cpp \
			srcs/Server.cpp

MAKEFILE =	Makefile

CXX =		c++

all:		$(NAME)

$(NAME):	$(SRCS) $(MAKEFILE) $(INCDIR)/*.hpp
			$(CXX) $(CXXFLAGS) -I$(INCDIR) $(SRCS)  -o $(NAME)

clean:
			rm -rf $(OSFS)

fclean:		clean
			rm -rf $(NAME)

re:			fclean all

show:
			@echo "Hello from our happy IRC!!!"

.PHONY:		all re clean fclean show