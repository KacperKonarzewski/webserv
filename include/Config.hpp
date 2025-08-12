/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkonarze <kkonarze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 05:11:45 by kkonarze          #+#    #+#             */
/*   Updated: 2025/08/12 20:57:51 by kkonarze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "Location.hpp"
#include "Webserv.hpp"

struct ListenAddress {
	std::string host;
	int port;
};

struct Config {
	std::vector<ListenAddress> listen_addresses;
	size_t client_max_body_size;
	std::vector<std::string> server_names;
	std::map<int, std::string> error_pages;
	std::vector<Location> locations;
};

#endif