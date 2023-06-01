#include "Cgi.hpp"
#include "Server.hpp"
#include <unistd.h>

Cgi::Cgi(Request & request, Server& conf) : _r(request), _headers(request.getRequestHeaders())
{
	initEnv(conf);
	execute(conf);
}

Cgi::~Cgi(void)
{
}

void	Cgi::initEnv(Server& conf)
{
	(void)conf;

	if (_headers.find("Auth-Scheme") != _headers.end())
		_env["AUTH_TYPE"] = _headers["Authorization"];
	
	if (_headers.find("Content-Length") != _headers.end())
		_env["CONTENT_LENGTH"] = _headers["Content-Length"];
	
	if (_headers.find("Content-Type") != _headers.end())
		_env["CONTENT_TYPE"] = _headers["Content-Type"];

	_env["GATEWAY_INTERFACE"] = "CGI/1.1";

	_env["REDIRECT_STATUS"] = "0";

	// PATH_INFO
	//_env["PATH_INFO"] = _r.getPath();
	// PATH_TRANSLATED
	// QUERY_STRING
	
	// REMOTE_ADDR
	//
	// REQUEST_METHOD
	_env["REQUEST_METHOD"] = _r.getMethod();
	//
	// SCRIPT_NAME = path before path info
	char	buf[80];
	getcwd(buf, 80);
	std::string	cwd = buf;
	//std::cout << "cwd: " << cwd << std::endl;
	//_env["SCRIPT_NAME"] = cwd + "/" + _r.getPath();
	_env["SCRIPT_NAME"] = _r.getRequestPath();
	_env["SCRIPT_FILENAME"] = _r.getPath();
	//_env["PATH_INFO"] = cwd + "/" + _r.getPath();
	_env["PATH_INFO"] = "";
	_env["SERVER_NAME"] = conf.getServerName();

	_env["DOCUMENT_ROOT"] = cwd + "/" + _r.getPath().substr(0, _r.getPath().find(_r.getRequestPath()));;

	// todo continue read from 4.1.15

	_env["SERVER_PROTOCOL"] = "HTTP/1.1";

	_env["CONTENT_TYPE"] = _headers["Content-Type"];

	_env["SERVER_SOFTWARE"] = "nou";
}

void	Cgi::execute(Server& conf)
{
	(void)conf;
	pid_t	pid;
	int	pipe1[2];
	int	pipe2[2];

	if (pipe(pipe1) < 0)
	{
		_error = true;
		return ;
	}
	if (pipe(pipe2) < 0)
	{
		_error = true;
		return ;
	}
	
	pid = fork();
	if (pid == -1)
	{
		std::cout << "fork failed" << std::endl;
		_error = true;
		return ;
	}
	else if (pid == 0) // child
	{
		std::cout << "child alive" << std::endl;
		
		std::string	bin = _r.getCgiBin();
	
		char	buf[80];
		getcwd(buf, 80);
		std::string	cwd = buf;

		std::string	path = cwd + "/" + _r.getPath();

		char**	arg;
		arg = new char*[3];
		arg[0] = new char[bin.size() + 1];
		arg[0] = ::strcpy(arg[0], bin.c_str());
		arg[1] = new char[path.size() + 1];
		arg[1] = ::strcpy(arg[1], path.c_str());
		arg[2] = NULL;

		std::cout << "bin: " << arg[0] << std::endl;
		std::cout << "file: " << arg[1] << std::endl;

		dup2(pipe1[0], STDIN_FILENO);
		dup2(pipe2[1], STDOUT_FILENO);

		close(pipe1[0]);
		close(pipe1[1]);
		close(pipe2[0]);
		close(pipe2[1]);

		execve(bin.c_str(), arg, getEnvv());
		std::cout << "child bug" << std::endl;
		perror("yo: ");
		exit(1);
	}
	else
	{
		//sleep(1);
		std::cout << "parent alive" << std::endl;

		close(pipe1[0]);
		close(pipe2[1]);
		if (_r.getMethod() == "POST")
		{
			std::cout << "RequestBody sent to CGI: " << std::endl;
			std::cout << _r.getRequestBody() << std::endl;
			write(pipe1[1], _r.getRequestBody().c_str(), _r.getRequestBody().size());
		}
		close(pipe1[1]);
		std::cout << "parent wait for " << pid << std::endl;
		int	status;
		waitpid(pid, &status, 0);
		if (WIFEXITED(status) && WEXITSTATUS(status))
		{
			_error = true;
			std::cout << "child exit error" << std::endl;
		}

		_res = readRes(pipe2[0]);
		close(pipe2[0]);
		//std::cout << "Res:" << std::endl;
		//std::cout << _res << std::endl;


	}

}

std::string	Cgi::readRes(int fd)
{
	std::string	res;
	int		tmp = 0;
	char	buf[BUFSIZE];

	while ((tmp = read(fd, buf, BUFSIZE - 1)) > 0)
	{
		if (tmp == -1)
		{
			std::cout << "Error: cgi readRes read" << std::endl;
			_error = true;
			return "";
		}
		buf[tmp] = '\0';
		res.insert(res.length(), buf, tmp);
	}
	return (res);
}

char**	Cgi::getEnvv(void) const
{
	char	**envv;
	int	i = 0;

	envv = new char*[_env.size() + 1];
	for (std::map<std::string, std::string>::const_iterator it = _env.begin() ; it != _env.end() ; it++)
	{
		std::string	str = it->first + "=" + it->second;
		envv[i] = new char[str.size() + 1];
		envv[i] = ::strcpy(envv[i], str.c_str());
		i++;
	}
	envv[i] = NULL;
	return (envv);
}

//	private

Cgi::Cgi(void)
{
}
