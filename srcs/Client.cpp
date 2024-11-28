/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juramos <juramos@student.42madrid.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 10:07:13 by juramos           #+#    #+#             */
/*   Updated: 2024/11/28 13:25:28 by juramos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRC.hpp"

Client::Client(int socket): _socket(socket), _nickname(""),
	_username(""), _buffer(""), _authenticated(false) {}

Client::Client(): _socket(-1), _nickname(""),
	_username(""), _buffer(""), _authenticated(false) {}

Client::~Client() {}

int	Client::getSocket() const { return _socket; }

std::string const Client::getNickname() const { return _nickname; }

std::string const Client::getUsername() const { return _username; }

bool Client::isAuthenticated() const { return _authenticated; }

void        Client::setNickname(const std::string& nickname) {
	_nickname = nickname;
}

void        Client::setUsername(const std::string& username) {
	_username = username;
}

void        Client::setAuthenticated(bool status) {
	_authenticated = status;
}

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