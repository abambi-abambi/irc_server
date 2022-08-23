/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abambi <abambi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/19 17:09:03 by abambi            #+#    #+#             */
/*   Updated: 2022/08/19 13:02:41 by abambi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"

Commands::Commands(void)
{
	_commands["BOT"]		= &Commands::cmd_bot;
	_commands["CAP"]		= &Commands::cmd_cap;
	_commands["CHANSALL"]	= &Commands::cmd_chansall;
	_commands["HELP"]		= &Commands::cmd_help;
	_commands["JOIN"]		= &Commands::cmd_join;
	_commands["KICK"]		= &Commands::cmd_kick;
	_commands["MODE"]		= &Commands::cmd_mode;
	_commands["MYNAME"]		= &Commands::cmd_myname;
	_commands["NICK"]		= &Commands::cmd_nick;
	_commands["NOTICE"]		= &Commands::cmd_notice;
	_commands["PASS"]		= &Commands::cmd_pass;
	_commands["PING"]		= &Commands::cmd_ping;
	_commands["PRIVMSG"]	= &Commands::cmd_privmsg;
	_commands["START"]		= &Commands::cmd_start;
	_commands["STOP"]		= &Commands::cmd_stop;
	_commands["TEST0"]		= &Commands::cmd_test0;
	_commands["TEST1"]		= &Commands::cmd_test1;
	_commands["TEST2"]		= &Commands::cmd_test2;
	_commands["TEST3"]		= &Commands::cmd_test3;
	_commands["TEST4"]		= &Commands::cmd_test4;
	_commands["TEST5"]		= &Commands::cmd_test5;
	_commands["TEST6"]		= &Commands::cmd_test6;
	_commands["TEST7"]		= &Commands::cmd_test7;
	_commands["TEST8"]		= &Commands::cmd_test8;
	_commands["TEST9"]		= &Commands::cmd_test9;
	_commands["QUIT"]		= &Commands::cmd_quit;
	_commands["USER"]		= &Commands::cmd_user; 
	_commands["USERSALL"]	= &Commands::cmd_usersall;
	_commands["USERSINCHAN"] = &Commands::cmd_usersinchan;
	_commands["WHO"]		= &Commands::cmd_who;
	std::cout << "command's tuning: success\n";
}

Commands::~Commands(void)
{

}

/*
** for service purposes in KVirc
*/

void	Commands::cmd_cap(Client& client, Message& msg)
{
	(void)client;
	(void)msg;
}

void	Commands::cmd_ping(Client& client, Message& msg)
{
	std::string								response;
	std::list<std::string>::const_iterator	it;

	response  = "PONG";
	for (it = msg._paramlist.begin(); it != msg._paramlist.end(); ++it)
		response += " " + *it;
	response += "\r\n";
	send(client.get_fd(), response.c_str(), response.size(), MSG_DONTWAIT );
}

void	Commands::cmd_mode(Client& client, Message& msg)
{
	(void)client;
	(void)msg;
}

void	Commands::cmd_who(Client& client, Message& msg)
{
	(void)client;
	(void)msg;
}

/*
** adaptations for Kvirc client
*/

std::string Commands::set_code(int response_code)
{
	std::string			code;
	std::stringstream	ss;

	ss << response_code;
	code = ss.str();
	while (code.size() < 3)
		code = "0" + code;	
	return (code);
}

std::string Commands::add_kvirc_params(Client &client, int response_code)
{
	std::string	response;

	if (client.get_nickname().empty())
		response = ":" + _servname + " " + set_code(response_code) + " " + CLIENT + " ";
	else
		response = ":" + _servname + " " + set_code(response_code) + " " + client.get_nickname() + " ";	
	return (response);
}

/*
** PASS <password>
*/

void	Commands::cmd_pass(Client &client, Message &msg)
{
	if (client.get_nickname().size() == 0)
	{
		if (msg._paramlist.empty() == true)
		{
			pass_errors(client, msg, ERR_NEEDMOREPARAMS);
		}
		else if (msg._paramlist.front() != client.get_password())
		{
			client.set_status(false);
			pass_errors(client, msg, ERR_PASSWDMISMATCH);
		}
		else
		{
			client.set_status(true);
		}
	}
	else if (client.get_nickname().size() > 0)
	{
		pass_errors(client, msg, ERR_ALREADYREGISTRED);
	}
	else
		{};
}

void	Commands::pass_errors(Client& client, Message &msg, int error)
{
	std::string response;

	response = add_kvirc_params(client, error);
	if (error == ERR_NEEDMOREPARAMS)
	{
		response += msg._command + " :Not enough parameters\r\n";
	}
	else if (error == ERR_PASSWDMISMATCH)
	{
		response += ":Password incorrect\r\n";
	}
	else if (error == ERR_ALREADYREGISTRED)
	{
		response += ":Unauthorized command (already registered)\r\n";
	}
	else
		{};
	send(client.get_fd(), response.c_str(), response.size(), MSG_DONTWAIT  );
}

/*
** NICK <nickname>
** :old_nick NICK new_nick
*/

int	Commands::isvalid_nick(const std::string& nick)
{
	size_t	i;

	if (nick.size() > MAX_NICK_LENGTH)
		return (-1);
	i = 0;
	while (i < nick.size())
	{
		if (((nick[i] >= 'A' && nick[i] <= 'Z') 
		 || (nick[i] >='a' && nick[i] <= 'z') 
		 || (nick[i] >= '0' && nick[i] <= '9') 
		 ||  nick[i] == '-' || nick[i] == '_') == false)
			break ;
		i++;
	}
	if (i == nick.size())
		return (0);
	else
		return (-1);
}

/*
**	BIG-server's errors:
**    ERR_RESTRICTED ":Your connection is restricted!" if the restricted nature of the connection (user mode "+r"
**    ERR_NICKCOLLISION: "<nick> :Nickname collision KILL from <user>@<host>"
**    ERR_UNAVAILRESOURCE "<nick/channel> :Nick/channel is temporarily unavailable" user trying to change nickname when the desired nickname is blocked by the nick delay mechanism.
*/

