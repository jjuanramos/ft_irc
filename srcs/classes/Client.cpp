/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmunoz-g <cmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 10:07:13 by juramos           #+#    #+#             */
/*   Updated: 2024/12/14 18:39:29 by cmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRC.hpp"

Client::Client(int socket, unsigned int id): _socket(socket), _nickname(""),
	_username(""), _buffer(""), _authenticated(false), _id(id) {}

Client::Client(): _socket(-1), _nickname(""),
	_username(""), _buffer(""), _authenticated(false), _id(0) {} // 0 is set as default id

Client::~Client() {
	close(_socket);
}

int	Client::getSocket() const { return _socket; }

Client::Client(const Client &toCopy)
    : _socket(toCopy._socket),
      _nickname(toCopy._nickname),
      _username(toCopy._username),
      _buffer(toCopy._buffer),
      _authenticated(toCopy._authenticated),
      _id(toCopy._id), // Initialize const member
      _channels(toCopy._channels), // Copy map of channels
      _op_channels(toCopy._op_channels) // Copy map of op channels
{}

std::string const	Client::getNickname() const { return _nickname; }

std::string const	Client::getUsername() const { return _username; }

unsigned int	Client::getId() const { return _id; }

//
bool	Client::isAuthenticated() const { return _authenticated; }

bool    Client::isCapNegotiationDone() const { return _capNegotiation; }

//

void        Client::setNickname(const std::string& nickname) {
	_nickname = nickname;
}

void        Client::setUsername(const std::string& username) {
	_username = username;
}

void        Client::setAuthenticated(bool status) {
	_authenticated = status;
}

void		Client::setCapNegotiationStatus(bool status) {
	_capNegotiation = status;
}

//

void	Client::appendToBuffer(const std::string& data)
{
	_buffer.append(data);
}

std::string const	Client::getBuffer() const {
	return _buffer;
}

void	Client::clearBuffer(void) {
	_buffer.clear();
}

//

void	Client::joinChannel(Channel *channel) {
	if (isInChannel(channel)) {
		std::cerr << "Client is already on the channel";
		return ;
	}
	_channels.insert(std::make_pair(channel->getName(), channel));
	//channel.addClient();
}

void	Client::leaveChannel(const Channel *channel) {
	if (!isInChannel(channel)) {
		std::cerr << "Client is not on the channel" << std::endl;
		return ;
	}

	std::map<const std::string, Channel*>::iterator it = _channels.begin();
	
	while (it->second->getName() != channel->getName())  // Ahora mismo, para comprobar que sea el canal que estamos buscando comparamos nombres, pero deberiamos hacer un overload de == en Channel
		++it;
		
	_channels.erase(it);
	//channel.removeClient(it);
	
	if (isOperator(channel))
		removeOperatorStatus(channel);
}

bool	Client::isInChannel(const Channel *channel) const {
	for (std::map<const std::string, Channel*>::const_iterator it = _channels.begin(); it != _channels.end(); ++it) {
		if (it->second->getName() == channel->getName())
			return (true);
	}
	return (false);
}

void	Client::setOperatorStatus(Channel *channel) {
	if (!isInChannel(channel)) {
		// pensar como gestionar errores, como sacar los mensajes etc.
		std::cerr << "Client is not on the channel" << std::endl;
		return ;
	}
	else if (isOperator(channel)) {
		std::cerr << "Client is already an operator" << std::endl;
		return ;
	}
	
	std::map<const std::string, Channel*>::const_iterator it = _channels.begin();
	
	while (it->second->getName() != channel->getName()) 
		++it;

	//channel.addOperator(it->second);
	_op_channels.insert(std::make_pair(channel->getName(), channel));
}

void	Client::removeOperatorStatus(const Channel *channel) {
	if (!isInChannel(channel)) {
		std::cerr << "Client is not on the channel" << std::endl;
		return ;
	}
	else if (!isOperator(channel)) {
		std::cerr << "Client is not an operator" << std::endl;
		return ;
	}

	std::map<const std::string, Channel*>::iterator it = _op_channels.begin();
	while (it->second->getName() != channel->getName())
		++it;
	
	//channel.removeOperator(it->second);
	_op_channels.erase(it);
}

bool	Client::isOperator(const Channel *channel) const {
	for (std::map<const std::string, Channel*>::const_iterator it = _op_channels.begin(); it != _op_channels.end(); ++it) {
		if (it->second->getName() == channel->getName())
			return (true);
	}
	return (false);
}

//

bool	Client::sendMessage(const std::string& message) { // Basic implementation, review
	std::cout << _nickname << ": " << message << std::endl;
	return (true);
}

void	Client::receiveMessage(const std::string &message) {
	ssize_t bytesSent = ::send(_socket, message.c_str(), message.size(), 0);

	if (bytesSent < 0) {
		std::cerr << "Error sending message to client" << _id << ": " << strerror(errno) << std::endl;
	}
	else if (static_cast<size_t>(bytesSent) < message.size()) {
		std::cerr << "Partial send: Only" << bytesSent << " bytes sent to client" << _id << ": " << strerror(errno) << std::endl;
	}
	else {
		std::cout << "Message sent to client" << _id << ": " << message;
	}
}

//

void	Client::cleanup() {
	for (std::map<const std::string, Channel*>::const_iterator it = _channels.begin(); it != _channels.end(); ++it) {
		it->second->removeClient(this);
	}
	_channels.clear();

	for (std::map<const std::string, Channel*>::const_iterator it = _op_channels.begin(); it != _op_channels.end(); ++it) {
		it->second->removeClient(this);
	}
	_op_channels.clear();
}

bool	Client::operator==(Client &other) {
	return (_id == other.getId());
}
