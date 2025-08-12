/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkonarze <kkonarze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 12:48:05 by kkonarze          #+#    #+#             */
/*   Updated: 2025/08/12 20:59:14 by kkonarze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include <map>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <cstdio>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <cstring>
#include <iostream>
#include <signal.h>
#include <errno.h>
#include <cstddef>
#include <cstdlib>
#include <fcntl.h>

// Utils.cpp
void						error(const std::string& message);
void						trim_whitespace(std::string& s);
void						remove_comment(std::string& s);
int							string_to_int(const std::string& s);
int							make_socket_non_blocking(int fd);
std::vector<std::string>	ft_split(std::string str, std::string delimiter);
bool						is_directory(std::string& str);
void						trim_string(std::string &s, std::string charset);
std::string					get_file_extension(const std::string &path);

#endif