int	ft_compare(std::string const& str1, std::string const& str2)
{    
    if (str1.size() != str2.size())
		return (1);
	
    for (size_t i = 0; i < str1.size(); i++)
        if (str1[i] != str2[i])
			return (1);
    return (0);
}
 
void	Commands::cmd_nick(Client& client, Message& msg)
{
	std::map<std::string, Client*>::const_iterator	it;
	std::string	old_nick;
	std::string	new_nick;

	if (msg._paramlist.size() != 0)
		new_nick = msg._paramlist.front();
	if (msg._paramlist.empty() == true)
	{
		nick_errors(client, msg, ERR_NONICKNAMEGIVEN);
	}
	else if (msg._prefix.size() == 0 && client.get_nickname().size() > 0)
	{
		nick_errors(client, msg, ERR_ALREADYREGISTRED);
	}
	else if (isvalid_nick(new_nick) == -1)
	{
		nick_errors(client, msg, ERR_ERRONEUSNICKNAME);
	}
	else if (_user_list.find(new_nick) != _user_list.end())
	{
		nick_errors(client, msg, ERR_NICKNAMEINUSE);
	}
	else
	{
		if (msg._prefix.size() > 0)
		{
			if (client.get_nickname().size() == 0)
			{
				nick_errors(client, msg, ERR_NOTREGISTERED);
				return ;
			}
			if (ft_compare(msg._prefix,  client.get_nickname()) == 1) // + we forbid white symbols at the begining (before :old_nick) // see exec_command() and split_message() logic
			{
				nick_errors(client, msg, ERR_ERRONEUSNICKNAME);
				return ;
			}
			old_nick = client.get_nickname();
			msg._tmp = old_nick;
			_user_list.erase(old_nick);
		}
		client.set_nickname(new_nick);
		_user_list[new_nick] = &client;

		if (msg._prefix.size() > 0)
			ischanged_nick_notify(client, msg);
	}
}

void	Commands::ischanged_nick_notify(Client& client, Message& msg)
{
	std::string	response;
	std::list<Channel*>::const_iterator	i;

	response = ":" + msg._tmp + client.get_who_small() + " " + msg._command + " " + client.get_nickname() + "\r\n";
	for (i = client.get_channels().begin(); i != client.get_channels().end(); ++i)
		send_to_channel(client, response, **i, true);
}

void	Commands::send_to_channel(Client& client, std::string& response, Channel& channel, bool send_everyone)
{
	std::list<Client*>::const_iterator	it;

	for (it = channel.get_clients().begin(); it != channel.get_clients().end(); ++it)
	{
		if (send_everyone == true || (**it).get_nickname() != client.get_nickname())
			send(_user_list[(**it).get_nickname()]->get_fd(), response.c_str(), response.size(), MSG_DONTWAIT  );
	}
}

void	Commands::nick_errors(Client& client, Message& msg, int error)
{
	std::string			response;

	response = add_kvirc_params(client, error);
	if (error == ERR_NOTREGISTERED)
	{
		response += CLIENT + (std::string)" ::You have not registered\r\n"; // another branch is preferably used: see launch_cmd()
	} // detect this branch ^ by additional ':'
	else if (error == ERR_NONICKNAMEGIVEN) // this error is not supported by KVirc-5.0.0
	{
		response = add_kvirc_params(client, ERR_NEEDMOREPARAMS); // => using another error // change default response params (see previous value) into these ones
		response += msg._command + " :Not enough parameters\r\n";
	}
	else if (error == ERR_ERRONEUSNICKNAME) // KVIrc will automatically try to set the nick (from user settings in KVIrc)
	{
		response += msg._paramlist.front() + " :Erroneus nickname\r\n";
	}
	else if (error == ERR_NICKNAMEINUSE) // KVIrc will automatically try to set the nick (from user settings in KVIrc)
	{
		response += (std::string)" " + msg._paramlist.front() + " :Nickname is already in use\r\n";
	}
	else if (error == ERR_ALREADYREGISTRED)
	{
		response += CLIENT + (std::string)" :Unauthorized command (already registered)\r\n";
	}
	else
		{};
	send(client.get_fd(), response.c_str(), response.size(), MSG_DONTWAIT  ); 
}

/*
** USER <user> <mode> <unused> <realname>
** USER <user> 0 * <realname>
*/

void	Commands::cmd_user(Client& client, Message& msg)
{
	std::string	new_user;
	std::string	response;
	std::string	real_name;
	int			i;

	if (client.get_nickname().size() == 0)
	{
		user_errors(client, msg, ERR_NOTREGISTERED);
	}
	else if (msg._paramlist.size() < 4)
	{
		user_errors(client, msg, ERR_NEEDMOREPARAMS);
	}
	else if (client.get_username().size() > 0)
	{
		user_errors(client, msg, ERR_ALREADYREGISTRED);
	}
	else if (*(++msg._paramlist.begin()) != "0" || *(++++msg._paramlist.begin()) != "*")
	{
		if (*(++msg._paramlist.begin()) != "0")
			user_errors(client, msg, ERR_UNKNOWNMODE);
		if (*(++++msg._paramlist.begin()) != "*")
			user_errors(client, msg, ERR_NOSERVICEHOST);
	}
	else
	{
		new_user = msg._paramlist.front(); // our realname should not obligatory begin from ':'
		if (new_user.length() > USERLEN)
			new_user = new_user.substr(0, USERLEN);
		client.set_username(new_user);
		client.set_mode("0");
		client.set_unused_host_name("*");
		i = 0;
		for (std::list<std::string>::const_iterator it = msg._paramlist.begin(); it != msg._paramlist.end(); it++)
		{
			if (i >= 3)
			{
				if (i != (int)msg._paramlist.size() - 1)
				{
					real_name = real_name.append(*it);
					real_name = real_name.append(" ");
				}
				else
					real_name = real_name.append(*it);
			}
			i++;
		}
		if (real_name.length() > USERLEN)
			real_name = real_name.substr(0, USERLEN);
		client.set_realname(real_name); // we don't check the realname for errors and save it "as is"
		response = add_kvirc_params(client, RPL_WELCOME);
		response += CLIENT + (std::string)" :Welcome to the " + _servname + " Network, " + client.get_who() + "\r\n"; 
		send(client.get_fd(), response.c_str(), response.size(), MSG_DONTWAIT  );
	}
}

