/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManager.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkonarze <kkonarze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 17:48:57 by kkonarze          #+#    #+#             */
/*   Updated: 2025/08/13 23:13:22 by kkonarze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

#include "ServerManager.hpp"
#include "Signal.hpp"
#include "Request.hpp"

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

void ServerManager::remove_client(int event_fd)
{
	std::map<int, Client*>::iterator it;
	
	it = clients.find(event_fd);
	if (it != clients.end())
	{
		epoll_ctl(epoll_state.epoll_fd, EPOLL_CTL_DEL, it->second->get_client_fd(), &epoll_state.info);
		if (it->second)
			delete it->second;
		clients.erase(it);
	}
}

Client *ServerManager::find_client(int event_fd)
{
	std::map<int, Client*>::iterator it = clients.find(event_fd);

	if (it == clients.end())
		return (NULL);
	return (it->second);
}

void ServerManager::check_host(Client *client)
{
	Request											*request = client->get_request();
	std::map<std::string, std::string>				tokens = request->get_tokens();
	std::string										host;
	std::map<std::string, std::string>::iterator	it;
	std::map<int, Server*>::iterator				it2;
	std::vector<std::string>::const_iterator		it3;

	it = tokens.find("Host");
	if (it == tokens.end())
		return ;
	for (it2 = servers.begin(); it2 != servers.end(); it2++)
	{
		it3 = std::find(it2->second->get_conf().server_names.begin(), it2->second->get_conf().server_names.end(), it->second);
		if (it3 == it2->second->get_conf().server_names.end())
			continue ;
		client->set_server(it2->second);
		return ;
	}
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
				if (client)
					clients[client->get_client_fd()] = client;
				continue;
			}
			client = find_client(epoll_state.events[x].data.fd);
			if (!client || epoll_state.events[x].events & (EPOLLRDHUP | EPOLLHUP | EPOLLERR) || client->read_request() == false)
			{
				remove_client(epoll_state.events[x].data.fd);
				continue ;
			}
			check_host(client);
			client->get_server()->send_response(client);
		}
	}
}

EpollState	&ServerManager::get_epoll_state()
{
	return (epoll_state);
}

ServerManager::ServerManager(std::map<int, Server*> &servers) : servers(servers)
{
	init_epoll();
}

ServerManager::~ServerManager()
{
	std::map<int, Server*>::iterator it;
	std::map<int, Client*>::iterator it2;

	for (it2 = clients.begin(); it2 != clients.end(); it2++)
	{
		epoll_ctl(epoll_state.epoll_fd, EPOLL_CTL_DEL, it2->first, &epoll_state.info);
		delete (it2->second);
	}
	
	for (it = servers.begin(); it != servers.end(); it++)
	{
		epoll_ctl(epoll_state.epoll_fd, EPOLL_CTL_DEL, it->first, &epoll_state.info);
		delete (it->second);
	}
}