/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaszuba <mkaszuba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 05:11:45 by kkonarze          #+#    #+#             */
/*   Updated: 2025/08/05 19:56:04 by mkaszuba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "Location.hpp"
#include <string>
#include <map>
#include <vector>

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