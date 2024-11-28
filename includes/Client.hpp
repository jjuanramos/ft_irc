#ifndef CLIENT_HPP
#define CLIENT_HPP

class Channel;

#include "IRC.hpp"

class Client {
private:
    int                 _socket;
    std::string         _nickname;
    std::string         _username;
    std::string         _buffer;
    bool                _authenticated;
    std::map<const std::string, Channel*> _channels;   // Canales a los que está unido
    std::map<const std::string, Channel*> _op_channels; // Canales donde es operador
    Client(&toCopy);
    Client &operator=(Client &other);

public:
    Client(int socket);
    Client(void);
    ~Client();

    void    cleanup();
    Client operator==(Client &other);

    // Getters básicos
    int					getSocket() const;
    std::string const	getNickname() const;
    std::string const	getUsername() const;
    bool                isAuthenticated() const;
	std::string const 	getBuffer() const;
    
    // Setters básicos
    void        setNickname(const std::string& nickname);
    void        setUsername(const std::string& username);
    void        setAuthenticated(bool status);

    // Buffer para mensajes parciales
    void        appendToBuffer(const std::string& data);
	void		clearBuffer(void);

    // Gestión de canales y operadores
    void        joinChannel(const Channel *channel);
    void        leaveChannel(const Channel *channel);
    bool        isInChannel(const Channel *channel) const;
    void        setOperatorStatus(const Channel *channel);
    void        removeOperatorStatus(const Channel *channel);
    bool        isOperator(const Channel *channel) const;

    // Comunicación básica
    //bool        sendMessage(const std::string& message);
    //bool        receiveMessage();
};

#endif