/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abambi <abambi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/11 17:21:12 by abambi            #+#    #+#             */
/*   Updated: 2022/08/17 17:09:20 by abambi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "Client.hpp"
# include <iostream>
# include <string>
# include <list>

# define CLIENT_LIMIT 3

class Client;

class Channel
{
	private:
		std::string			_name;
		std::list<Client*>	_clients;
		int					_client_limit;
		int					_client_counter;

	public:
		Channel(void);
		Channel(const std::string& name);
		Channel(const Channel& src);
		Channel&	operator=(const Channel& rhs);
		~Channel(void);

		void				set_name(const std::string& name);
		void				add_client(Client& client);
		void				set_client_counter(int num);

		std::string&		get_name(void);
		std::list<Client*>&	get_clients(void);
		size_t				get_number_clients(void);
		size_t				get_client_counter(void);
		std::string			get_names(void);

		void				del_client(Client& client);
};

#endif
