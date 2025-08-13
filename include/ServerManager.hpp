/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkonarze <kkonarze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 20:49:24 by kkonarze          #+#    #+#             */
/*   Updated: 2025/08/13 22:26:41 by kkonarze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef SERVERMANAGER_HPP
#define SERVERMANAGER_HPP

#include "Webserv.hpp"

#include "Server.hpp"
#include "EpollState.hpp"

class ServerManager
{
private:
	std::map<int, Server*>	&servers;
	std::map<int, Client*>	clients;

	EpollState			epoll_state;

	void				init_epoll();
	void				check_host(Client *client);
	void 				remove_client(int event_fd);
	Client				*find_client(int event_fd);
public:
	void		event_loop();

	EpollState	&get_epoll_state();
	
	ServerManager(std::map<int, Server*> &servers);
	~ServerManager();
};



#endif