void	Commands::user_errors(Client& client, Message& msg, int error)
{
	std::string	response;

	response = add_kvirc_params(client, error);
	if (error == ERR_NOTREGISTERED)
	{
		response += ":You have not registered\r\n";
	}
	else if (error == ERR_NEEDMOREPARAMS)
	{
		response += msg._command + " :Not enough parameters\r\n";
	}
	else if (error == ERR_UNKNOWNMODE)
	{
		response += "'" + *(++msg._paramlist.begin()) + "' :is unknown mode char to me\r\n";
		response += add_kvirc_params(client, error);
		response += "in_command:_USER_<username>_<mode>_<hostname>_<realname> the_<mode>_equals_'0'\r\n";
	}
	else if (error == ERR_NOSERVICEHOST) // this error is not supported by KVirc-5.0.0
	{
		response = add_kvirc_params(client, ERR_UNKNOWNMODE); // => using another error // change default response params (see previous value) into these ones
		response += " '" + *(++++msg._paramlist.begin()) + "' :is unknown hostname to me\r\n" +
		response += "in_command:_USER_<username>_<mode>_<hostname>_<realname> the_<hostname>_equals_'*'\r\n";
	}
	else if (error == ERR_ALREADYREGISTRED)
	{
		response += ":Unauthorized command (already registered)\r\n";
	}
	else
		{};
	send(client.get_fd(), response.c_str(), response.size(), MSG_DONTWAIT  );
}

/*
** JOIN <channel>{,<channel>} [<key>{,<key>}]
** JOIN 0 (Leave all currently joined channels)
*/

int	Commands::isvalid_channel(const std::string& name)
{
	size_t len = 0;

	len = name.length();
	if (len == 1 && name[0] == '0')
		return (0);
	if (len <= 1 || len > 11 || name[0] != '#')
		return (-1);
	for (size_t i = 0; i < len; ++i)
		if (name[i] == ',') // we consider only ',' as error inside the name
			return (-1); // if there are next args for channel after channel name, they are saved in _paramlist but we ignore them 
	return (0);
}

/*
** BIG server's errors:
**    ERR_BANNEDFROMCHAN
**    ERR_INVITEONLYCHAN
**    ERR_UNAVAILRESOURCE

**	  ERR_BADCHANNELKEY
**    ERR_BADCHANMASK

**    ERR_TOOMANYTARGETS

**    RPL_TOPIC	& RPL_NOTOPIC
*/

void	Commands::cmd_join(Client& client, Message& msg)
{
	std::string	response;
	std::string	channel;

	if (msg._paramlist.size() == TARGMAX)
		channel = msg._paramlist.front();
	else if (msg._paramlist.size() > TARGMAX)
	{
		join_errors(client, msg, ERR_LARGERTARGMAX);
		return ;
	}
	if (client.get_username().size() == 0)
	{
		join_errors(client, msg, ERR_NOTREGISTERED);
	}
	else if (msg._paramlist.size() == 0)
	{
		join_errors(client, msg, ERR_NEEDMOREPARAMS);
	}
	else if (isvalid_channel(channel) == -1)
	{
		join_errors(client, msg, ERR_NOSUCHCHANNEL);
	}
	else if (channel == "0")
	{
		exit_all_channels(client);
	}
	else
	{
		if (_channels.find(channel) == _channels.end())
		{
			Channel channel_obj = Channel(channel);
			_channels.insert(std::make_pair(channel, channel_obj));
		}
		if ((client.get_n_channels() + 1) > CHANLIMIT)
		{
			if (find(client.get_channels().begin(), client.get_channels().end(), &(_channels.find(channel)->second)) != client.get_channels().end())
				return ;
			join_errors(client, msg, ERR_TOOMANYCHANNELS); // we can allow to create the new channel, but not to enter in
		}
		else if ((_channels.find(channel)->second.get_client_counter() + 1) > CLIENT_LIMIT)
		{
			join_errors(client, msg, ERR_CHANNELISFULL);
		}
		else if (find(client.get_channels().begin(), client.get_channels().end(), &(_channels.find(channel)->second)) != client.get_channels().end())
		{
			return ;
		}
		else
		{
			join_channel(_channels[channel], client, msg);
		}
	}
}

void	Commands::join_channel(Channel& channel, Client& client, Message& msg)
{
	std::string	response;

	client.add_channel(channel);
	client.set_n_channels(client.get_n_channels() + 1);
	channel.add_client(client);
	channel.set_client_counter(channel.get_client_counter() + 1);
	response = ":" + client.get_who() + " " + msg._command + " " + msg._paramlist.front() + "\r\n";
	send_to_channel(client, response, channel, true);
	response = add_kvirc_params(client, RPL_NAMREPLY);
	response += CLIENT + (std::string)" = " + msg._paramlist.front() + " :" + channel.get_names() + "\r\n";
	response += add_kvirc_params(client, RPL_ENDOFNAMES);
	response += CLIENT + (std::string)" " + msg._paramlist.front() + " :End of /NAMES list\r\n";
	send(client.get_fd(), response.c_str(), response.size(), MSG_DONTWAIT  );
}

void	Commands::exit_all_channels(Client& client)
{
	std::string										response;
	std::map<std::string, Channel>::iterator		channel_name_it;

	if (client.get_n_channels() == 0)
		return ;
	for (channel_name_it = _channels.begin(); channel_name_it != _channels.end(); channel_name_it++)
	{
		if (find(client.get_channels().begin(), client.get_channels().end(), &channel_name_it->second) != client.get_channels().end())
		{
			response = ":" + client.get_who() + " PART " + channel_name_it->first + "\r\n";
			send_to_channel(client, response, channel_name_it->second, true);
			client.del_channel(channel_name_it->second);
			client.set_n_channels(client.get_n_channels() - 1);
			channel_name_it->second.del_client(client);
			channel_name_it->second.set_client_counter(channel_name_it->second.get_client_counter() - 1);
		} // graceful exit from channels, without channel deleting
	}
}

