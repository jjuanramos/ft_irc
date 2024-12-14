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
    bool                _capNegotiation; // ha mandado CAP END
    bool                _authenticated; // ha seteado NICK y PASS
    unsigned int        _id;
    std::map<const std::string, Channel*> _channels;   // Canales a los que está unido
    std::map<const std::string, Channel*> _op_channels; // Canales donde es operador
    Client(const Client& other);
    Client& operator=(const Client& other);
    Client(void);

public:
    Client(int socket, unsigned int id);
    ~Client();

    void    cleanup();
    //Client &operator=(Client &other);
    bool operator==(Client &other);

    // Getters básicos
    int					getSocket() const;
    std::string const   getNickname() const;
    std::string const   getUsername() const;
    bool                isAuthenticated() const;
    bool                isCapNegotiationDone() const;
	std::string const   getBuffer() const;
    unsigned int        getId() const;
    
    // Setters básicos
    void        setNickname(const std::string& nickname);
    void        setUsername(const std::string& username);
    void        setAuthenticated(bool status);
    void        setCapNegotiationStatus(bool status);

    // Buffer para mensajes parciales
    void        appendToBuffer(const std::string& data);
	void		clearBuffer(void);

    // Gestión de canales y operadores
    void        joinChannel(Channel *channel);
    void        leaveChannel(const Channel *channel);
    bool        isInChannel(const Channel *channel) const;
    void        setOperatorStatus(Channel *channel);
    void        removeOperatorStatus(const Channel *channel);
    bool        isOperator(const Channel *channel) const;

    // Comunicación básica
    bool        sendMessage(const std::string& message);
    void        receiveMessage(const std::string &message);
};

#endif