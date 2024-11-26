/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juramos <juramos@student.42madrid.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:04:39 by juramos           #+#    #+#             */
/*   Updated: 2024/11/26 10:42:48 by juramos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRC.hpp"

Server::Server(void): _port(6667), _password("password") {
		setUpServerSocket();
}

Server::Server(int port, const std::string& password):
	_port(port), _password(password) {
		setUpServerSocket();
}

// Server::Server(Server &toCopy): _port(toCopy.getPort()), _password(toCopy.getPassword()) {}

// Server	&Server::operator=(Server &other) {
// 	if (this != &other)
// 	{
// 		// TODO
// 		return *this;
// 	}
// 	return *this;
// }

int	Server::getPort() const { return _port; }

const std::string	&Server::getPassword() const { return _password; }

Server::~Server() {
	close(_server_fd);
}

void	Server::setUpServerSocket() {
	// Crear socket
	_server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (_server_fd < 0)
		throw std::runtime_error("Error creando socket");

	// Configurar socket como no bloqueante
	fcntl(_server_fd, F_SETFL, O_NONBLOCK);

	// Permitir reutilización del puerto
	int opt = 1;
	setsockopt(_server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	// Configurar dirección del servidor
	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(_port);

	// Vincular socket
	if (bind(_server_fd, (struct sockaddr*)&address, sizeof(address)) < 0)
		throw std::runtime_error("Error en bind");

	// Escuchar conexiones
	if (listen(_server_fd, 10) < 0)
		throw std::runtime_error("Error en listen");
}

void	Server::handleNewConnection(std::vector<struct pollfd> &pollfds) {
	struct sockaddr_in client_addr;
	socklen_t client_len = sizeof(client_addr);
	
	int client_fd = accept(_server_fd, (struct sockaddr*)&client_addr, &client_len);
	if (client_fd < 0) {
		if (errno != EWOULDBLOCK)
			std::cerr << "Error aceptando conexión" << std::endl;
		return;
	}

	// Configurar nuevo socket cliente como no bloqueante
	fcntl(client_fd, F_SETFL, O_NONBLOCK);

	// Añadir nuevo cliente a poll
	struct pollfd client_pollfd = {client_fd, POLLIN, 0};
	pollfds.push_back(client_pollfd);

	// Añadir nuevo cliente al map
	Client newClient(client_fd);
	_clients.insert(std::make_pair(client_fd, newClient));

	std::cout << "Nueva conexión aceptada" << std::endl;
}

void Server::handleClientMessage(struct pollfd& pfd) {
        char buffer[BUFFER_SIZE];
        ssize_t bytes_read = recv(pfd.fd, buffer, BUFFER_SIZE - 1, 0);

        if (bytes_read <= 0) {
            if (bytes_read == 0 || errno != EWOULDBLOCK) {
                // Cliente desconectado o error
                close(pfd.fd);
                pfd.fd = -1; // Marcar para eliminar
            }
            return;
        }

        buffer[bytes_read] = '\0';
		_clients[pfd.fd].appendToBuffer(buffer);
    	if (_clients[pfd.fd].getBuffer().substr(_clients[pfd.fd].getBuffer().size() - 2) == "\r\n") {
			Message newMessage(_clients[pfd.fd].getBuffer());
			_clients[pfd.fd].clearBuffer();

			// Muy tocho, poner bonito.
			switch (newMessage.getParsedCommand())
			{
				case IRC::CMD_CAP:
					break;
				case IRC::CMD_NICK:
					break;
				case IRC::CMD_PASS:
					break;
				case IRC::CMD_USER:
					break;
				case IRC::CMD_PRIVMSG:
					break;
				case IRC::CMD_JOIN:
					break;
				case IRC::CMD_INVITE:
					break;
				case IRC::CMD_TOPIC:
					break;
				case IRC::CMD_MODE:
					break;
				case IRC::CMD_KICK:
					break;
				case IRC::CMD_QUIT:
					break;
				default:
					break;
				
			}
			// if (newMessage.process() == -1) {
			// 	// gestionar
			// }
			std::cout << "command :" << newMessage.getCommand() << std::endl; 
			std::cout << "prefix :" << newMessage.getPrefix() << std::endl; 
			std::cout << "params :" << newMessage.getParams() << std::endl; 
			
		}
}

void Server::start() {
	std::vector<struct pollfd> pollfds;
	
	struct pollfd server_pollfd = {_server_fd, POLLIN, 0};
	pollfds.push_back(server_pollfd);

	while (true) {
		// Poll espera eventos en los sockets
		int ret = poll(pollfds.data(), pollfds.size(), -1);
		if (ret < 0)
			throw std::runtime_error("Error en poll");

		// Revisar todos los file descriptors
		for (size_t i = 0; i < pollfds.size(); i++) {
			if (pollfds[i].revents & POLLIN) {
				if (pollfds[i].fd == _server_fd) {
					// Nueva conexión en el socket servidor
					handleNewConnection(pollfds);
				} else {
					// Mensaje de un cliente existente
					handleClientMessage(pollfds[i]);
				}
			}
		}

		// TODO: Eliminar clientes desconectados
	}
}