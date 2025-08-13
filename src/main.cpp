/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkonarze <kkonarze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 12:48:19 by kkonarze          #+#    #+#             */
/*   Updated: 2025/08/12 21:01:24 by kkonarze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

#include "Server.hpp"
#include "Signal.hpp"
#include "ConfigParser.hpp"

int main(int argc, char *argv[])
{
    std::string config_filepath = "default.conf";

    if (argc > 1) 
        config_filepath = argv[1];

    try {
        ConfigParser config(config_filepath);
		
        setup_signals();
		
		Server *server = new Server(config.get_configs()[0]);
		server->init_epoll();
		server->event_loop();
		delete server;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
