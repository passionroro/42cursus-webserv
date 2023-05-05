#ifndef SERVER_HPP
# define SERVER_HPP

# include "webserv.hpp"
# include "config/Config.hpp"
# include "Response.hpp"

typedef struct sockaddr_in saddr_in;
typedef struct sockaddr saddr;


class	Server {

public:

    Server(Object & default_obj);
    Server(Object & default_obj, Object & object);

    /* ACCESSORS */
    unsigned int	getHost(void) const;
    short			getPort(void) const;
    int             getListenFd(void) const;
    int             getSocket(void) const;

    /* SERVER UTILS */
    void accept(void);
    void close(void);
    int	setup(void);
    int recv(void);
    int send(void);

protected:
    /* CFG UTILS */
    void    assignDefaultConfig(Object & object);
    void    assignNewConfig(Object & object);


private:

    /* CFG INFOS */
    std::string _server_name;
    int         _port;
    int         _clt_body_size;
    bool        _auto_index;
    std::vector<std::string>    _address;
    std::vector<std::string>    _disabled_methods;
    std::map<std::string, std::string>  _error_pages;
    std::vector< std::map<std::string, std::string> > _locations;

    /* SERVER UTILS */
    unsigned int	_host; //still useful ?
    Response	_response;
    saddr_in    _addr;
    int			_listen_fd;
    int			_socket;

private:

    Server(void);

};

#endif
