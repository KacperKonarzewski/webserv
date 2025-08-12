/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Signal.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkonarze <kkonarze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 12:48:31 by kkonarze          #+#    #+#             */
/*   Updated: 2025/08/12 21:00:57 by kkonarze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

#include "Signal.hpp"

SignalState g_signal_state;

void handle_sigint(int signum) {
	(void)signum;
	g_signal_state.sigint = 1;
}

void handle_sigterm(int signum) {
	(void)signum;
	g_signal_state.sigterm = 1;
}

void setup_signals() {
	g_signal_state.sigint = 0;
	g_signal_state.sigterm = 0;

	signal(SIGINT, handle_sigint);
	signal(SIGTERM, handle_sigterm);
}
