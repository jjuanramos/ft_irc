/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCTypes.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juramos <juramos@student.42madrid.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 13:23:33 by juramos           #+#    #+#             */
/*   Updated: 2024/11/28 13:23:40 by juramos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRC_TYPES_HPP
#define IRC_TYPES_HPP

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
    enum ChannelMode {
        MODE_NONE,
        MODE_I,    // i - Canal solo por invitación
        MODE_T,    // t - Solo ops pueden cambiar topic
        MODE_K,    // k - Canal tiene password
        MODE_O,    // o - Privilegios de operador
        MODE_L     // l - Límite de usuarios
    };

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