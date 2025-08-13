/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EpollState.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkonarze <kkonarze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 19:09:40 by kkonarze          #+#    #+#             */
/*   Updated: 2025/08/13 19:10:59 by kkonarze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef EPOLLSTATE_HPP
#define EPOLLSTATE_HPP

#include "Webserv.hpp"

struct EpollState
{
	int						epoll_fd;
	int						num_of_fds;
	struct epoll_event		info;
	struct epoll_event		events[10];
};

#endif