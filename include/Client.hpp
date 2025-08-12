/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkonarze <kkonarze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 13:13:03 by kkonarze          #+#    #+#             */
/*   Updated: 2025/08/12 20:57:40 by kkonarze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Server.hpp"
#include "Webserv.hpp"

class Server;
class Request;

class Client
{
private:
	int			client_fd;
	int			blocking_flag;
	Request		*request;
public:
	static int		accept_client(Server &serv);
	static Client	*find_client(Server &serv, int event_fd);
	void			read_request();
	int				get_client_fd();
	int				get_blocking_flag();

	Request			*get_request();

	Client(Server &serv);
	~Client();
};

#endif