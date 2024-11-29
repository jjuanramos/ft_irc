/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmunoz-g <cmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 10:44:00 by juramos           #+#    #+#             */
/*   Updated: 2024/11/29 09:42:15 by cmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

class Client;

#include "IRC.hpp"

class Channel {
private:
    std::string _name;                         // Nombre del canal
    std::string _topic;                        // Topic del canal
    std::string _password;                     // Password (para modo k)
    std::vector<unsigned int> _modes;          // Modos activos (usando ChannelMode)
    std::map<unsigned int, Client*> _clients;      // Clientes en el canal
    std::map<unsigned int, Client*> _operators;    // Operadores (true) y usuarios normales (false)
    size_t _userLimit;                         // Límite de usuarios (para modo l)
    Channel(Channel &toCopy);
    Channel &operator=(Channel &other);
    Channel(void);

public:
    // Constructor y destructor
    Channel(const std::string& name, Client* creator);
    ~Channel();

    // Getters básicos
    const std::string& getName() const;
    const std::string& getTopic() const;
    size_t getUserCount() const;
    size_t getUserLimit() const;
    bool isOperator(Client* client) const;

    // Manejo de modos (requeridos por el subject)
    void setMode(IRC::ChannelMode mode, bool enabled = true);
    bool hasMode(IRC::ChannelMode mode) const;
    void setPassword(const std::string& pass);
    void setUserLimit(size_t limit);
    bool checkPassword(const std::string& pass) const;

    // Manejo de topic (requerido por el subject)
    bool setTopic(Client* client, const std::string& newTopic);
    
    // Manejo de usuarios
    bool addClient(Client* client, const std::string& password = "");
    bool removeClient(Client* client);
    bool hasClient(Client* client) const;
    
    // Manejo de operadores
    bool addOperator(Client* client);
    bool removeOperator(Client* client);
    
    // Comandos de operador (requeridos por el subject)
    bool kickClient(Client* operator_client, Client* target, const std::string& reason = "");
    bool inviteClient(Client* operator_client, Client* target);

    // Mensajes
    void broadcastMessage(const std::string& message, Client* exclude = NULL);
    void sendNames(Client* client) const;

private:
    // Métodos auxiliares
    bool canModifyTopic(Client* client) const;
    bool isInviteOnly() const;
    bool isClientBanned(Client* client) const;
    bool notifyModeChange(Client* changer, char mode, bool enabled, const std::string& param = "");

    // Prohibir copia
    Channel(const Channel& other);
    Channel& operator=(const Channel& other);
};

#endif