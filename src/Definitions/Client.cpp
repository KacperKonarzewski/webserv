/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkonarze <kkonarze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 13:16:44 by kkonarze          #+#    #+#             */
/*   Updated: 2025/08/13 23:12:56 by kkonarze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

#include "Client.hpp"
#include "Server.hpp"
#include "Request.hpp"


bool Client::read_request()
{
	if (request != NULL)
		delete request;
	request = new Request();
	if (request->read_request(client_fd) == false)
		return (false);
	request->parse_request();
	return (true);
}

int	Client::get_client_fd()
{
	return (client_fd);
}

int	Client::get_blocking_flag()
{
	return (blocking_flag);
}

Request	*Client::get_request()
{
	return (request);
}

Server	*Client::get_server()
{
	return (server);
}

void	Client::set_server(Server *serv)
{
	server = serv;
}

Client::Client(Server *serv, EpollState &epoll_state)
{
	server = serv;
	request = NULL;
	client_fd = accept(server->get_server(), NULL, NULL);
	if (client_fd < 0) 
	{
		perror("accept");
		return ;
	}
	if (make_socket_non_blocking(client_fd) < 0)
	{
		perror("non blocking");
		blocking_flag = 1;
		return ;
	}
	blocking_flag = 0;
	epoll_state.info.events = EPOLLIN | EPOLLET | EPOLLRDHUP | EPOLLHUP;
	epoll_state.info.data.fd = client_fd;
	epoll_ctl(epoll_state.epoll_fd, EPOLL_CTL_ADD, client_fd, &epoll_state.info);
}

Client::~Client()
{
	if (request != NULL)
		delete request;
}
