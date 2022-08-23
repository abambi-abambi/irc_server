/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abambi <abambi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/19 17:08:06 by abambi            #+#    #+#             */
/*   Updated: 2022/08/17 11:47:16 by abambi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "Channel.hpp"
# include <string>
# include <list>


class Channel;

class Client
{
	private:
		int					_fd;
		std::string			_address;
		std::string			_password;
		bool				_status;
		std::string			_buffer;
		std::string			_nickname;
		std::string			_username;
		std::string			_mode;
		std::string			_unused_host_name;
		std::string			_realname;
		std::list<Channel*>	_channels;
		int					_n_channels;
		bool				_bot;

	public:
		Client(void);
		Client(int fd, const std::string& address, const std::string& password);
		Client(const Client& src);
		Client&	operator=(const Client& rhs);
		~Client(void);

		void					set_address(const std::string& address);
		void					set_status(bool status);
		void					set_buffer(const std::string& buffer);
		void					set_password(const std::string& password);
		void					set_nickname(const std::string& nickname);
		void					set_username(const std::string& username);
		void					set_mode(const std::string& mode);
		void					set_unused_host_name(const std::string& x);
		void					set_realname(const std::string& realname);
		void					set_n_channels(int n_channels);
		void					activate_the_bot(void);
	
		void					add_channel(Channel& channel);

		int						get_fd(void);
		std::string&			get_address(void);
		bool					get_status(void);
		std::string&			get_buffer(void);
		std::string&			get_password(void);
		std::string&			get_nickname(void);
		std::string&			get_username(void);
		std::string&			get_mode(void);
		std::string&			get_realname(void);
		int						get_n_channels(void);
		bool					get_bot_status(void);

		std::list<Channel*>&	get_channels(void);

		void					del_channel(Channel& channel);
		void					deactivate_the_bot(void);

		std::string				get_who(void);
		std::string				get_who_small(void);
};

#endif
