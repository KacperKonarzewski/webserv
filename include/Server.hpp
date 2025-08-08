/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaszuba <mkaszuba@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 01:45:54 by kkonarze          #+#    #+#             */
/*   Updated: 2025/08/08 20:17:54 by mkaszuba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef SERVER_H
#define SERVER_H

#include <arpa/inet.h>
#include <sys/epoll.h>
#include <map>
#include "Client.hpp"
#include "Config.hpp"

class Client;

class Server
{
private:
	std::vector<int>	server_fd;
	std::vector<struct sockaddr_in>	addresses;

	const Config&		conf;
	socklen_t			addrlen;

	int					epoll_fd;
	int					num_of_fds;
	struct epoll_event	info;
	struct epoll_event	events[10];

	std::map<int, Client> clients;
	
public:
	~Server();
	Server(const Config& conf);
	Server(Server& serv);

	void		init_epoll();
	void		event_loop();
	void		send_response(Client *client);

	std::vector<int> &Server::get_server();
	int						get_epoll_fd();
	std::vector<sockaddr_in> &Server::get_addresses();
	socklen_t				&get_addrlen();
	epoll_event				&get_info();
	epoll_event				*get_events();		
	std::map<int, Client>	&get_clients();
};

#endif