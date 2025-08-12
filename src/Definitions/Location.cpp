/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkonarze <kkonarze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 04:02:09 by kkonarze          #+#    #+#             */
/*   Updated: 2025/08/12 21:00:01 by kkonarze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

#include "Location.hpp"

/**
 * Finds directive of location and adds it.
 *
 * @param line_num config file line on which token is.
 * @param token directive which should be added.
 * @param reminder reminder used for the directive.
 */
void Location::find_token(int line_num, std::string& token, std::string& reminder)
{
	size_t		semicolon_pos = reminder.find(';');
	std::string	value;

	if (token != "allowed_methods" && token != "upload_dir" && token != "cgi_extension" && token != "directory_listing")
		parser_error("Unknown directive '" + token + "' inside server block.", line_num);

	if (semicolon_pos == std::string::npos)
		parser_error(("Missing ';' after '") + token + "' directive.", line_num);

	value = reminder.substr(0, semicolon_pos);
	trim_whitespace(value);

	if (value.empty())
		parser_error(("Empty '") + token + "' directive.", line_num);

	if (token == "directory_listing" && (value != "on" && value != "off"))
		parser_error("Invalid 'directory_listing' value. Expected 'on' or 'off'.", line_num);
		
	directive[token] = value;
}

/**
 * Prints out a error message for parser and exits the program.
 *
 * @param message message to print on cerr.
 * @param line_num config file line on which there is error.
 */
__attribute__((noreturn)) void Location::parser_error(const std::string message, int line_num = -1)
{
	if (line_num >= 0)
		std::cerr << "Parser Error [Line " << line_num << "]: " << message << std::endl;
	else
		std::cerr << "Parser Error: " << message << std::endl;
	exit(EXIT_FAILURE);
}

/**
 * Adds token to location.
 *
 * @param token directive which should be added.
 * @param value value used for the directive.
 */
void Location::add_token(std::string token, std::string& value)
{
	directive[token] = value;
}

std::string	Location::get_location_path() const
{
	return (location_path);
}

std::map<std::string, std::string>	Location::get_directive() const
{
	return (directive);
}

Location::Location(std::string& location_path)
{
	this->location_path = location_path;
}


Location::Location()
{
	
}
Location::~Location()
{
	
}