void	Commands::join_errors(Client& client, Message& msg, int error)
{
	std::string	response;

	response = add_kvirc_params(client, error);
	if (error == ERR_NOTREGISTERED)
	{
		response += ":You have not registered\r\n";
	}
	else if (error == ERR_NEEDMOREPARAMS)
	{
		response += CLIENT + (std::string)" " + msg._command + " :Not enough parameters\r\n";
	}
	else if (error == ERR_NOSUCHCHANNEL)
	{
		response += CLIENT + (std::string)" " + msg._paramlist.front() + " :No such channel\r\n";
	}
	else if (error == ERR_LARGERTARGMAX) // this custom error of our server is not supported by KVirc-5.0.0
	{
		response = add_kvirc_params(client, ERR_UNAVAILRESOURCE); // => using another error	// change default response params (see previous value) into these ones
		response += CLIENT + (std::string)" " + msg._paramlist.front() + " :Multiple channel connections in one JOIN command is not supported by our server\r\n";
	}
	else if (error == ERR_TOOMANYCHANNELS) // this error is not supported by KVirc-5.0.0
	{
		response = add_kvirc_params(client, ERR_UNAVAILRESOURCE); // => using another error	// change default response params (see previous value) into these ones
		response += CLIENT + (std::string)" " + msg._paramlist.front() + " :You have joined too many channels\r\n";
	}
	else if (error == ERR_CHANNELISFULL)
	{
		response += CLIENT + (std::string)" " + msg._paramlist.front() + " :Cannot join channel (+l)\r\n";
	}
	else
		{};
	send(client.get_fd(), response.c_str(), response.size(), MSG_DONTWAIT  );
}

/*
** QUIT <optional: quit message>
*/

void	Commands::cmd_quit(Client& client, Message& msg)
{
	std::list<Channel*>::const_iterator		channel_list;
	int										id;

	if (client.get_nickname().size() > 0)
	{
		quit_msg(client, msg);
		for (channel_list = client.get_channels().begin(); channel_list != client.get_channels().end(); ++channel_list)
		{
			(*channel_list)->del_client(client); // delete client from channel
			(*channel_list)->set_client_counter((*channel_list)->get_client_counter() - 1);
			if ((*channel_list)->get_clients().size() == 0)
				_channels.erase((*channel_list)->get_name()); // delete channel from list of channels
		}
		_user_list.erase(client.get_nickname());
		client.set_n_channels(0);
	}
	id = client.get_fd();
	close(id);
	std::cout << "Client #fd" << id << " gone away ~\n";
	_clients.erase(id);
}

void	Commands::quit_msg(Client& client, Message& msg)
{
	std::list<Channel*>::const_iterator		channel_list;
	std::list<std::string>::const_iterator	it;
	std::string								response;

	response = ":" + client.get_who() + " " + msg._command + " :";
	if (msg._paramlist.size() != 0)
		for (it = msg._paramlist.begin(); it != msg._paramlist.end(); ++it)
			response += " " + *it;
	response += "\r\n";
	for (channel_list = client.get_channels().begin(); channel_list != client.get_channels().end(); ++channel_list)
		send_to_channel(client, response, **channel_list, false);
}

/*
** KICK <channel> <user> [optional: comment]
*/

std::map<std::string, Client *>::const_iterator Commands::find_user(Message& msg)
{
    std::map<std::string, Client *>::const_iterator	user;
    std::list<std::string>::const_iterator  param;
    std::string user_name;

    param = msg._paramlist.begin();
    ++param;
    user_name = *param;
    user = _user_list.find(user_name);
    return (user);
}

void	Commands::cmd_kick(Client& client, Message& msg)
{
	std::map<std::string, Client *>::const_iterator	user;
	std::string										response;
	std::map<std::string, Channel>::iterator		channel_it;

	if (check_kick_errors(client, msg) == -1)
		return ;
	channel_it = _channels.find(msg._paramlist.front());
	user = find_user(msg);
	if (check_kick_errors2(client, msg, user, channel_it) == -1)
    	return ;
	response += \
		":" + client.get_who() + " " + msg._command + " " + *(msg._paramlist.begin()) + " " + *(++msg._paramlist.begin()) + " :";
	add_message(response, msg, 2, client.get_nickname());
	send_to_channel(client, response, channel_it->second, true);
	user->second->del_channel(channel_it->second); // delete channel from client
	user->second->set_n_channels(user->second->get_n_channels() - 1);
	channel_it->second.del_client(*(user->second)); // delete client from channel
	channel_it->second.set_client_counter(channel_it->second.get_client_counter() - 1);
	if (channel_it->second.get_clients().size() == 0)
		_channels.erase(channel_it->second.get_name()); // delete channel from list of channels
}

int 	Commands::check_kick_errors(Client& client, Message& msg)
{
	int			error;
	std::string	response;
	int			res = 0;

	if (client.get_username().size() == 0)
	{
		error = ERR_NOTREGISTERED;
		response = add_kvirc_params(client, error);
		response += ":You have not registered\r\n";
		res = -1;
	}
	else if (msg._paramlist.size() < 1)
	{
		error = ERR_NEEDMOREPARAMS;
		response = add_kvirc_params(client, error);
		response += CLIENT + (std::string)" " + msg._command + " :Not enough parameters\r\n";
		res = -1;
	}
	else 
	{
		if (msg._paramlist.size() < 2)
		{
			error = ERR_NEEDMOREPARAMS;
			response = add_kvirc_params(client, error);
			response += CLIENT + (std::string)" " + msg._command + " :Not enough parameters\r\n";
			res = -1;
		}
		if (isvalid_channel(msg._paramlist.front()) == -1 || _channels.find(msg._paramlist.front()) == _channels.end())
		{
			error = ERR_NOSUCHCHANNEL;
			response += add_kvirc_params(client, error);
			response += msg._paramlist.front() + " :No such channel\r\n";
			res = -1;
		}
		else if (find(client.get_channels().begin(), client.get_channels().end(), \
			&(_channels.find(msg._paramlist.front())->second)) == client.get_channels().end())
		{
			error = ERR_NOTONCHANNEL; // this error is not supported by KVirc-5.0.0
			response += add_kvirc_params(client, ERR_UNAVAILRESOURCE); // => using another error
			response += msg._paramlist.front() + " :You're not on that channel\r\n";
			res = -1;
		}
		else if (_channels.find(msg._paramlist.front())->second.get_clients().front()->get_nickname() != client.get_nickname())
		{
			error = ERR_CHANOPRIVSNEEDED;
			response += add_kvirc_params(client, error);
			response += msg._paramlist.front() + " :You're not channel operator\r\n";
			// response is sent by KVirc inside the channel, unlike above ones
			res = -1;
		}
		else
			{};
	}
	if (res == -1)
		send(client.get_fd(), response.c_str(), response.size(), MSG_DONTWAIT  );
	return (res);
}

