/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abambi <abambi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/11 17:23:29 by abambi            #+#    #+#             */
/*   Updated: 2022/08/17 17:05:48 by abambi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel(void) : 
	_name(""), _clients(), _client_limit(CLIENT_LIMIT), _client_counter(0)
{
	std::cout << "Hi!\n";
}

Channel::Channel(const std::string &name) : 
	_name(name), _clients(), _client_limit(CLIENT_LIMIT), _client_counter(0)
{
	std::cout << "Hello!\n";
}

Channel::Channel(const Channel &src)
{
	*this = src;
}

Channel&	Channel::operator=(const Channel &src)
{
	if (this != &src)
	{
		_name = src._name;
		_clients = src._clients;
		_client_limit = src._client_limit;
		_client_counter = src._client_counter;
	}
	return *this;
}

Channel::~Channel(void) {}

void		Channel::set_name(const std::string& name)	{ _name = name; }
void		Channel::add_client(Client& client)			{ _clients.push_back(&client); }
void		Channel::set_client_counter(int num)		{ _client_counter = num; }

std::string&		Channel::get_name(void)				{ return _name; }
std::list<Client*>&	Channel::get_clients(void)			{ return _clients; }
size_t				Channel::get_client_counter(void)	{ return _client_counter; }
size_t				Channel::get_number_clients(void)
{
	size_t								i;
	std::list<Client*>::const_iterator	it;

	i = 0;
	for (it = get_clients().begin(); *it != get_clients().back(); ++it)
		++i;
	return i;
}

std::string			Channel::get_names(void)
{
	std::string							ret;
	std::list<Client*>::const_iterator	it;

	ret = get_clients().back()->get_nickname();
	for (it = get_clients().begin(); *it != get_clients().back(); ++it)
		ret += " " + (**it).get_nickname();
	return ret;
}

void				Channel::del_client(Client& client) { _clients.remove(&client); }
