/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkonarze <kkonarze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 16:01:25 by kkonarze          #+#    #+#             */
/*   Updated: 2025/08/05 21:12:20 by kkonarze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
# ifndef RESPONSE_HPP
# define RESPONSE_HPP

#include <string>
#include <map>
#include <iostream>
#include <sstream>
#include <cstring>
#include <unistd.h>
#include <cstdio>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <errno.h>
#include <fstream>

class Response
{
private:
	std::string res;
public:
	void	create_response(std::string& target);
	void	create_error(std::map<int, std::string> error, int code, std::string message);

	std::string	get_response();
	Response();
	~Response();
};


#endif