int 	Commands::check_kick_errors2(Client& client, Message& msg, \
            std::map<std::string, Client *>::const_iterator user, std::map<std::string, Channel>::iterator it)
{
	int			error;
	std::string	response;
	int			res = 0;

	if (user == _user_list.end())
    {
	    error = ERR_NOSUCHNICK;
		response = add_kvirc_params(client, error);
		response += *(++msg._paramlist.begin()) + " :No such nick/channel\r\n";
		res = -1;
    }
	else if	(find(user->second->get_channels().begin(), user->second->get_channels().end(), &(it->second)) \
		    == user->second->get_channels().end())
	{
		error = ERR_USERNOTINCHANNEL; // this error is not supported by KVirc-5.0.0
		response += add_kvirc_params(client, ERR_UNAVAILRESOURCE); // => using another error
       	response += CLIENT + (std::string)" " + *(++msg._paramlist.begin()) + " " + *(msg._paramlist.begin()) + \
        " :They aren't on that channel\r\n";
		res = -1;
	}
	else
		{};
	if (res == -1)
		send(client.get_fd(), response.c_str(), response.size(), MSG_DONTWAIT  );
	return (res);	
}

/*
** NOTICE <msgtarget> <text>
** <msgtarget> is the (recipient) nickname or the channel name
*/

void	Commands::cmd_notice(Client& client, Message& msg)
{
	std::string	response;

	if (client.get_username().empty())
		privmsg_notice_errors(client, msg, ERR_NOTREGISTERED);
	else if (client.get_n_channels() == 0)
		privmsg_notice_errors(client, msg, ERR_CANNOTSENDTOCHAN);
	else if (msg._paramlist.size() < 2)
	{
		if (msg._paramlist.size() == 0)
			privmsg_notice_errors(client, msg, ERR_NEEDMOREPARAMS);
		else if (_channels.find(msg._paramlist.front()) != _channels.end() && \
			find(client.get_channels().begin(), client.get_channels().end(), \
			&(_channels.find(msg._paramlist.front())->second)) == client.get_channels().end())
		{
			privmsg_notice_errors(client, msg, ERR_CANNOTSENDTOCHAN);
			privmsg_notice_errors(client, msg, ERR_NOTEXTTOSEND);
		}			
		else if (_user_list.find(msg._paramlist.front()) == _user_list.end() && \
			find(client.get_channels().begin(), client.get_channels().end(), \
			&(_channels.find(msg._paramlist.front())->second)) == client.get_channels().end())
			privmsg_notice_errors(client, msg, ERR_NORECIPIENT);
		else
			privmsg_notice_errors(client, msg, ERR_NOTEXTTOSEND);
	}
	// else if (msg._paramlist.size() >= 3 && 
	// 	find(msg._paramlist.begin(), ++++msg._paramlist.begin(), ",") != msg._paramlist.end())
	// {
	// 	privmsg_notice_errors(client, msg, ERR_TOOMANYTARGETS);
	// }
	else
	{
		response = ":" + client.get_who() + " " + msg._command + " ";
		if (msg._paramlist.front()[0] == '#')
		{
			privmsg_notice_to_channel(client, msg, response);
		}
		else
		{
			privmsg_notice_to_user(client, msg, response);
		}
	}
}

/*
** PRIVMSG <msgtarget> <text>
** <msgtarget> is the (recipient) nickname or the channel name
*/

/*
** for operators && channel-modes: ERR_WILDTOPLEVEL ERR_NOTOPLEVEL
*/

void	Commands::cmd_privmsg(Client& client, Message& msg)
{
	std::string	response;

	if (client.get_username().empty())
		privmsg_notice_errors(client, msg, ERR_NOTREGISTERED);
	else if (client.get_n_channels() == 0)
		privmsg_notice_errors(client, msg, ERR_CANNOTSENDTOCHAN);
	else if (msg._paramlist.size() < 2)
	{
		if (msg._paramlist.size() == 0)
			privmsg_notice_errors(client, msg, ERR_NEEDMOREPARAMS);
		else if (_channels.find(msg._paramlist.front()) != _channels.end() && \
			find(client.get_channels().begin(), client.get_channels().end(), \
			&(_channels.find(msg._paramlist.front())->second)) == client.get_channels().end())
		{
			privmsg_notice_errors(client, msg, ERR_CANNOTSENDTOCHAN);
			privmsg_notice_errors(client, msg, ERR_NOTEXTTOSEND);
		}			
		else if (_user_list.find(msg._paramlist.front()) == _user_list.end() && \
			find(client.get_channels().begin(), client.get_channels().end(), \
			&(_channels.find(msg._paramlist.front())->second)) == client.get_channels().end())
			privmsg_notice_errors(client, msg, ERR_NORECIPIENT);
		else
			privmsg_notice_errors(client, msg, ERR_NOTEXTTOSEND);
	}
	// else if (msg._paramlist.size() >= 3 && 
	// 	find(msg._paramlist.begin(), ++++msg._paramlist.begin(), ",") != msg._paramlist.end())
	// {
	// 	privmsg_notice_errors(client, msg, ERR_TOOMANYTARGETS);
	// }
	else
	{
		response = ":" + client.get_who() + " " + msg._command + " ";
		if (msg._paramlist.front()[0] == '#')
		{
			privmsg_notice_to_channel(client, msg, response);
		}
		else
		{
			privmsg_notice_to_user(client, msg, response);
		}
	}
}

