/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkonarze <kkonarze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 00:48:40 by kkonarze          #+#    #+#             */
/*   Updated: 2025/08/06 00:18:58 by kkonarze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Webserv.hpp"
#include "Signal.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <cstdio>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <errno.h>
#include <fstream>

Server::~Server()
{
	close(server_fd);
}

Server::Server(const Config& conf) : conf(conf)
{
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd == -1)
		error("socket error.");
	int opt = 1;
	
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(conf.listen_addresses.back().port);
	
	setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
		error("bind error.");
	if (listen(server_fd, 10) < 0)
		error("listen error.");
	addrlen = sizeof(address);
	std::cout << "Serwer działa na http://localhost:" << conf.listen_addresses.back().port << std::endl;
}

void Server::init_epoll()
{
	epoll_fd = epoll_create1(0);
	if (epoll_fd == -1)
		return error("epoll_create error.");

	info.events = EPOLLIN;
	info.data.fd = server_fd;
	if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &info) == -1)
		return error("epoll_ctl error.");
}

void Server::send_response(Client *client)
{
	Response response(client, conf);

    send(client->get_client_fd(), response.get_response().c_str(), response.get_response().size(), 0);
	close(client->get_client_fd());
	epoll_ctl(get_epoll_fd(), EPOLL_CTL_DEL, client->get_client_fd(), NULL);
}

void Server::event_loop()
{
	Client *client;

	while (g_signal_state.sigint == 0 && g_signal_state.sigterm == 0)
	{
		num_of_fds = epoll_wait(epoll_fd, events, 10, -1);
		if (num_of_fds == -1 && errno != EINTR)
			return error("epoll_wait error.");
		for (int x = 0; x < num_of_fds; x++)
		{
			if (events[x].data.fd == server_fd)
				Client::accept_client(*this);
			else
			{
				client = Client::find_client(*this, events[x].data.fd);
				if (client == NULL)
					continue ;
				client->read_request();
				send_response(client);
			}
		}
	}
}

int Server::get_server()
{
	return (server_fd);
};

sockaddr_in &Server::get_address()
{
	return (address);
};

socklen_t &Server::get_addrlen()
{
	return (addrlen);
};

epoll_event	&Server::get_info()
{
	return (info);
}

epoll_event	*Server::get_events()
{
	return (events);
}

int	Server::get_epoll_fd()
{
	return (epoll_fd);
}

std::map<int, Client> &Server::get_clients()
{
	return (clients);
}

Server::Server(Server& serv) : conf(serv.conf)
{
	if (this == &serv)
		return ;
	this->server_fd = serv.get_server();
	this->address = serv.get_address();
	this->addrlen = serv.get_addrlen();
}
