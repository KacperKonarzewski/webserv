/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkonarze <kkonarze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 16:14:29 by kkonarze          #+#    #+#             */
/*   Updated: 2025/08/06 00:22:07 by kkonarze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"
#include "Request.hpp"

const Location *Response::find_location(std::string uri, const Config &conf)
{
	for (size_t i = 0; i < conf.locations.size(); i++)
		if (conf.locations[i].get_location_path() == uri)
			return &(conf.locations[i]);
	
	for (size_t i = 0; i < conf.locations.size(); i++)
        if (conf.locations[i].get_location_path() == "/")
            return &(conf.locations[i]);
			
	return (NULL);
}

void	Response::create_response(std::string& target)
{
	std::ifstream index(target.c_str());
    std::string html;
	std::stringstream buffer;
	std::ostringstream headers;
	
	buffer << index.rdbuf();
	html = buffer.str();
    headers << "HTTP/1.1 200 OK\r\n"
            << "Content-Type: text/html\r\n"
            << "Content-Length: " << html.size() << "\r\n"
            << "Connection: close\r\n\r\n"
            << html;

	index.close();
	res = headers.str();
}

void	Response::create_error(std::map<int, std::string> error, int code, std::string message)
{
	std::ifstream index(error.at(code).c_str());
    std::string html;
	std::stringstream buffer;
	std::ostringstream headers;
	
	buffer << index.rdbuf();
	html = buffer.str();
    headers << "HTTP/1.1 " << code << " " << message << "\r\n"
            << "Content-Type: text/html\r\n"
            << "Content-Length: " << html.size() << "\r\n"
            << "Connection: close\r\n\r\n"
            << html;

	index.close();
	res = headers.str();
}

std::string	Response::get_response()
{
	return (res);
}

Response::Response(Client *client, const Config &conf)
{
	std::string target;
	std::string file;
	
	Request *request = client->get_request();
	std::map<std::string, std::string> tokens = request->get_tokens();

	const Location *location = find_location(tokens["request_uri"], conf);

	std::string root = location->get_directive()["root"];

	file = (is_directory(tokens["request_uri"]))? location->get_directive()["index"] : tokens["request_uri"];
	trim_string(file, "/");
	trim_string(root, "/");
	target = root + "/" + file;

	if (access(target.c_str(), F_OK) == 0)
		create_response(target);
	else
		create_error(conf.error_pages, 404, "Not Found");
}

Response::Response()
{
}

Response::~Response()
{
}