void	Commands::privmsg_notice_errors(Client& client, Message& msg, int error)
{
	std::string	response;

	if (error == ERR_NOTREGISTERED)
	{
		response = add_kvirc_params(client, error);
		response += ":You have not registered\r\n";
	}
	else if (error == ERR_NEEDMOREPARAMS)
	{
		response = add_kvirc_params(client, error);
		response += CLIENT + (std::string)" " + msg._command + " :Not enough parameters\r\n";
	}
	// else if (error == ERR_TOOMANYTARGETS) // this error is not supported by KVirc-5.0.0
	// { // find another error instead for hanling by KVirc-5.0.0 and test this branch if you decide to use it
	// 	response = add_kvirc_params(client, error);
	// 	response += CLIENT + (std::string)" :" + *(msg._paramlist.begin()) + " " +  
	// 											 *(++msg._paramlist.begin()) + " ... :407 recipients. Too many recipients\r\n";
	// }	
	else if (error == ERR_NORECIPIENT)
	{
		response = add_kvirc_params(client, ERR_UNAVAILRESOURCE);
		response += CLIENT + (std::string)" :No recipient given (" + msg._command + ")\r\n";
	}
	else if (error == ERR_NOTEXTTOSEND)
	{
		response = add_kvirc_params(client, ERR_UNAVAILRESOURCE);
		response += CLIENT + (std::string)" :No text to send\r\n";
	}
	else if (error == ERR_NOSUCHNICK)
	{
		response = add_kvirc_params(client, error);
		response += CLIENT + (std::string)" " + client.get_nickname() + " :No such nick/channel\r\n";
	}
	else if (error == ERR_NOSUCHCHANNEL)
	{
		response = add_kvirc_params(client, error);
		response += CLIENT + (std::string)" " + msg._paramlist.front() + " :No such channel\r\n";
	}
	else if (error == ERR_CANNOTSENDTOCHAN)
	{
		response = add_kvirc_params(client, error);	
		if (msg._paramlist.size() < 1)
			response += CLIENT + (std::string)" :Cannot send to channel\r\n";
		else	
			response += CLIENT + (std::string)" " + msg._paramlist.front() + " :Cannot send to channel\r\n";
	}
	else
		{};
	send(client.get_fd(), response.c_str(), response.size(), MSG_DONTWAIT  );
}

std::string	Commands::add_message(std::string &response, Message& msg, size_t skip, std::string nick)
{
	size_t											i;
	std::list<std::string>::const_iterator			params;
	size_t											len;

	i = 0;
	params = msg._paramlist.begin();
	len = msg._paramlist.size() - skip;
	while (i < skip)
	{
		++params;
		++i;
	}
	i = 0;
	if (skip == 2 && msg._paramlist.size() == 2)
		response += nick;
	else
	{	
		while (i < len)
		{
			if (i == len - 1)
				response += *params;
			else
				response += *params + " ";
			++i;
			++params;
		}
	}
	response += "\r\n";
	return (response);
}

void	Commands::privmsg_notice_to_user(Client& client, Message& msg, std::string response)
{
	std::map<std::string, Client*>::const_iterator	it;
	std::string										user_name;
	
	user_name = msg._paramlist.front();
	it = _user_list.find(user_name);
	if (it == _user_list.end())
	{
		privmsg_notice_errors(client, msg, ERR_NOSUCHNICK);
		return ;
	}
	response += it->second->get_nickname() + " :";
	add_message(response, msg, 1, "default");
	send(it->second->get_fd(), response.c_str(), response.size(), MSG_DONTWAIT  );
}

void	Commands::privmsg_notice_to_channel(Client& client, Message& msg, std::string response)
{
	std::map<std::string, Channel>::iterator		it;
	std::string										channel_name;
	
	channel_name = msg._paramlist.front();
	it = _channels.find(channel_name);
	if (it == _channels.end())
	{
		privmsg_notice_errors(client, msg, ERR_NOSUCHCHANNEL);
		return ;
	}
	if (find(client.get_channels().begin(), client.get_channels().end(), &(it->second)) 
		== client.get_channels().end())
	{
		privmsg_notice_errors(client, msg, ERR_CANNOTSENDTOCHAN);
		return ;
	}
	response += it->second.get_name() + " :";
	add_message(response, msg, 1, "default");
	send_to_channel(client, response, it->second, false);
}

/*
** Our Commands
*/

/*
** USERSALL
*/

void	Commands::cmd_usersall(Client& client, Message& msg)
{
	std::string	response;
	std::map<std::string, Client*>::const_iterator	it;

	(void)msg;
	response += add_kvirc_params(client, RPL_ENDOFSTATS);
	response += ": ";
	response += "LIST of ALL USERS:\r\n";
	response += add_kvirc_params(client, RPL_ENDOFSTATS);
	response += ": ";
	for (it = _user_list.begin(); it != _user_list.end(); it++)
	{
		response += " " + it->first;
	}
	response += "\r\n";
	response += add_kvirc_params(client, RPL_ENDOFSTATS);
	response += ": [End]\r\n";
	send(client.get_fd(), response.c_str(), response.size(), MSG_DONTWAIT  );
}

/*
** CHANSALL
*/

void	Commands::cmd_chansall(Client& client, Message& msg)
{
	std::string	response;
	std::map<std::string, Channel>::const_iterator	it;

	(void)msg;
	response += add_kvirc_params(client, RPL_ENDOFSTATS);
	response += ": ";
	response += "LIST of ALL CHANNELS:\r\n";
	response += add_kvirc_params(client, RPL_ENDOFSTATS);
	response += ": ";	
	for (it = _channels.begin(); it != _channels.end(); ++it)
	{
		response += " " + it->first;
	}
	response += "\r\n";
	response += add_kvirc_params(client, RPL_ENDOFSTATS);
	response += ": [End]\r\n";
	send(client.get_fd(), response.c_str(), response.size(), MSG_DONTWAIT  );
}

