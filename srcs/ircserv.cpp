/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abambi <abambi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/14 17:12:17 by hhanh             #+#    #+#             */
/*   Updated: 2022/08/18 09:20:33 by abambi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <iostream>

int	main(int ac, char** av)
{
	if (ac != 3)
	{
		std::cerr << "Error: need two arguments (after ./ircserv): <port> <password>\n";
		return (1);
	}

	Server	server(av[1], av[2]);

	try
	{
		server.start();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}

	return (0);
}
