/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juramos <juramos@student.42madrid.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:04:39 by juramos           #+#    #+#             */
/*   Updated: 2024/12/14 12:14:46 by juramos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRC.hpp"

Server::Server(void): _port(6667), _password("password") {
		setUpServerSocket();
}

Server::Server(int port, const std::string& password):
	_port(port), _password(password) {
		FD_ZERO(&_master_read);
		FD_ZERO(&_master_write);
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
	std::cout << "Cliente conectado con ID: " << client_fd << std::endl;
	
	Client* newClient = _clients[id - 1];
	if (newClient) {
		std::cout << "Cliente creado correctamente" << std::endl;
		std::cout << "Estado de autenticación: " << (newClient->isAuthenticated() ? "true" : "false") << std::endl;
	} else {
		std::cout << "Error: Cliente no creado" << std::endl;
	}

	FD_SET(client_fd, &_master_read);
	FD_SET(client_fd, &_master_write);
}

void Server::handleJoin(const Message& msg, Client* client) {
    
	std::cout << "handleJoin" << std::endl;
	if (!client->isAuthenticated()) {
		std::cout << "Error: cliente no autenticado" << std::endl;
        // Enviar error: no autenticado
        return;
    }

    const std::vector<std::string>& params = msg.getParams();
    if (params.empty()) {
		std::cout << "Error: parámetros insuficientes" << std::endl;
        // Enviar error: parámetros insuficientes
        return;
    }

    std::string channelName = params[0];
    if (channelName[0] != '#') {
		std::cout << "Error: nombre de canal inválido" << std::endl;
        // Enviar error: nombre de canal inválido
        return;
    }

    // Buscar o crear canal
    if (_channels.find(channelName) == _channels.end()) {
        _channels[channelName] = Channel(channelName, client);
		std::cout << "Canal creado" << std::endl;
    }

    Channel& channel = _channels[channelName];
    if (channel.addClient(client)) {
        client->joinChannel(&channel);
        channel.sendNames(client);
    }
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
		if (client_id < 0 || client_id >= FD_SETSIZE) {
			std::cout << "Error: client_id inválido: " << client_id << std::endl;
			return;
		}
		if (client_id > 0) {
			_clients[client_id]->appendToBuffer(buffer);
			if (_clients[client_id]->getBuffer().substr(_clients[client_id]->getBuffer().size() - 2) == "\r\n") {
				// TODO: Update to construct Message from Client, so we store: sender (client socket) 
				// and receiver (channel name)
				Message newMessage(_clients[client_id]);
				_clients[client_id]->clearBuffer();
				// Muy tocho, poner bonito.
				switch (newMessage.getCommandType())
				{
					case IRC::CMD_CAP:
						std::cout << "Recibido CAP" << std::endl;
						if (!newMessage.getParams().empty() && newMessage.getParams()[0] == "LS") {
							// Para irssi, necesitamos responder con CAP LS vacío y luego END
							std::string response = ":irc CAP * LS :\r\n";
							int socket = _clients[client_id]->getSocket();
							ssize_t sent = send(socket, response.c_str(), response.length(), 0);
							std::cout << "Enviado [" << response << "], bytes: " << sent << std::endl;
							
							// El END debe ser inmediato
							response = ":irc CAP * ACK\r\n";
							sent = send(socket, response.c_str(), response.length(), 0);
							std::cout << "Enviado [" << response << "], bytes: " << sent << std::endl;
						}
						else if (!newMessage.getParams().empty() && newMessage.getParams()[0] == "END") {
							// Cuando recibimos CAP END, confirmamos que la negociación está completa
							std::string response = ":irc CAP * ACK\r\n";
							int socket = _clients[client_id]->getSocket();
							send(socket, response.c_str(), response.length(), 0);
						}
						break;
					case IRC::CMD_PASS:
						std::cout << "Recibido PASS" << std::endl;
						if (!newMessage.getParams().empty()) {
							if (newMessage.getParams()[0] == _password) {
								_clients[client_id]->setAuthenticated(true);
								std::cout << "Password correcto, cliente autenticado" << std::endl;
							} else {
								std::string response = ":localhost 464 * :Password incorrect\r\n";
								send(_clients[client_id]->getSocket(), response.c_str(), response.length(), 0);
								std::cout << "Password incorrecto" << std::endl;
							}
						}
						break;
					case IRC::CMD_NICK:
						std::cout << "Recibido NICK" << std::endl;
						break;
					case IRC::CMD_USER:
						std::cout << "Recibido USER" << std::endl;
						break;
					case IRC::CMD_PRIVMSG:
						break;
					case IRC::CMD_JOIN:
						handleJoin(newMessage, _clients[client_id]);
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
				std::cout << "command :" << newMessage.getCommand() << std::endl; 
				std::cout << "prefix :" << newMessage.getPrefix() << std::endl; 
				const std::vector<std::string> params = newMessage.getParams();
				int i = 1;
				for (std::vector<std::string>::const_iterator it = params.begin(); it != params.end(); ++it) {
					std::cout << "param" << i << ":" << *it << std::endl; 
					i++;
				}
				
			}
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
					// Nueva conexión en el socket servidor
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