/*
** USERSINCHAN
*/

void	Commands::cmd_usersinchan(Client& client, Message& msg)
{

	std::list<Channel*>::const_iterator	channel;
	std::string							response;

	(void)msg;
	response += add_kvirc_params(client, RPL_ENDOFSTATS);
	response += ": ";
	response += "LIST of USERS in_the_same_CHANNEL(s):\r\n";
	for (channel = client.get_channels().begin(); channel != client.get_channels().end(); ++channel)
	{
		response += add_kvirc_params(client, RPL_ENDOFSTATS);
		response += ": ";
		response += "Channel " + (*channel)->get_name() + ":\r\n";
		response += add_kvirc_params(client, RPL_ENDOFSTATS);
		response += ": ";
		response += (*channel)->get_names();
		response += "\r\n";
	}
	response += add_kvirc_params(client, RPL_ENDOFSTATS);
	response += ": ";
	response += "LIST [End]\r\n";
	send(client.get_fd(), response.c_str(), response.size(), MSG_DONTWAIT  );
}

/*
** MYNAME
*/

void	Commands::cmd_myname(Client& client, Message& msg)
{
	std::string							response;
	std::list<Channel*>::const_iterator	channel;

	(void)msg;
	response += add_kvirc_params(client, RPL_ENDOFSTATS);
	response += ": ";
	response += "personal_INFO [Beginning]" + (std::string)"\r\n";		
	response += add_kvirc_params(client, RPL_ENDOFSTATS);
	response += ": ";
	response += "\tyour_nick:................................................" + client.get_nickname() + "\r\n";
	response += add_kvirc_params(client, RPL_ENDOFSTATS);
	response += ": ";
	response += "\tyour_username:......................................" + client.get_username() + "\r\n";
	response += add_kvirc_params(client, RPL_ENDOFSTATS);
	response += ": ";
	response += "\tyour usermode:......................................." + client.get_mode() + "\r\n";
	response += add_kvirc_params(client, RPL_ENDOFSTATS);
	response += ": ";
	response += "\tyour real name:......................................." + client.get_realname() + "\r\n";
	response += add_kvirc_params(client, RPL_ENDOFSTATS);
	response += ": ";
	response += "\tchannels you are in:................................";
	for (channel = client.get_channels().begin(); channel != client.get_channels().end(); ++channel)
		response += (*channel)->get_name() + " ";
	if (client.get_n_channels() == 0)
		response += "(none)";
	response += "\r\n";
	response += add_kvirc_params(client, RPL_ENDOFSTATS);
	response += ": ";
	std::stringstream ss;
	ss << client.get_n_channels();
	response += "\tnumber of channels you are in:.............." + ss.str() + (std::string)"\r\n";
	response += add_kvirc_params(client, RPL_ENDOFSTATS);
	response += ": ";
	response += "personal_INFO [End]\r\n";
	send(client.get_fd(), response.c_str(), response.size(), MSG_DONTWAIT  );
}

/*
** HELP
*/

void	Commands::cmd_help(Client& client, Message& msg)
{
	std::string	response;

	(void)msg;
	response = add_kvirc_params(client, RPL_COMMANDSYNTAX);
	response += "for available_commands:\r\n";
	response += add_kvirc_params(client, RPL_ENDOFSTATS);
	response += "1 : 1) PASS <password>....................................................give valid password\r\n";
	response += add_kvirc_params(client, RPL_ENDOFSTATS);
	response += "2 : 2) NICK <nickname>....................................................set your nick\r\n";
	response += add_kvirc_params(client, RPL_ENDOFSTATS);
	response += "3 : 3) :old_nick NICK new_nick...........................................change your nick\r\n";
	response += add_kvirc_params(client, RPL_ENDOFSTATS);
	response += "4 : 4) USER <user> 0 * <realname>.................................create new user\r\n";
	response += add_kvirc_params(client, RPL_ENDOFSTATS);
	response += "5 : 5) JOIN <#channel>......................................................join channel to talk\r\n";
	response += add_kvirc_params(client, RPL_ENDOFSTATS);
	response += "6 : 6) JOIN 0.........................................................................leave all channels\r\n";
	response += add_kvirc_params(client, RPL_ENDOFSTATS);
	response += "7 : 7) PRIVMSG <nick> <text>..........................................send private message to user\r\n";
	response += add_kvirc_params(client, RPL_ENDOFSTATS);
	response += "8 : 8) PRIVMSG <channel> <text>....................................send private message to channel\r\n";
	response += add_kvirc_params(client, RPL_ENDOFSTATS);
	response += "9 : 9) NOTICE <nick> <text>............................................send notice message to user\r\n";
	response += add_kvirc_params(client, RPL_ENDOFSTATS);
	response += "10 : 10) NOTICE <channel> <text>....................................send notice message to channel\r\n";
	response += add_kvirc_params(client, RPL_ENDOFSTATS);
	response += "11 : 11) KICK <channel> <user> [optional: comment]......kick user from channel\r\n";
	response += add_kvirc_params(client, RPL_ENDOFSTATS);
	response += "12 : 12) USERSALL................................................................show all users\r\n";
	response += add_kvirc_params(client, RPL_ENDOFSTATS);
	response += "13 : 13) CHANSALL...............................................................show all created channels\r\n";
	response += add_kvirc_params(client, RPL_ENDOFSTATS);
	response += "14 : 14) USERSINCHAN.........................................................show all users in your channel(s)\r\n";
	response += add_kvirc_params(client, RPL_ENDOFSTATS);
	response += "15 : 15) MYNAME..................................................................show info about you\r\n";
	response += add_kvirc_params(client, RPL_ENDOFSTATS);
	response += "16 : 16) BOT..........................................................................activate the bot\r\n";
	response += add_kvirc_params(client, RPL_ENDOFSTATS);
	response += "17 : 17) QUIT [optional: quit message]................................exit\r\n";
	response += add_kvirc_params(client, RPL_ENDOFSTATS);
	response += "commands [End]\r\n";
	send(client.get_fd(), response.c_str(), response.size(), MSG_DONTWAIT  );
}

