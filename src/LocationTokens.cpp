/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationTokens.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaszuba <mkaszuba@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 03:50:24 by kkonarze          #+#    #+#             */
/*   Updated: 2025/08/07 11:40:33 by mkaszuba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"
#include <iostream>
#include <sstream>

void Location::read_allowed_methods(int line_num, std::string& reminder)
{
	std::string	methods_value;

	extract_value(line_num, reminder, methods_value, "allowed_methods");
	validate_methods(line_num, methods_value);
}

void Location::validate_methods(int line_num, std::string& reminder)
{
	std::string method;
	std::istringstream methods_stream(reminder);
	bool has_valid_method = false;

	if (reminder.empty())
		parser_error("Empty 'allowed_methods' directive.", line_num);

	while (methods_stream >> method)
	{
		if (method != "GET" && method != "POST" && method != "DELETE")
			parser_error("Invalid method '" + method + "' in 'allowed_methods' directive.", line_num);
		has_valid_method = true;
	}

	if (!has_valid_method)
		parser_error("No valid methods in 'allowed_methods' directive.", line_num);
}

void Location::read_upload_dir(int line_num, std::string& reminder)
{
	std::string	upload_dir_value;
	
	extract_value(line_num, reminder, upload_dir_value, "upload_dir");
}

void Location::read_cgi_extension(int line_num, std::string& reminder)
{
	std::string	cgi_extension_value;
	
	extract_value(line_num, reminder, cgi_extension_value, "cgi_extension");
}

void Location::read_directory_listing(int line_num, std::string& reminder)
{
	std::string	directory_listing_value;

	extract_value(line_num, reminder, directory_listing_value, "directory_listing");

	if (directory_listing_value != "on" && directory_listing_value != "off")
		parser_error("Invalid 'directory_listing' value. Expected 'on' or 'off'.", line_num);
}
