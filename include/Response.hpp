/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkonarze <kkonarze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 16:01:25 by kkonarze          #+#    #+#             */
/*   Updated: 2025/08/12 20:58:30 by kkonarze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
# ifndef RESPONSE_HPP
# define RESPONSE_HPP

#include "Client.hpp"
#include "Webserv.hpp"

class Response
{
private:
	std::string res;
	std::map<std::string, std::string> types;

	const Location *find_location(std::string uri, const Config &conf);
	void			init_mime_types();
	std::string		get_mime_type(const std::string &path);
public:
	void	create_response(std::string& target);
	void	create_error(std::map<int, std::string> error, int code, std::string message);

	std::string	get_response();

	Response(Client *client, const Config &conf);
	Response();
	~Response();
};


#endif