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
    std::vector<std::string> _channels;   // Canales a los que está unido
    std::vector<std::string> _op_channels; // Canales donde es operador

public:
    Client(int socket);
    Client(void);
    // Client(&toCopy);
    // Client &operator=(&other);
    ~Client();

    // Getters básicos
    int         getSocket() const;
    std::string getNickname() const;
    std::string getUsername() const;
    bool        isAuthenticated() const;
    
    // Setters básicos
    void        setNickname(const std::string& nickname);
    void        setUsername(const std::string& username);
    void        setAuthenticated(bool status);

    // Buffer para mensajes parciales
    void        appendToBuffer(const std::string& data);
    std::string getBuffer() const;

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