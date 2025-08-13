/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkonarze <kkonarze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 12:48:19 by kkonarze          #+#    #+#             */
/*   Updated: 2025/08/13 22:28:13 by kkonarze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

#include "Server.hpp"
#include "Signal.hpp"
#include "ConfigParser.hpp"
#include "ServerManager.hpp"

int main(int argc, char *argv[])
{
	std::map<int, Server*>	servers;
    std::string				config_filepath = "default.conf";
	std::vector<Config>		configs;
	Server					*server;
	ServerManager			*manager;

    if (argc > 1) 
        config_filepath = argv[1];

    try {
        ConfigParser config(config_filepath);
		
        setup_signals();
		
		configs = config.get_configs();
		for (size_t i = 0; i < configs.size(); i++)
		{
			for (size_t j = 0; j < configs[i].listen_addresses.size(); j++)
			{
				server = new Server(config.get_configs()[i], j);
				servers[server->get_server()] = server;
			}
		}

		manager = new ServerManager(servers);
		manager->event_loop();
		
		delete manager;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
