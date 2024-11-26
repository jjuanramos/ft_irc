#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "IRC.hpp"

class Client {
private:
    int                 _socket;
    std::string         _nickname;
    std::string         _username;
    std::string         _buffer;
    bool                _authenticated;
    std::map<const std::string, Channel> _channels;   // Canales a los que está unido
    std::map<const std::string, Channel> _op_channels; // Canales donde es operador
public:
    Client(int socket);
    Client(void);
    // Client(&toCopy);
    // Client &operator=(&other);
    ~Client();

    // Getters básicos
    int const	const	getSocket() const;
    std::string const	getNickname() const;
    std::string const	getUsername() const;
    bool        const	isAuthenticated() const;
	std::string const 	getBuffer() const;
    
    // Setters básicos
    void        setNickname(const std::string& nickname);
    void        setUsername(const std::string& username);
    void        setAuthenticated(bool status);

    // Buffer para mensajes parciales
    void        appendToBuffer(const std::string& data);
	void		clearBuffer(void);

    // Gestión de canales y operadores
    void        joinChannel(const std::string& channel);
    void        leaveChannel(const std::string& channel);
    bool        isInChannel(const std::string& channel) const;
    void        setOperatorStatus(const std::string& channel);
    void        removeOperatorStatus(const std::string& channel);
    bool        isOperator(const std::string& channel) const;

    // Comunicación básica
    bool        sendMessage(const std::string& message);
    bool        receiveMessage();
};

#endif