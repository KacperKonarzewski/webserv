/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkonarze <kkonarze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 01:09:30 by kkonarze          #+#    #+#             */
/*   Updated: 2025/08/05 03:00:36 by kkonarze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Request.hpp"

void Request::parse_request()
{
	std::istringstream	stream(buffer);
    std::string			line;
	size_t 				line_num = 0;
	bool				body_flag = false;

	while (std::getline(stream, line))
	{
		line_num++;
		std::cout << line << std::endl;
		if (line == "\r" || line.empty())
		{
			body_flag = true;
			continue ;
		}
		process_line(line, line_num, body_flag);
	}
	if (tokens["method"] == "GET" && tokens["data"] != "")
		parse_body(tokens["data"]);
}

Request::Request()
{

}

std::map<std::string, std::string>	Request::get_tokens()
{
	return (tokens);
}

Request::Request(int client_fd)
{
	std::memset(buffer, 0, sizeof(buffer));
	read(client_fd, buffer, sizeof(buffer) - 1);
}

Request::~Request() 
{

}

void Request::process_line(std::string& line, int line_num, bool body_flag)
{
	if (line_num == 1)
		parse_requestline(line);
	else if (!body_flag)
		parse_header(line);
	else
		parse_body(line);
}

void Request::parse_requestline(std::string& line)
{
	std::vector<std::string>	request_line;
	size_t						first_space;
	std::string					file;
	std::string					data;

	request_line = ft_split(line, " ");
	tokens["method"] = request_line[0];
	if (tokens["method"] == "GET")
	{
		first_space = request_line[1].find_first_of("?");
		file = (first_space == std::string::npos)? request_line[1] : request_line[1].substr(0, first_space);
		data = (first_space == std::string::npos)? "" : request_line[1].substr(first_space);
		tokens["request_uri"] = file;
		tokens["data"] = data;
	}
	else
		tokens["request_uri"] = request_line[1];
	tokens["HTTP_version"] = request_line[2];
}

void Request::parse_header(std::string& line)
{
	std::vector<std::string> header_line;

	if (line.find(": ") == std::string::npos)
		return ;
	header_line = ft_split(line, ": ");
	tokens.insert(std::pair<std::string, std::string>(header_line[0], header_line[1]));
	if (header_line[0].empty())
		return ;
}

void Request::parse_body(std::string& line)
{
	std::vector<std::string> body_pairs;
	std::vector<std::string> pair;
	size_t i = 0;

	body_pairs = ft_split(line, "&");

	while (i < body_pairs.size())
	{
		pair = ft_split(body_pairs[i], "=");
		tokens[pair[0]] = pair[1];
		i++;
	}
}