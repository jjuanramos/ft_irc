/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juramos <juramos@student.42madrid.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 10:28:50 by juramos           #+#    #+#             */
/*   Updated: 2024/11/28 13:08:52 by juramos          ###   ########.fr       */
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
    std::map<int, Client>		_clients;
	std::map<const std::string, Channel>	_channels;
    static int BUFFER_SIZE = 1024;
    
    void setUpServerSocket();
	Server(Server &toCopy);
	Server	&operator=(Server &other);
public:
	Server(void);
    Server(int port, const std::string& password);
    ~Server();

	int	getPort() const;
	const std::string	&getPassword() const;
    void handleNewConnection(std::vector<struct pollfd> &pollfds);
    void handleClientMessage(struct pollfd& pfd);
    void start();
};

#endif