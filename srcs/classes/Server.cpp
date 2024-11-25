/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juramos <juramos@student.42madrid.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:04:39 by juramos           #+#    #+#             */
/*   Updated: 2024/11/25 11:37:36 by juramos          ###   ########.fr       */
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

	// Añadir socket servidor a poll
	struct pollfd server_pollfd = {_server_fd, POLLIN, 0};
	_pollfds.push_back(server_pollfd);
}

void	Server::handleNewConnection() {
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
	_pollfds.push_back(client_pollfd);

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
        std::cout << "Mensaje recibido: " << buffer << std::endl;
        // TODO: el procesamiento del mensaje
}

void Server::start() {
	while (true) {
		// Poll espera eventos en los sockets
		int ret = poll(_pollfds.data(), _pollfds.size(), -1);
		if (ret < 0)
			throw std::runtime_error("Error en poll");

		// Revisar todos los file descriptors
		for (size_t i = 0; i < _pollfds.size(); i++) {
			if (_pollfds[i].revents & POLLIN) {
				if (_pollfds[i].fd == _server_fd) {
					// Nueva conexión en el socket servidor
					handleNewConnection();
				} else {
					// Mensaje de un cliente existente
					handleClientMessage(_pollfds[i]);
				}
			}
		}

		// TODO: Eliminar clientes desconectados
	}
}