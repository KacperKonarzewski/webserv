/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaszuba <mkaszuba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 12:48:01 by kkonarze          #+#    #+#             */
/*   Updated: 2025/08/05 21:34:44 by mkaszuba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef CONFIGPARSER_HPP
#define CONFIGPARSER_HPP

#include "Config.hpp"
#include <string>
#include <vector>
#include <map>
#include <fstream>

class ConfigParser
{
private:
	std::vector<Config>	configs;

	int			block_num;
	std::string	token;
	std::string	remainder;
	std::map<std::string, void (ConfigParser::*)(int)>	tokens;

	void	read_listen(int line_num);
	void 	read_server_name(int line_num);
	void 	read_client_max_body_size(int line_num);
	void 	read_error_page(int line_num);
	void    read_location(int line_num);
	void    read_root(int line_num);
	void    read_index(int line_num);
	void	read_server(int line_num);
	void 	fill_tokens();
	void	remove_semicolon(int line_num, std::string &value, std::string communicate);
	
	size_t		parse_size(const std::string& s);
	void		tokenize(const std::string& line);
	__attribute__((noreturn)) void	parser_error(const std::string message, int line_num);
public:
	ConfigParser(const std::string& filepath);
	ConfigParser();
	~ConfigParser();

	const	std::vector<Config>&		get_configs() const;
};


#endif