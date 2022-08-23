/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abambi <abambi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/14 17:03:45 by hhanh             #+#    #+#             */
/*   Updated: 2022/08/18 09:10:28 by abambi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "Parse.hpp"
# include "Client.hpp"
# include <csignal>
# include <netdb.h>
# include <fcntl.h>
# include <sys/poll.h>
# include <arpa/inet.h>

# define MAX_EVENTS		10000
# define BUFFER_SIZE	512

class Server : public Parse
{
	public:
		Server(void);
		Server(const char* port, const char* password);
		~Server(void);

		void	start(void);
		void	stop(void);

	private:
		Server(const Server& src);
		Server&	operator=(const Server& rhs);

		const char*	_port;
		const char*	_passwd;
		int			_servfd;
		std::vector <pollfd> fds;

		void accept_new_client();
		void	init_server(void);
		void	main_loop(void);
		void	handle_client(int fd);
		void	remove_server(void);
		void	throw_error(const char* error);
};

#endif
