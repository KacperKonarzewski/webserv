/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkonarze <kkonarze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 16:14:29 by kkonarze          #+#    #+#             */
/*   Updated: 2025/08/05 21:13:25 by kkonarze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

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

Response::Response()
{
}

Response::~Response()
{
}