/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRC.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juramos <juramos@student.42madrid.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 10:38:54 by juramos           #+#    #+#             */
/*   Updated: 2024/11/26 10:39:45 by juramos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRC_HPP
#define IRC_HPP

#include <vector>
#include <string>
#include <map>
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#include <errno.h>
#include <cstdlib>
#include "Channel.hpp"
#include "Client.hpp"
#include "Server.hpp"
#include "Message.hpp"

namespace IRC {

    // Comandos básicos requeridos según el subject
    enum CommandType {
		// Validación conexión entre cliente y servidor
		CMD_CAP,
        // Autenticación y configuración de usuario
        CMD_PASS,    // Autenticación con password
        CMD_NICK,    // Establecer/cambiar nickname
        CMD_USER,    // Establecer username
        
        // Comandos de canal
        CMD_JOIN,    // Unirse a un canal
        CMD_PRIVMSG, // Enviar mensaje privado/canal
        
        // Comandos de operador
        CMD_KICK,    // Expulsar usuario del canal
        CMD_INVITE,  // Invitar usuario al canal
        CMD_TOPIC,   // Cambiar/ver topic del canal
        CMD_MODE,    // Cambiar modo del canal
        
        // Comandos adicionales necesarios
        // CMD_PING,    // Mantener conexión viva
        // CMD_PONG,    // Respuesta a PING
        CMD_QUIT     // Desconexión del servidor
    };

    // Modos de canal requeridos según el subject
    // enum ChannelMode {
    //     MODE_NONE = 0,
    //     MODE_I = 1 << 0,    // i - Canal solo por invitación
    //     MODE_T = 1 << 1,    // t - Solo ops pueden cambiar topic
    //     MODE_K = 1 << 2,    // k - Canal tiene password
    //     MODE_O = 1 << 3,    // o - Privilegios de operador
    //     MODE_L = 1 << 4     // l - Límite de usuarios
    // };

    // // Estados posibles de un usuario
    // enum UserState {
    //     USER_CONNECTED,     // Conectado pero no autenticado
    //     USER_AUTHENTICATED, // Password correcto
    //     USER_REGISTERED,    // Nick y usuario establecidos
    //     USER_OPERATOR      // Es operador
    // };

    // // Estados posibles de un canal
    // enum ChannelState {
    //     CHANNEL_ACTIVE,    // Canal activo
    //     CHANNEL_INVITE,    // Canal solo por invitación
    //     CHANNEL_PRIVATE,   // Canal privado (con password)
    //     CHANNEL_LIMITED    // Canal con límite de usuarios
    // };
}

#endif