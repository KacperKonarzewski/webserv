/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaszuba <mkaszuba@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 00:48:40 by kkonarze          #+#    #+#             */
/*   Updated: 2025/08/08 20:18:49 by mkaszuba         ###   ########.fr       */
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
    for (size_t i = 0; i < server_fd.size(); ++i)
        close(server_fd[i]);
}

Server::Server(const Config& conf) : conf(conf)
{
	int		opt = 1;
	int		fd;
	size_t	i = 0;

	while(i < conf.listen_addresses.size())
	{
		fd = socket(AF_INET, SOCK_STREAM, 0);
		if (fd == -1)
			error("socket error.");
		
		sockaddr_in addr{};
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = inet_addr(conf.listen_addresses[i].host.c_str());
		addr.sin_port = htons(conf.listen_addresses[i].port);
		
		setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
		
		if (bind(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
			error("bind error.");
		if (listen(fd, 10) < 0)
			error("listen error.");

		server_fd.push_back(fd);
		addresses.push_back(addr);
		
		std::cout << "Serwer działa na http://" << conf.listen_addresses[i].host << ":" << conf.listen_addresses[i].port << std::endl;
		++i;
	}
	
	addrlen = sizeof(sockaddr_in);
}

void Server::init_epoll()
{
	size_t	i = 0;
	epoll_fd = epoll_create1(0);
	if (epoll_fd == -1)
		return error("epoll_create error.");

	for (size_t i = 0; i < server_fd.size(); ++i)
	{
		info.events = EPOLLIN;
		info.data.fd = server_fd[i];
		if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd[i], &info) == -1)
			return error("epoll_ctl error.");
	}
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
	bool	is_listening_fd;

	while (g_signal_state.sigint == 0 && g_signal_state.sigterm == 0)
	{
		num_of_fds = epoll_wait(epoll_fd, events, 10, -1);
		if (num_of_fds == -1 && errno != EINTR)
			return error("epoll_wait error.");
		for (int x = 0; x < num_of_fds; x++)
		{
			is_listening_fd = false;
			for (size_t i = 0; i < server_fd.size(); i++)
			{
				if (events[x].data.fd == server_fd[i])
				{
					is_listening_fd = true;
					break;
				}
			}
			if(is_listening_fd)
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

std::vector<int> &Server::get_server()
{
	return (server_fd);
};

std::vector<sockaddr_in> &Server::get_addresses()
{
	return (addresses);
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
	this->addresses = serv.get_addresses();
	this->addrlen = serv.get_addrlen();
}
