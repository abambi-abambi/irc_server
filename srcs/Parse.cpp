/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parse.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abambi <abambi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/29 16:07:39 by abambi            #+#    #+#             */
/*   Updated: 2022/08/19 11:19:47 by abambi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parse.hpp"

Parse::Parse(void) : Commands()
{
	_servname = "ircserv";
}

Parse::~Parse(void)
{

}

size_t skip_white_spaces(std::string str, size_t i)
{
	while (str[i] == ' ' || str[i] == '\t')
		++i;
	return (i);
}

Parse::Message	Parse::split_message(Parse::Message &msg, std::string part, size_t len, size_t k)
{
	if (part[k] == ':' &&
		msg._prefix.empty() && 
		msg._command.empty())
	{
		msg._prefix = part.substr(k + 1, len - 1);
	}
	else if (msg._command.empty())
		msg._command = part.substr(k, len - k);
	else
	{
		if (part[k] == ':')
			++k;
		msg._paramlist.push_back(part.substr(k, len - k));
	}
	cmd_toupper(msg);
	return (msg);
}

Parse::Message	Parse::cmd_toupper(Parse::Message &msg)
{
	for (int i = 0; (msg._command)[i]; ++i)
		(msg._command)[i] = toupper((msg._command)[i]);
	return (msg);
}

void Parse::message_free(Parse::Message &msg)
{
	msg._prefix.clear();
	msg._command.clear();
	msg._paramlist.clear();
}

void	Parse::exec_command(Client& client)
{
	std::string &buf = client.get_buffer();
	std::string	part;
	Message		msg;
	std::queue<Message>	msgq;

	size_t		len_world;
	size_t		end_index;
	size_t		k;
	
	for (size_t i = 0; buf[i]; i = end_index + strlen("\r\n"))
	{
		end_index = buf.find("\r\n", i);
		part = buf.substr(i, end_index - i);
		if (end_index == std::string::npos)
			break;
		k = 0;
		while (k != std::string::npos && msg._paramlist.size() <= 15)
		{
			k = skip_white_spaces(part, k);
			if (!part[k])
				break;
			len_world = part.find(' ', k);
			msg = split_message(msg, part, len_world, k);
			k = len_world;
		}
		msgq.push(msg);
		message_free(msg);
	}
	if (end_index == std::string::npos)
		buf = part;
	else
		buf.clear();
	launch_cmd(msgq, client);
}

void	Parse::launch_cmd(std::queue<Message> msgq, Client& client)
{
	std::map<std::string, Command>::const_iterator	it;
	std::string	cmd;

	while (msgq.size())
	{
		cmd = msgq.front()._command;

		if (client.get_status() == true \
			|| cmd == "PASS" || cmd == "HELP" || cmd == "QUIT" \
			|| cmd == "CAP" || cmd == "PING") // adaptations for KVIrc
		{ // see more: https://ircv3.net/specs/extensions/capability-negotiation.html
			it = _commands.find(cmd);
			if (it != _commands.end())
				(this->*(it->second))(client, msgq.front());
			else
				send_error(client, msgq.front(), ERR_UNKNOWNCOMMAND);
		}
		else
			send_error(client, msgq.front(), ERR_NOTREGISTERED);
		msgq.pop();
	}
}

void	Parse::send_error(Client& client, Message& msg, int error)
{
	std::string			response;

	response = add_kvirc_params(client, error);	
	if (error == ERR_NOTREGISTERED)
		response += ":You have not registered\r\n";
	else
		response += msg._command + " :Unknown command\r\n";
	send(client.get_fd(), response.c_str(), response.size(), MSG_DONTWAIT);
}
