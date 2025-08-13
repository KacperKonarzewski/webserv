/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkonarze <kkonarze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 01:45:54 by kkonarze          #+#    #+#             */
/*   Updated: 2025/08/13 23:04:32 by kkonarze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef SERVER_H
#define SERVER_H

#include "Webserv.hpp"

#include "Client.hpp"
#include "Config.hpp"
#include "EpollState.hpp"

class Client;

class Server
{
private:
	int					server_fd;
	struct sockaddr_in	address;

	const Config&		conf;
	socklen_t			addrlen;

public:
	~Server();
	Server(const Config& conf, int i);
	Server(Server& serv);

	void		init_epoll();
	void		event_loop();
	Client		*accept_client(EpollState &epoll_state);
	void		send_response(Client *client);

	int						&get_server();
	const Config			&get_conf()	const;
	sockaddr_in				&get_address();
	socklen_t				&get_addrlen();
};

#endif