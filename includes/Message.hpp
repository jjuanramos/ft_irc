/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmunoz-g <cmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 12:01:43 by cmunoz-g          #+#    #+#             */
/*   Updated: 2024/11/28 16:06:18 by cmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include "IRC.hpp"


struct CommandData {
	std::string				_prefix;
	std::string				_command;
	std::vector<std::string>	_params;
};

class Message {
private:
    CommandData			_command;
	IRC::CommandType	_parsedCommand;
	int					_senderSocket;
	std::string			_receiverChannel;
	
    Message(void);
	Message(const Message &toCopy);
	Message &operator=(const Message &other);
	void parse(const std::string& buffer);
    
public:
	Message(const Client &client);
	~Message(void);
    
    // Getters
    const std::string&		getPrefix() const;
    const std::string&		getCommand() const;
    const std::vector<std::string>& getParams() const;
    IRC::CommandType	getParsedCommand() const;

	void					setReceiver();
};

#endif

// class IRCParser {
// public:
//     static void handleMessage(Client& client, const std::string& raw) {
//         Message msg;
//         if (!msg.parse(raw))
//             return;

//         const std::string& cmd = msg.getCommand();
        
//         if (cmd == "CAP")
//             handleCap(client, msg);
//         else if (cmd == "NICK")
//             handleNick(client, msg);
//         else if (cmd == "USER")
//             handleUser(client, msg);
//         else if (cmd == "JOIN")
//             handleJoin(client, msg);
//         else if (cmd == "PRIVMSG")
//             handlePrivmsg(client, msg);
//         // Añadir más comandos según sea necesario
//     }

// private:
//     static void handleCap(Client& client, const Message& msg) {
//         if (msg.getParams().find("LS") != std::string::npos) {
//             // Respuesta básica a CAP LS
//             std::string response = ":server CAP * LS :\r\n";
//             client.send(response);
//             response = ":server CAP * END\r\n";
//             client.send(response);
//         }
//     }
    
//     static void handleNick(Client& client, const Message& msg) {
//         std::string nickname = msg.getParams();
//         if (nickname.empty())
//             return;
//         // Verificar si el nickname está disponible
//         if (isNicknameAvailable(nickname)) {
//             client.setNickname(nickname);
//             tryAuthenticate(client);
//         }
//     }

//     static void handleUser(Client& client, const Message& msg) {
//         std::istringstream iss(msg.getParams());
//         std::string username, hostname, servername, realname;
//         iss >> username >> hostname >> servername;
        
//         // El realname puede contener espacios y comienza con ':'
//         std::getline(iss, realname);
//         if (!realname.empty() && realname[0] == ':')
//             realname = realname.substr(1);

//         client.setUsername(username);
//         tryAuthenticate(client);
//     }

//     static void tryAuthenticate(Client& client) {
//         // Si tenemos tanto nick como username, el cliente está autenticado
//         if (!client.getNickname().empty() && !client.getUsername().empty()) {
//             // Enviar mensaje de bienvenida
//             std::string welcome = ":server 001 " + client.getNickname() + 
//                                 " :Welcome to the Internet Relay Network " +
//                                 client.getNickname() + "!" + 
//                                 client.getUsername() + "@" + 
//                                 client.getHostname() + "\r\n";
//             client.send(welcome);
//         }
//     }
// };