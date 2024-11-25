/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmunoz-g <cmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 12:07:15 by cmunoz-g          #+#    #+#             */
/*   Updated: 2024/11/25 12:19:12 by cmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRC.hpp"

Message::Message(void): _prefix(""), _command(""), _params("") {}

Message::Message(const std::string &buffer) {
	parse(buffer);
}

Message::~Message(void) {}

const std::string& Message::getPrefix() const { return _prefix; }
const std::string& Message::getCommand() const { return _command; }
const std::string& Message::getParams() const { return _params; }

void Message::parse(const std::string& buffer) {
        std::string msg = buffer;
        
        // Eliminar \r\n del final si existe
        size_t end = msg.find("\r\n");
        if (end != std::string::npos)
            msg = msg.substr(0, end);

        // Extraer prefix si existe
        if (!msg.empty() && msg[0] == ':') {
            size_t space = msg.find(' ');
            if (space == std::string::npos) {
				_prefix = "";
                return;
			}
			else {
            	_prefix = msg.substr(1, space - 1);
            	msg = msg.substr(space + 1);
			}
        }

        // Extraer comando
        size_t space = msg.find(' ');
        if (space == std::string::npos) {
            _command = msg;
			_params = "";
        }
		else {
        	_command = msg.substr(0, space);
        	_params = msg.substr(space + 1);	
		}
    }