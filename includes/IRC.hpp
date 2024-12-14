/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRC.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmunoz-g <cmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 10:38:54 by juramos           #+#    #+#             */
/*   Updated: 2024/12/14 18:45:28 by cmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRC_HPP
#define IRC_HPP

#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#include <errno.h>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include "Channel.hpp"
#include "Client.hpp"
#include "Server.hpp"
#include "Message.hpp"

class Channel;
class Client;
class Server;
class Message;

const static int BUFFER_SIZE = 1024;
static std::string SERVER_NAME = "irc.localhost";

#endif