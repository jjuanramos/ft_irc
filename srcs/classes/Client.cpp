/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmunoz-g <cmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 10:07:13 by juramos           #+#    #+#             */
/*   Updated: 2024/11/28 16:18:26 by cmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRC.hpp"

Client::Client(int socket): _socket(socket), _nickname(""),
	_username(""), _buffer(""), _authenticated(false) {}

Client::Client(): _socket(-1), _nickname(""),
	_username(""), _buffer(""), _authenticated(false) {}

Client::~Client() {
	close(_socket);
}

//

int const	Client::getSocket() const { return _socket; }

std::string const Client::getNickname() const { return _nickname; }

std::string const Client::getUsername() const { return _username; }

//

bool const	Client::isAuthenticated() { return _authenticated; }

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

//

void	Client::appendToBuffer(const std::string& data)
{
	_buffer.append(data);
}

std::string const Client::getBuffer() const {
	return _buffer;
}

void	Client::clearBuffer(void) {
	_buffer.clear();
}

//

void	Client::joinChannel(const Channel *channel) {
	if (isInChannel(channel)) {
		std::cerr << "Client is already on the channel";
		return ;
	}
	_channels.insert(std::make_pair(channel.getName(), Channel));
	//channel.addClient();
}

void	Client::leaveChannel(const Channel *channel) {
	if (!isInChannel(channel)) {
		std::cerr << "Client is not on the channel" << std::endl;
		return ;
	}

	std::map<const std::string, Channel>::iterator it = _channels.begin();
	
	while (it->second.getName() != channel.getName())  // Ahora mismo, para comprobar que sea el canal que estamos buscando comparamos nombres, pero deberiamos hacer un overload de == en Channel
		++it;
		
	_channels.erase(it);
	//channel.removeClient(it);
	
	if (isOperator(channel))
		removeOperatorStatus(channel);
}

bool	Client::isInChannel(const Channel *channel) const {
	for (std::map<const std::string, Channel>::iterator it = _channels.begin(); it != _channels.end(); ++it) {
		if (it->second.getName() == channel.getName())
			return (true);
	}
	return (false);
}

void	Client::setOperatorStatus(const Channel *channel) {
	if (!isInChannel(channel)) {
		// pensar como gestionar errores, como sacar los mensajes etc.
		std::cerr << "Client is not on the channel" << std::endl;
		return ;
	}
	else if (isOperator(channel)) {
		std::cerr << "Client is already an operator" << std::endl;
		return ;
	}
	
	std::map<const std::string, Channel>::iterator it = _channels.begin();
	
	while (it->second.getName() != channel.getName()) 
		++it;

	//channel.addOperator(it->second);
	_op_channels.insert(std::make_pair(channel.getName(), Channel));
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

	std::map<const std::string, Channel>::iterator it = _op_channels.begin();
	while (it->second.getName() != channel.getName()) l
		++it;
	
	//channel.removeOperator(it->second);
	_op_channels.erase(it);
}

bool	Client::isOperator(const Channel *channel) {
	for (std::map<const std::string, Channel>::iterator it = _op_channels.begin(); it != _op_channels.end(); ++it) {
		if (it->second.getName() == channel.getName())
			return (true);
	}
	return (false);
}

//

void	Client::cleanup() {
	for (std::map<const std::string, Channel>::iterator it = _channels.begin(); it != _channels.end(); ++it) {
		it->second.removeClient(*this);
	}
	_channels.clear();

	for (std::map<const std::string, Channel>::iterator it = _op_channels.begin(); it != _op_channels.end(); ++it) {
		it->second.removeClient(*this);
	}
	_op_channels.clear();
}

bool	Client::operator==(Client &other) {
	return (_nickname == other.getNickname())
}