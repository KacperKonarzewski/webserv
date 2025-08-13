/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkonarze <kkonarze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 16:14:29 by kkonarze          #+#    #+#             */
/*   Updated: 2025/08/13 21:55:10 by kkonarze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

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

void	Response::init_mime_types()
{
	types[".html"] = "text/html";
	types[".htm"] = "text/html";
	types[".css"] = "text/css";
	types[".js"] = "application/javascript";
	types[".json"] = "application/json";
	types[".png"] = "image/png";
	types[".jpg"] = "image/jpeg";
	types[".jpeg"] = "image/jpeg";
	types[".svg"] = "image/svg+xml";
	types[".ico"] = "image/x-icon";
	types[".pdf"] = "application/pdf";
	types[".txt"] = "text/plain";
}

std::string Response::get_mime_type(const std::string &path)
{
	std::string ext = get_file_extension(path);
	
	std::map<std::string, std::string>::const_iterator it = types.find(ext);
	if (it != types.end())
    	return it->second;
	return "application/octet-stream";
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
            << "Content-Type: " << get_mime_type(target) << "\r\n"
            << "Content-Length: " << html.size() << "\r\n"
            << "Connection: keep-alive\r\n\r\n"
            << html;

	index.close();
	res = headers.str();
}

void	Response::create_error(std::map<int, std::string> error, int code, std::string message)
{
    std::string html;
	std::stringstream buffer;
	std::ostringstream headers;
	
	if (error.count(code))
	{
		std::ifstream index(error.at(code).c_str());
		buffer << index.rdbuf();
		index.close();
	}
	else
	{
		buffer << "<html><head><title>" << code << " " << message << "</title></head>"
			   << "<body><h1>" << code << " " << message << "</h1>"
			   << "<p>" << message << "</p></body></html>";
	}
	html = buffer.str();
	headers << "HTTP/1.1 " << code << " " << message << "\r\n"
            << "Content-Type: text/html\r\n"
            << "Content-Length: " << html.size() << "\r\n"
            << "Connection: close\r\n\r\n"
            << html;
	
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
	//std::string	method = tokens["method"];
	//std::string allowed = location->get_directive()["allow_methods"];
	//if (allowed.empty() && allowed.find(method) == std::string::npos)
	//{
	//	create_error(conf.error_pages, 405, "Method Not Allowed");
	//	return;
	//}
	std::string root = location->get_directive()["root"];

	init_mime_types();

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