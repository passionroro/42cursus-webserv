#ifndef SERVER_HPP
# define SERVER_HPP

# include "webserv.hpp"
# include "config/Config.hpp"
# include "Request.hpp"
# include "Response.hpp"

typedef struct sockaddr_in saddr_in;
typedef struct sockaddr saddr;

class	Server {

public:

    Server(Object & default_obj);
    Server(Object & default_obj, Object & object);

    /* ACCESSORS */
    unsigned int	getHost() const;
    int             getListenFd() const;
	std::queue<int>             getSocket() const;

    std::string getServerName() const;
    int         getPort() const;
    int         getClientBodySize() const;
    bool        getAutoIndex() const;
	MapStr      getErrorPages() const;
	std::vector<std::string>    getUploads() const;
    std::vector<std::string>    getAddress() const;
    std::vector<std::string>    getDisabledMethods() const;

    /* SERVER UTILS */
    void accept(void);
    void close(void);
    int	setup(void);
    int recv(int socket);
    int send(int socket);

	void	printSocket(void);

	/* GETTERS */

	int	getPort(void)	{ return _port; }
	std::string	getServerName(void)	{ return _server_name; }
	std::vector<std::string>&	getAddress(void) { return _address; }
	std::vector<std::string>	getDisabledMethods(void) { return _disabled_methods; }
	Locations		getLocations(void) { return _locations; }
	Locations		getCgi(void) { return _cgi; }
	Redirection		getRedirection(void) { return _redirection; }


protected:
    /* CFG UTILS */
    void    assignDefaultConfig(Object & object);
    bool	assignNewConfig(Object & object);


private:

    /* CFG INFOS */
    std::string _server_name;
    int         _port;
    int         _clt_body_size;
    bool        _auto_index;
    std::vector<std::string>    _address;
    std::vector<std::string>    _disabled_methods;
	std::vector<std::string>	_uploads;
	MapStr      _error_pages;
    Locations   _locations;
	Redirection	_redirection;
	Locations	_cgi;

	std::map<int, std::string>	_recvStr;
	std::string	_toSend;

    /* SERVER UTILS */
    unsigned int	_host; //still useful ?
    Response	_response;
    saddr_in    _addr;
    int			_listen_fd;
	std::queue<int>			_socket;

private:

    Server(void);

};

#endif
