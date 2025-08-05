/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigTokens.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaszuba <mkaszuba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 12:48:15 by kkonarze          #+#    #+#             */
/*   Updated: 2025/08/05 21:42:50 by mkaszuba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigParser.hpp"
#include "Webserv.hpp"
#include "Location.hpp" 
#include <sstream>

void ConfigParser::read_listen(int line_num)
{
	std::string 	listen_value;
	size_t			colon_pos;
	ListenAddress	address;
	
	remove_semicolon(line_num, listen_value, "listen");
	
	colon_pos = listen_value.find(':');

	address.host = (colon_pos == std::string::npos)? "0.0.0.0" : listen_value.substr(0, colon_pos);
	address.port = string_to_int((colon_pos == std::string::npos)? listen_value : listen_value.substr(colon_pos + 1));
	configs.back().listen_addresses.push_back(address);
}

void ConfigParser::read_server_name(int line_num)
{
	std::string	trimmed_name;

	remove_semicolon(line_num, trimmed_name, "server_name");
	this->configs.back().server_names = ft_split(trimmed_name, " ");
}

void ConfigParser::read_client_max_body_size(int line_num)
{
	std::string	size_str;

	remove_semicolon(line_num, size_str, "client_max_body_size");
	configs.back().client_max_body_size = parse_size(size_str);
}

void ConfigParser::read_error_page(int line_num)
{
	std::string	error_page_str;
	std::string	error_code_str;
	std::string	error_page_path;
	size_t		space_pos;
	int			error_code;

	remove_semicolon(line_num, error_page_str, "error_page");
	space_pos = error_page_str.find(' ');
	if (space_pos == std::string::npos)
		parser_error("Invalid 'error_page' format. Expected 'error_page CODE PATH;'", line_num);

	error_code_str = error_page_str.substr(0, space_pos);
	error_page_path = error_page_str.substr(space_pos + 1);
	trim_whitespace(error_code_str);
	trim_whitespace(error_page_path);

	error_code = string_to_int(error_code_str);
	configs.back().error_pages[error_code] = error_page_path;
}

void ConfigParser::read_location(int line_num)
{
    size_t		open_brace_pos = remainder.find('{');
	std::string	location_path;

	if (block_num == 0)
		parser_error("Not inside server block.", line_num);
    if (open_brace_pos == std::string::npos)
        parser_error("Missing '{' after 'location' directive.", line_num);

    location_path = remainder.substr(0, open_brace_pos);
    trim_whitespace(location_path);

	configs.back().locations.push_back(Location(location_path));
    block_num++;
}

void ConfigParser::read_server(int line_num)
{
	Config c;
	
	if (block_num != 0)
		parser_error("Server block inside of server block on line: ", line_num);
	if (remainder != "{")
		parser_error("Expected 'server {'", line_num);
	configs.push_back(c);
	block_num++;
}

void ConfigParser::read_root(int line_num)
{
	std::string	root_value;

	remove_semicolon(line_num, root_value, "root");
	trim_whitespace(root_value);

	if (block_num == 2)
		configs.back().locations.back().add_token("root", root_value);
}

void ConfigParser::read_index(int line_num)
{
	std::string	index_value;

	remove_semicolon(line_num, index_value, "index");
	if (block_num == 2)
		configs.back().locations.back().add_token("index", index_value);
}

void ConfigParser::fill_tokens()
{
	tokens["listen"] = &ConfigParser::read_listen;
	tokens["server_name"] = &ConfigParser::read_server_name;
	tokens["client_max_body_size"] = &ConfigParser::read_client_max_body_size;
	tokens["error_page"] = &ConfigParser::read_error_page;
	tokens["location"] = &ConfigParser::read_location;
	tokens["server"] = &ConfigParser::read_server;
	tokens["root"] = &ConfigParser::read_root;
	tokens["index"] = &ConfigParser::read_index;
}

