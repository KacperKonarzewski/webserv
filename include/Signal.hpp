/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Signal.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkonarze <kkonarze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 12:47:56 by kkonarze          #+#    #+#             */
/*   Updated: 2025/08/12 21:05:29 by kkonarze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef SIGNAL_HANDLER_HPP
#define SIGNAL_HANDLER_HPP

#include "Webserv.hpp"

struct SignalState {
	sig_atomic_t sigint;
	sig_atomic_t sigterm;
};

extern SignalState g_signal_state;

void setup_signals();
void handle_sigint(int signum);
void handle_sigterm(int signum);

#endif
