/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmunoz-g <cmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 12:07:15 by cmunoz-g          #+#    #+#             */
/*   Updated: 2024/11/28 16:18:50 by cmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRC.hpp"

Message::Message(const Client &client) {
	_senderSocket = client.getSocket();
	parse(client.getBuffer());
	setReceiver();
}

Message::~Message(void) {}

const std::string& Message::getPrefix() const { return _command._prefix; }
const std::string& Message::getCommand() const { return _command._command; }
const std::vector<std::string>& Message::getParams() const { return _command._params; }
IRC::CommandType Message::getParsedCommand() const { return _parsedCommand; }

void	Message::parse(const std::string& buffer) {
    std::string msg = buffer;
    
    // Eliminar \r\n del final si existe
    size_t end = msg.find("\r\n");
    if (end != std::string::npos)
        msg = msg.substr(0, end);

    // Extraer prefix si existe
    if (!msg.empty() && msg[0] == ':') {
        size_t space = msg.find(' ');
        if (space == std::string::npos) {
            _command._prefix = "";
            return;
        }
        _command._prefix = msg.substr(1, space - 1);
        msg = msg.substr(space + 1);
    }

    // Extraer comando
    size_t space = msg.find(' ');
    if (space == std::string::npos) {
        _command._command = msg;
        return; // No hay parámetros
    }
    
    _command._command = msg.substr(0, space);
    msg = msg.substr(space + 1);

    // Extraer parámetros
    _command._params.clear();
    size_t start = 0;
    
    // Procesar todos los parámetros
    while (start < msg.length()) {
        // Si encontramos un ':', tomar todo lo que queda como último parámetro
        if (msg[start] == ':') {
            _command._params.push_back(msg.substr(start + 1));
            break;
        }

        // Encontrar el siguiente espacio
        space = msg.find(' ', start);
        
        // Si no hay más espacios, este es el último parámetro
        if (space == std::string::npos) {
            _command._params.push_back(msg.substr(start));
            break;
        }

        // Añadir el parámetro encontrado
        _command._params.push_back(msg.substr(start, space - start));
        start = space + 1;

        // Saltar espacios adicionales
        while (start < msg.length() && msg[start] == ' ')
            start++;
    }
}

void	Message::setReceiver(void)
{
	for (std::vector<std::string>::iterator it = _command._params.begin(); it < _command._params.end(); it++)
	{
		if (!(*it).empty() && (*it).at(0) == '#')
			_receiverChannel = (*it);
	}
}
