/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juramos <juramos@student.42madrid.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 12:01:43 by cmunoz-g          #+#    #+#             */
/*   Updated: 2024/11/26 10:36:07 by juramos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGE_HPP
#define MESSAGE_HPP



#include "IRC.hpp"

class Message {
private:
    std::string					_prefix;    // Opcional: comienza con ':'
    std::string					_command;   // Comando IRC
    std::string 				_params;    // Parámetros del comando
	IRC::CommandType			_parsedCommand;
	
	void parse(const std::string& buffer);
    
public:
    Message(void);
	Message(const std::string &buffer);
	~Message(void);
	// Message(const Message &toCopy);
	// Message &operator=(const &other);
    
    // Getters
    const std::string&		getPrefix() const;
    const std::string&		getCommand() const;
    const std::string&		getParams() const;
    const IRC::CommandType	getParsedCommand() const;

	void					parseCommand();
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