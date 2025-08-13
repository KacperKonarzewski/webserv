/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkonarze <kkonarze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 00:48:40 by kkonarze          #+#    #+#             */
/*   Updated: 2025/08/13 23:45:27 by kkonarze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Webserv.hpp"

#include "Server.hpp"
#include "Signal.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "ServerManager.hpp"

Server::~Server()
{
    close(server_fd);
}

Server::Server(const Config& conf, int i) : conf(conf)
{
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd == -1)
		error("socket error.");

	int opt = 1;
	setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	address.sin_family = AF_INET;
	address.sin_port = htons(conf.listen_addresses[i].port);

	if (inet_pton(AF_INET, conf.listen_addresses[i].host.c_str(), &address.sin_addr) <= 0)
		error("invalid address: " + conf.listen_addresses[i].host);
	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
		error("bind error.");
	if (listen(server_fd, 10) < 0)
		error("listen error.");
	addrlen = sizeof(address);
	std::cout << "Serwer działa na http://" << conf.listen_addresses[i].host << ":" << conf.listen_addresses[i].port << std::endl;
}

void Server::send_response(Client *client)
{
	Response response(client, conf);

    send(client->get_client_fd(), response.get_response().c_str(), response.get_response().size(), 0);
}

Client	*Server::accept_client(EpollState &epoll_state)
{
	Client *client = new Client(this, epoll_state);
	
	if (client->get_client_fd() < 0)
		return (NULL);
	if (client->get_blocking_flag())
		return (NULL);
	return (client);
}

int &Server::get_server()
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

const Config &Server::get_conf()	const
{
	return (conf);
}

Server::Server(Server& serv) : conf(serv.conf)
{
	if (this == &serv)
		return ;
	this->server_fd = serv.get_server();
	this->address = serv.get_address();
	this->addrlen = serv.get_addrlen();
}
