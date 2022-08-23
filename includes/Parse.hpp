/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parse.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abambi <abambi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/29 16:07:36 by abambi            #+#    #+#             */
/*   Updated: 2022/07/29 23:54:38 by abambi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_HPP
# define PARSE_HPP

# include "Client.hpp"
# include "Channel.hpp"
# include "Commands.hpp"

# include <map>
# include <queue>

# define ERR_UNKNOWNCOMMAND		421
# define ERR_NOTREGISTERED		451

class Parse : public Commands
{
	public:
		Parse(void);
		~Parse(void);

	protected:
		void		exec_command(Client& client);

	private:
		Parse(const Parse& src);
		Parse&		operator=(const Parse& rhs);

		void		launch_cmd(std::queue<Message> msgq, Client& client);
		Message		split_message(Message &msg, std::string	part, size_t len, size_t k);
		Message		cmd_toupper(Message &msg);
		void		message_free(Parse::Message &msg);
		void		send_error(Client& client, Message& msg, int error);
};

#endif
