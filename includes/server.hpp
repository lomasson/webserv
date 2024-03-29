/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomasson <lomasson@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 15:20:31 by jrasser           #+#    #+#             */
/*   Updated: 2023/04/26 16:24:35 by lomasson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#define REQ_MAX_SIZE 8192
#define MAX_REQUESTS 1024

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/event.h>
#include <netdb.h>

#include <algorithm>
#include <iostream>
#include <string>
#include <cstring>
#include <ctime>
#include <sstream>
#include <vector>
#include <exception>
#include <map>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <dirent.h>

enum Status
{
	WAITING_FOR_REQUEST,
	HEADER_RECEIVED,
	REQUEST_CHUNKED,
	REQUEST_RECEIVED,
	PURGE_REQUIRED,
	REQUEST_PARSED,
	CGI_PROCESS_LAUNCHED,
	BODY_GENERATED,
	HEADER_GENERATED,
	HEADER_SENT,
	BODY_SENT,
	SOCKET_ERROR,
};

#include "Request.hpp"
#include "Config.hpp"
#include "Cgi.hpp"

#include "Settings.hpp"

#endif