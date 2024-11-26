/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juramos <juramos@student.42madrid.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 10:28:50 by juramos           #+#    #+#             */
/*   Updated: 2024/11/26 10:58:49 by juramos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "IRC.hpp"

class Server {
private:
    int _server_fd;
    int _port;
    std::string _password;
    std::map<const int, Client>		_clients;
	std::map<const std::string, Channel>	_channels;
    static const int BUFFER_SIZE = 1024;
    
    void setUpServerSocket();
public:
	Server(void);
    Server(int port, const std::string& password);
	// Server(Server &toCopy);
	// Server	&operator=(Server &other);
    ~Server();

	int	getPort() const;
	const std::string	&getPassword() const;
    void handleNewConnection(std::vector<struct pollfd> &pollfds);
    void handleClientMessage(struct pollfd& pfd);
    void start();
};

#endif