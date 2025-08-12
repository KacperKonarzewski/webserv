/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkonarze <kkonarze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 20:58:34 by kkonarze          #+#    #+#             */
/*   Updated: 2025/08/12 20:58:34 by kkonarze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Server.hpp"
#include "Webserv.hpp"

class Request
{
	private:
		char buffer[2048];
		std::map<std::string, std::string> tokens;
		std::map<std::string, std::string> body;

		void process_line(std::string& line, int line_num, bool body_flag);
		void parse_requestline(std::string& line);
		void parse_header(std::string& line);
		void parse_body(std::string& line);
	public:
		Request();
		Request(int client_fd);
		~Request();

		std::map<std::string, std::string>	get_tokens();
		void parse_request();
};