/*
** BONUSES
*/

void	Commands::cmd_bot(Client &client, Message &msg)
{
	std::string	response;

	(void)msg;
	client.activate_the_bot();
	response = add_kvirc_params(client, RPL_ENDOFSTATS);
	response += ": Hello, " + client.get_nickname() + " I_am_the_bot\r\n";
	response += add_kvirc_params(client, RPL_ENDOFSTATS);
	response += ": ";
	response += "write:______________start_____________to_get \"Tip_of_the_day\"\r\n";
	response += add_kvirc_params(client, RPL_ENDOFSTATS);
	response += ": ";
	response += "write:______________stop_____________to switch_me_off (this will finish my work)\r\n";
	send(client.get_fd(), response.c_str(), response.size(), MSG_DONTWAIT  );
}

void	Commands::cmd_start(Client &client, Message &msg)
{
	std::string	response;
	int			rand_value;

	if (client.get_bot_status() == false)
	{
		response = add_kvirc_params(client, ERR_UNKNOWNCOMMAND);
		response += msg._command + " :Unknown command\r\n";
	}
	else
	{
		srand(time(0));
		rand_value = 1 + rand() % 3;
		response += add_kvirc_params(client, RPL_ENDOFSTATS);
		response += ": ";
		if (rand_value == 1)
			response += "The_leopard_cannot_change_his_spots\r\n";
		else if (rand_value == 2)
			response += "Nothing_is_impossible_to_a_willing_heart\r\n";
		else
			response += "One_swallow_does_not_make_a_summer\r\n";
	}
	send(client.get_fd(), response.c_str(), response.size(), MSG_DONTWAIT  );
}

void	Commands::cmd_stop(Client &client, Message &msg)
{
	std::string	response;

	if (client.get_bot_status() == true)
	{
		client.deactivate_the_bot();
		response = add_kvirc_params(client, RPL_ENDOFSTATS);
		response += ": ";
		response += "Bot_sais_to_you:\t\"Good_bye, bye-bye\"" + (std::string)"\r\n";
	}
	else
	{
		response = add_kvirc_params(client, ERR_UNKNOWNCOMMAND);
		response += msg._command + " :Unknown command\r\n";
	}	
	send(client.get_fd(), response.c_str(), response.size(), MSG_DONTWAIT  );
}

/*
** test cases
** see how KVIrc handles different codes of commands
*/

/*
** Download KVirc from here:
** https://github.com/kvirc/KVIrc/
**
** See these: folder and files:
** KVIrc-5.0.0/src/kvirc/sparser/
**
** KVIrc-5.0.0/src/kvirc/sparser/KviIrcServerParser_numericHandlers.cpp // handling of the each sertain command (the code identifies it)
** KVIrc-5.0.0/src/kvirc/sparser/KviIrcServerParser_tables.cpp // which codes (commands) are handled. Not all.
*/

void	Commands::cmd_test0(Client &client, Message &msg)
{
	std::string	response;

	(void)msg;
	response = "test0 test0\r\n";
	send(client.get_fd(), response.c_str(), response.size(), MSG_DONTWAIT  );
}

void	Commands::cmd_test1(Client &client, Message &msg)
{
	std::string	response;

	(void)msg;
	response = add_kvirc_params(client, 020);
	response += "test1 test1\r\n";
	send(client.get_fd(), response.c_str(), response.size(), MSG_DONTWAIT  );
}

void	Commands::cmd_test2(Client &client, Message &msg)
{
	std::string	response;

	(void)msg;

	response += add_kvirc_params(client, 219); // == RPL_ENDOFSTATS
	response += "test2 test2\r\n";	
	send(client.get_fd(), response.c_str(), response.size(), MSG_DONTWAIT  );
}

void	Commands::cmd_test3(Client &client, Message &msg)
{
	std::string	response;

	(void)msg;
	response = add_kvirc_params(client, 322);
	response += "test3 test3\r\n";
	send(client.get_fd(), response.c_str(), response.size(), MSG_DONTWAIT  );
}

void	Commands::cmd_test4(Client &client, Message &msg)
{
	std::string	response;

	(void)msg;
	response = add_kvirc_params(client, 332);
	response += "test4 test4\r\n";
	send(client.get_fd(), response.c_str(), response.size(), MSG_DONTWAIT  );
}

void	Commands::cmd_test5(Client &client, Message &msg)
{
	std::string	response;

	(void)msg;
	response = add_kvirc_params(client, 341);
	response += "test5 test5\r\n";
	send(client.get_fd(), response.c_str(), response.size(), MSG_DONTWAIT  );
}

void	Commands::cmd_test6(Client &client, Message &msg)
{
	std::string	response;

	(void)msg;
	response = add_kvirc_params(client, 352);
	response += "test6 test6\r\n";
	send(client.get_fd(), response.c_str(), response.size(), MSG_DONTWAIT  );
}

void	Commands::cmd_test7(Client &client, Message &msg)
{
	std::string	response;

	(void)msg;
	response = add_kvirc_params(client, 353);
	response += "test7 test7\r\n";
	send(client.get_fd(), response.c_str(), response.size(), MSG_DONTWAIT  );
}

void	Commands::cmd_test8(Client &client, Message &msg)
{
	std::string	response;

	(void)msg;
	response = add_kvirc_params(client, 386);
	response += "test8 test8\r\n";
	send(client.get_fd(), response.c_str(), response.size(), MSG_DONTWAIT  );
}

void	Commands::cmd_test9(Client &client, Message &msg)
{
	std::string	response;

	(void)msg;
	response = add_kvirc_params(client, 728);
	response += "test9 test9\r\n";
	send(client.get_fd(), response.c_str(), response.size(), MSG_DONTWAIT  );
}
