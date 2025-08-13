/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkonarze <kkonarze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 13:13:03 by kkonarze          #+#    #+#             */
/*   Updated: 2025/08/13 23:12:51 by kkonarze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Webserv.hpp"

#include "Server.hpp"
#include "EpollState.hpp"

class Server;
class Request;

class Client
{
private:
	Server		*server;
	int			client_fd;
	int			blocking_flag;
	Request		*request;
public:
	bool			read_request();
	int				get_client_fd();
	int				get_blocking_flag();

	Request			*get_request();
	Server			*get_server();

	void			set_server(Server *serv);

	Client(Server *serv, EpollState &epoll_state);
	~Client();
};

#endif