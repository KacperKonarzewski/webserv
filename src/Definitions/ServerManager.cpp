/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManager.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkonarze <kkonarze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 17:48:57 by kkonarze          #+#    #+#             */
/*   Updated: 2025/08/13 20:34:37 by kkonarze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

#include "ServerManager.hpp"
#include "Signal.hpp"

void ServerManager::init_epoll()
{
	int									current_server;
	std::map<int, Server*>::iterator	it;

	epoll_state.epoll_fd = epoll_create1(0);
	if (epoll_state.epoll_fd == -1)
		return error("epoll_create error.");

	epoll_state.info.events = EPOLLIN;
	for (it = servers.begin(); it != servers.end(); it++)
	{
		current_server = it->second->get_server();
		epoll_state.info.data.fd = current_server;
		if (epoll_ctl(epoll_state.epoll_fd, EPOLL_CTL_ADD, current_server, &epoll_state.info) == -1)
			return error("epoll_ctl error.");
	}
}

Client *ServerManager::find_client(int event_fd)
{
	std::map<int, Client*>::iterator it = clients.find(event_fd);

	if (it == clients.end())
		return (NULL);
	return (it->second);
}

void ServerManager::event_loop()
{
	Client								*client;
	std::map<int, Server*>::iterator	it;

	while (g_signal_state.sigint == 0 && g_signal_state.sigterm == 0)
	{
		epoll_state.num_of_fds = epoll_wait(epoll_state.epoll_fd, epoll_state.events, 10, -1);
		if (epoll_state.num_of_fds == -1 && errno != EINTR)
			return error("epoll_wait error.");
		for (int x = 0; x < epoll_state.num_of_fds; x++)
		{
			it = servers.find(epoll_state.events[x].data.fd);
			if (it != servers.end())
			{
				client = it->second->accept_client(epoll_state);
				if (client == NULL)
					continue ;
				clients[client->get_client_fd()] = client;
			}
			else
			{
				client = find_client(epoll_state.events[x].data.fd);
				if (client == NULL)
					continue ;
				client->read_request();
				//send_response(client);
			}
		}
	}
}

EpollState	&ServerManager::get_epoll_state()
{
	return (epoll_state);
}