/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abambi <abambi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/14 17:03:30 by hhanh             #+#    #+#             */
/*   Updated: 2022/08/18 09:10:23 by abambi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

bool	g_status;

Server::Server(void) \
	: _port("6667"), _passwd("1234"), _servfd(-1) {}

Server::Server(const char* port, const char* password) \
	: _port(port), _passwd(password), _servfd(-1) {}

Server::~Server(void)
{
	remove_server();
}

void	sighandler(int signum)
{
	if (signum == SIGINT)
	{
		g_status = false;
		std::cout << '\n';
	}
}

void	Server::start(void)
{
	g_status = true;
	signal(SIGINT, &sighandler);
	init_server();
	main_loop();
}

void	Server::stop(void)
{
	g_status = false;
}

void	Server::init_server(void)
{
	addrinfo	hints;
	addrinfo*	servinfo;
	addrinfo*	p;
	int			optval = 1;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	if (getaddrinfo("127.0.0.1", _port, &hints, &servinfo) != 0)
		throw_error("Error: getaddrinfo");
	for (p = servinfo; p != NULL; p = p->ai_next)
	{
		_servfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
		if (_servfd < 0)
			continue ;
		fcntl(_servfd, F_SETFL, O_NONBLOCK); 
		if (setsockopt(_servfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
		{
			freeaddrinfo(servinfo);
			throw_error("Error: setsockopt");
		}
		if (bind(_servfd, p->ai_addr, p->ai_addrlen) < 0)
		{
			close(_servfd);
			continue ;
		}
		break ;
	}
	freeaddrinfo(servinfo);
	if (!p)
		throw_error("Error: socket");
	if (listen(_servfd, SOMAXCONN) < 0)
		throw_error("Error: listen");
}

void Server::accept_new_client()
{
	int			fd;
	sockaddr	addr;
	socklen_t	addrlen;
	char		buf[INET6_ADDRSTRLEN];
	std::string	address;
	int			port;

	memset(&addr, 0, sizeof(addr));
	addrlen = sizeof(addr);
	fd = accept(_servfd, &addr, &addrlen);
	if (fd < 0)
		throw_error("Error: accept");
	fcntl(fd, F_SETFL, O_NONBLOCK);
	pollfd client_fd = {fd, POLLIN, 0};
    fds.push_back(client_fd);
	memset(buf, 0, INET6_ADDRSTRLEN);
	if (addr.sa_family == AF_INET)
	{
		address = inet_ntoa(((sockaddr_in*)&addr)->sin_addr);
		port = ntohs(((sockaddr_in*)&addr)->sin_port);
	}
	else
	{
		address = inet_ntop(AF_INET6, &(((sockaddr_in6*)&addr)->sin6_addr), buf, INET6_ADDRSTRLEN);
		port = ntohs(((sockaddr_in6*)&addr)->sin6_port);
	}
	_clients[fd] = Client(fd, address, _passwd);
	std::cout << "New client #fd" << fd << " from " << address << ":" << port << '\n';

}

void Server::main_loop()
{
    int  i, poll_ret = 0;
    pollfd server_fd = {_servfd, POLLIN, 0};
    fds.push_back(server_fd);
	while (g_status)
	{
		poll_ret = poll(&fds[0], fds.size(), -1);
		if (poll_ret < 0 && g_status)
			throw_error("Error: poll");	
		i = 0;
		while (poll_ret > 0)
		{
			if ((fds[i].revents & POLLIN) && !(fds[i].revents & POLLHUP))
			{
				if (fds[i].fd == _servfd)
				{
					accept_new_client();
					poll_ret -= 1;
				}
				else 
				{
					handle_client(fds[i].fd);
					poll_ret -= 1;
				}
			}
			else if (fds[i].revents & POLLHUP)
			{
				std::cout << "Client #fd" << _clients[fds[i].fd].get_fd() << " gone away\n";
				close(_clients[fds[i].fd].get_fd());
				_clients.erase(_clients[fds[i].fd].get_fd());
				fds.erase(fds.begin() + i);
				poll_ret -= 1;
			}	
			else if (fds[i].revents & POLLNVAL)
			{
				fds.erase(fds.begin() + i);
				poll_ret -= 1;
			}	
			++i;
		}	
	}
	remove_server();
}

void	Server::handle_client(int fd)
{
	int		ret;
	char	buf[BUFFER_SIZE];
	Client&	client = _clients[fd];

	ret = recv(fd, buf, BUFFER_SIZE, MSG_DONTWAIT);
	if (!ret)
	{
		client.get_buffer().clear();
		client.get_buffer().append("quit\r\n", 6);
	}
	else
		client.get_buffer().append(buf, ret);
	exec_command(client);
}

void	Server::remove_server(void)
{
	std::map<int, Client>::iterator	it;

	for (it = _clients.begin(); it != _clients.end(); ++it)
		close(it->first);
	close(_servfd);
}

void	Server::throw_error(const char* error)
{
	remove_server();
	throw std::runtime_error(error);
}
