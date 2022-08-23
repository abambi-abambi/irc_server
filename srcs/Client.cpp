/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abambi <abambi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/19 17:09:03 by abambi            #+#    #+#             */
/*   Updated: 2022/08/18 22:06:55 by abambi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(void) : _fd(-1), _address("invalid"), _password("none"), _status(false), \
	_buffer("invalid"), _nickname("invalid"), _username("invalid"), _mode("invalid"), \
	_unused_host_name("unused"), _realname("invalid"), _n_channels(0), \
	_bot(false) {}

Client::Client(int fd, const std::string& address, const std::string& password) \
	: _fd(fd), _address(address), _password(password), _status(false), \
	_buffer(""), _nickname(""), _username(""), _mode(""), \
	_unused_host_name("unused"), _realname(""), _n_channels(0), \
	_bot(false) {}

Client::Client(const Client& src)
{
	*this = src;
}

Client&	Client::operator=(const Client &src)
{
	if (this != &src)
	{
		_fd = src._fd;
		_address = src._address;
		_status = src._status;
		_buffer = src._buffer;
		_password = src._password;
		_nickname = src._nickname;
		_username = src._username;
		_mode = src._mode;
		_unused_host_name = src._unused_host_name;
		_realname = src._realname;
		_n_channels = src._n_channels;
		_bot = src._bot;
	}
	return *this;
}

Client::~Client(void) {}

void	Client::set_address(const std::string& address)		{ _address = address; }
void	Client::set_status(bool status)						{ _status = status; }
void	Client::set_buffer(const std::string& buffer)		{ _buffer = buffer; }
void	Client::set_password(const std::string& password)	{ _password = password; }
void	Client::set_nickname(const std::string& nickname)	{ _nickname = nickname; }
void	Client::set_username(const std::string& username)	{ _username = username; }
void	Client::set_mode(const std::string& mode)			{ _mode = mode; }
void	Client::set_unused_host_name(const std::string& x)	{ _unused_host_name = x; }
void	Client::set_realname(const std::string& realname)	{ _realname = realname; }
void	Client::set_n_channels(int n_channels)				{ _n_channels = n_channels; }
void	Client::activate_the_bot(void)						{ _bot = true; };

void	Client::add_channel(Channel& channel)				{ _channels.push_back(&channel); }

int				Client::get_fd(void)						{ return _fd; }
std::string&	Client::get_address(void)					{ return _address; }
bool			Client::get_status(void)					{ return _status; }
std::string&	Client::get_buffer(void)					{ return _buffer; }
std::string&	Client::get_password(void)					{ return _password; }
std::string&	Client::get_nickname(void)					{ return _nickname; }
std::string&	Client::get_username(void)					{ return _username; }
std::string&	Client::get_mode(void)						{ return _mode; }
std::string&	Client::get_realname(void)					{ return _realname; }
int				Client::get_n_channels(void)				{ return _n_channels; }
bool			Client::get_bot_status(void)				{ return _bot; }

std::list<Channel*>&	Client::get_channels(void)			{ return _channels; }

void			Client::del_channel(Channel& channel)		{ _channels.remove(&channel); }
void			Client::deactivate_the_bot(void)			{ _bot = false; }

std::string		Client::get_who(void)						{ return get_nickname() + "!" + get_username() + "@" + get_address(); }
std::string		Client::get_who_small(void)					{ return "!" + get_username() + "@" + get_address(); }
