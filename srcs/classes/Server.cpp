/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmunoz-g <cmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:04:39 by juramos           #+#    #+#             */
/*   Updated: 2024/12/14 19:12:50 by cmunoz-g         ###   ########.fr       */
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

unsigned int Server::fetchClientIdFromPid(int fd) {
	for (int i = 1; _clients[i]; i++)
		if (_clients[i]->getSocket() == fd)
			return (_clients[i]->getId());
	return (-1);
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
	static int id = 1;
	_clients[id] = new Client(client_fd, id); // Rarete, revisar y ver que esta pasando con la memoria
	// el mismo objeto, con la misma dir. de memoria tanto en server como en channel
	
	//_clients.insert(std::pair<int, Client>(id, Client(client_fd, id)));
	id++;
		
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
		unsigned int client_id = fetchClientIdFromPid(pfd.fd);
		if (client_id > 0) {
			_clients[client_id]->appendToBuffer(buffer);
			if (_clients[client_id]->getBuffer().substr(_clients[client_id]->getBuffer().size() - 2) == "\r\n") {
				// TODO: Update to construct Message from Client, so we store: sender (client socket) 
				// and receiver (channel name)
				Message newMessage(_clients[client_id]);
				_clients[client_id]->clearBuffer();
				// Muy tocho, poner bonito.
				switch (newMessage.getCommandType()) // De momento la gestion de comandos se va a hacer con metodos dentro de Server. Si la clase Server se vuelve
				// demasiado compleja, se puede modularizar esta gestion.
				{
					case IRC::CMD_CAP:
						handleCapCommand(newMessage);
						break;
					case IRC::CMD_NICK:
						std::cout << "sending nick";
						break;
					case IRC::CMD_PASS:
						std::cout << "sending pass";
						break;
					case IRC::CMD_USER:
						std::cout << "sending user";
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

				// Algo raro, cuando comento/descomento esto de abajo cambian cosas, i.e. el servidor recibe o no recibe los comandos. weird

				// std::cout << newMessage.getCommandType() << std::endl;
				// std::cout << "command :" << newMessage.getCommand() << std::endl; 
				// std::cout << "prefix :" << newMessage.getPrefix() << std::endl; 
				// const std::vector<std::string> params = newMessage.getParams();
				// int i = 1;
				// for (std::vector<std::string>::const_iterator it = params.begin(); it != params.end(); ++it) {
				// 	std::cout << "param" << i << ":" << *it << std::endl; 
				// 	i++;
				// }
			}
		}
}

void Server::handleCapCommand(Message &message) {
	// :<server_name> CAP <client_id> LS :
	if (message.getParams()[0] == "LS") { // && message.getParams()[1] == "302" hace falta???
		std::string response = ":" + SERVER_NAME + " CAP * LS :multi-prefix\r\n";
		_clients[message.getSenderId()]->receiveMessage(response);
	}
	else if (message.getParams()[0] == "REQ") {
		std::string response = ":" + SERVER_NAME + " CAP * ACK :" + message.getParams()[1] + "\r\n"; // Ahora mismo mandamos lo que pide Irssi, mirar si hay que implementarlos requerimientos diferente o que
		_clients[message.getSenderId()]->receiveMessage(response);
	}
	else if (message.getParams()[0] == "END") {
		std::cout << "Capability negotiation ended for client " << _clients[message.getSenderId()] << std::endl; // esta devolviendo el id en hex?
		_clients[message.getSenderId()]->setCapNegotiationStatus(true);
	}
	else {
		// checkear
	}

}

void Server::deleteClients() {
	std::map<unsigned int, Client*>::iterator it = _clients.begin();

	while (it != _clients.end()) {
		if (it->second->getSocket() == -1) {
			it->second->cleanup();
			std::map<unsigned int, Client*>::iterator toErase = it;
			++it;
			_clients.erase(toErase);
		}
		else {
			++it;
		}
	} 
}

void Server::start() {
	std::vector<struct pollfd> pollfds;
	Message::initCommandMap();
	
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
					// Nueva conexión en el socket servidor // Nota; Seguro???? 
					handleNewConnection(pollfds);
				} else {
					// Mensaje de un cliente existente
					handleClientMessage(pollfds[i]);
				}
			}
		}
		deleteClients();
	}
}
