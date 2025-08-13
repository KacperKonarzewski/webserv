/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkonarze <kkonarze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 12:48:11 by kkonarze          #+#    #+#             */
/*   Updated: 2025/08/12 20:59:52 by kkonarze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

#include "ConfigParser.hpp"

/**
 * Prints out a error message for parser and exits the program.
 *
 * @param message message to print on cerr.
 * @param line_num config file line on which there is error.
 */
__attribute__((noreturn)) void ConfigParser::parser_error(const std::string message, int line_num = -1)
{
	if (line_num >= 0)
		std::cerr << "Parser Error [Line " << line_num << "]: " << message << std::endl;
	else
		std::cerr << "Parser Error: " << message << std::endl;
	exit(EXIT_FAILURE);
}

/**
 * Converts string to byte amount while units.
 *
 * @param s string reference to byte amount.
 * @return Byte amount multiplied by units.
 */
size_t ConfigParser::parse_size(const std::string& s)
{
	size_t		multiplier = 1;
	size_t		len = s.length();
	std::string	num_str;
	char		unit;
	int			num;

	if (len == 0)
		parser_error("Empty size value");
	unit = s[len - 1];
	switch (unit)
	{
		case 'k': case 'K':
			multiplier = 1024;
			num_str = s.substr(0, len - 1);
			break ;
		case 'm': case 'M':
			multiplier = 1024 * 1024;
			num_str = s.substr(0, len - 1);
			break ;
		case 'g': case 'G':
			multiplier = 1024 * 1024 * 1024;
			num_str = s.substr(0, len - 1);
			break ;
		default:
			if (!isdigit(unit))
				parser_error("Invalid size unit '" + std::string(1, unit) + "'");
			num_str = s;
			break ;
	}
	try
	{
		num = string_to_int(num_str);
		if (num < 0)
			parser_error("Size cannot be negative: '" + s + "'");
		return num * multiplier;
	}
	catch (...)
	{
		parser_error("Invalid size format '" + s + "'");
	}
}

void	ConfigParser::tokenize(const std::string& line)
{
	size_t		first_space;

	first_space = line.find_first_of(" \t");
	token = (first_space == std::string::npos)? line : line.substr(0, first_space);
	remainder = (first_space == std::string::npos)? "" : line.substr(first_space);
	trim_whitespace(token);
	trim_whitespace(remainder);
}

void	ConfigParser::validate_methods(int line_num)
{
	Location	*last_location = &configs.back().locations.back();
	std::string	method;

	if (last_location->get_directive().count("allowed_methods") == 0)
	{
		last_location->get_directive()["allowed_methods"] = "GET POST DELETE";
		return ;
	}
	std::istringstream methods_stream(last_location->get_directive()["allowed_methods"]);

	while (methods_stream >> method)
		if (method != "GET" && method != "POST" && method != "DELETE")
			parser_error("Invalid method '" + method + "' in 'allowed_methods' directive.", line_num);
}

ConfigParser::~ConfigParser()
{

}

ConfigParser::ConfigParser()
{

}

ConfigParser::ConfigParser(const std::string& filepath)
{
	std::ifstream	file(filepath.c_str());
	std::string		line;
	int				line_num = 0;

	block_num = 0;
	if (!file.is_open())
		parser_error("Could not open configuration file '" + filepath + "'");
	fill_tokens();
	while (std::getline(file, line))
	{
		line_num++;
		remove_comment(line);
		trim_whitespace(line);
		if (line.empty())
			continue;
		tokenize(line);

		if (tokens.count(token))
			(this->*tokens[token])(line_num);
		else if (block_num == 2)
			configs.back().locations.back().find_token(line_num, token, remainder);
		else
			parser_error("Unknown directive '" + token + "' inside server block.", line_num);
	}
	file.close();
	if (block_num > 0)
		parser_error(std::string("Unexpected EOF. Missing '}' for ") + ((block_num == 1) ? "server" : "location") + " block.");
}

const std::vector<Config>&	ConfigParser::get_configs() const
{
	return configs;
}

void	ConfigParser::remove_semicolon(int line_num, std::string &value, std::string communicate)
{
	size_t	semicolon_pos = remainder.find(';');
	
	if (block_num == 0)
		parser_error("Not inside server block.", line_num);
	if (semicolon_pos == std::string::npos)
		parser_error(std::string("Missing ';' after '") + communicate + "' directive.", line_num);

	value = remainder.substr(0, semicolon_pos);
	trim_whitespace(value);
}

//const std::string& ConfigParser::get_host() const
//{
//    if (!listen_configs.empty())
//        return listen_configs[0].host;

//    static const std::string empty_string;
//    return empty_string;
//}

//int ConfigParser::get_port() const
//{
//    // Return the port of the first listen config, or 0 if none exists
//    if (!listen_configs.empty())
//        return listen_configs[0].port;

//    return 0;
//}
