/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abambi <abambi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/11 17:21:12 by abambi            #+#    #+#             */
/*   Updated: 2022/08/19 12:42:44 by abambi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef COMMANDS_HPP
# define COMMANDS_HPP

# include "Client.hpp"

# include <iostream>		// std::cout
# include <cstring>			// strchr()
# include <sstream>

# include <list>
# include <map>
# include <algorithm>

# include <sys/socket.h>	// command send()
# include <unistd.h>		// close()

# define ERR_UNKNOWNCOMMAND		421

// special additions adapted to KVirc
# define ERR_UNAVAILRESOURCE	437

// PASS
# define ERR_NEEDMOREPARAMS		461
# define ERR_ALREADYREGISTRED	462
# define ERR_PASSWDMISMATCH		464

// NICK
# define ERR_NEEDMOREPARAMS		461
# define ERR_NONICKNAMEGIVEN	431
# define ERR_ERRONEUSNICKNAME	432
# define ERR_NICKNAMEINUSE		433

// USER
# define ERR_NOTREGISTERED		451
# define ERR_NEEDMOREPARAMS		461
# define ERR_ALREADYREGISTRED	462
# define ERR_UNKNOWNMODE		472
# define ERR_NOSERVICEHOST		492

// JOIN
# define ERR_NOSUCHCHANNEL		403
# define ERR_TOOMANYCHANNELS	405
# define ERR_CHANNELISFULL		471
# define ERR_LARGERTARGMAX		999

// REPLY
# define RPL_WELCOME			001
# define RPL_NAMREPLY			353
# define RPL_ENDOFNAMES			366
# define RPL_COMMANDSYNTAX		704
# define RPL_ENDOFSTATS			219

// KICK
# define ERR_USERNOTINCHANNEL   441
# define ERR_NOTONCHANNEL		442
# define ERR_CHANOPRIVSNEEDED	482

// NOTICE
# define ERR_NOSUCHNICK			401
# define ERR_CANNOTSENDTOCHAN	404
# define ERR_TOOMANYTARGETS		407
# define ERR_NORECIPIENT		411
# define ERR_NOTEXTTOSEND		412

// LIST
# define RPL_LIST				322
# define RPL_LISTEND			323

# define CLIENT					"KVIrc"
# define MAX_NICK_LENGTH		15
# define USERLEN				18
# define CHANLIMIT				3
# define TARGMAX				1

class Commands
{
	private:
		Commands(const Commands &src);
		Commands &operator=(const Commands &src);

	protected:
		std::string						_name;
		std::string						_servname;
		std::map<std::string, Channel>	_channels;
		std::map<std::string, Client*>	_user_list;
		std::map<int, Client>			_clients;

		struct Message
		{
			std::string				_prefix;
			std::string				_command;
			std::list<std::string>	_paramlist;
			std::string				_tmp;
		};
		typedef	void (Commands::*Command)(Client& client, Message& msg);
		std::map<std::string, Command>	_commands;

	public:
		Commands(void);
		virtual ~Commands(void);

    std::map<std::string, Client *>::const_iterator find_user(Message& msg);

	std::string set_code(int response_code);
	std::string add_kvirc_params(Client &client, int response_code);
	std::string add_kvirc_params_short(Client &client, int response_code);
	std::string add_kvirc_params_long(Client &client, int response_code);

	void	cmd_bot(Client &client, Message &msg);
	void	cmd_start(Client &client, Message &msg);
	void	cmd_stop(Client &client, Message &msg);
	
	void	cmd_cap(Client& client, Message& msg);
	void	cmd_ping(Client& client, Message& msg);
	void	cmd_mode(Client& client, Message& msg);
	void	cmd_who(Client& client, Message& msg);

	void	cmd_pass(Client &client, Message &msg);

	void	cmd_names(Client& client, Message& msg);
	void	cmd_list(Client& client, Message& msg);

	void	cmd_nick(Client &client, Message &msg);
	int		isvalid_nick(const std::string &nick);
	void	ischanged_nick_notify(Client& client, Message& msg);
	void	send_to_channel(Client& client, std::string& response, Channel& channel, bool self);
	
	void	cmd_user(Client& client, Message& msg);

	void	cmd_join(Client& client, Message& msg);
	int		isvalid_channel(const std::string& name);
	void	join_channel(Channel&	channel, Client& client, Message& msg);
	void	exit_all_channels(Client& client);
	void	cmd_quit(Client& client, Message& msg);
	void	quit_msg(Client& client, Message& msg);
	
	void	cmd_kick(Client& client, Message& msg);

	void	cmd_notice(Client& client, Message& msg);
	void	cmd_privmsg(Client& client, Message& msg);
	void	privmsg_notice_to_user(Client& client, Message& msg, std::string response);
	void	privmsg_notice_to_channel(Client& client, Message& msg, std::string response);
	std::string	add_message(std::string &response, Message& msg, size_t skip, std::string nick);

	void	cmd_usersall(Client& client, Message& msg);
	void	cmd_chansall(Client& client, Message& msg);
	void	cmd_usersinchan(Client& client, Message& msg);
	void	cmd_myname(Client& client, Message& msg);
	void	cmd_help(Client& client, Message& msg);

	void	cmd_test0(Client& client, Message& msg);
	void	cmd_test1(Client& client, Message& msg);
	void	cmd_test2(Client& client, Message& msg);
	void	cmd_test3(Client& client, Message& msg);
	void	cmd_test4(Client& client, Message& msg);
	void	cmd_test5(Client& client, Message& msg);
	void	cmd_test6(Client& client, Message& msg);
	void	cmd_test7(Client& client, Message& msg);
	void	cmd_test8(Client& client, Message& msg);
	void	cmd_test9(Client& client, Message& msg);

	void	pass_errors(Client& client, Message &msg, int error);
	void	nick_errors(Client& client, Message& msg, int error);
	void	user_errors(Client& client, Message& msg, int error);
	void	join_errors(Client& client, Message& msg, int error);
	int 	check_kick_errors(Client& client, Message& msg);
	int 	check_kick_errors2(Client& client, Message& msg, \
	            std::map<std::string, Client *>::const_iterator user, std::map<std::string, Channel>::iterator it);	
	void	privmsg_notice_errors(Client& client, Message& msg, int error);
};

#endif
