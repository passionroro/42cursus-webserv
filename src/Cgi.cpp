#include "Cgi.hpp"
#include <unistd.h>

Cgi::Cgi(Request & request) : _r(request), _headers(request.getRequestHeaders())
{
	initEnv();
	execute();
}

Cgi::~Cgi(void)
{
}

void	Cgi::initEnv(void)
{
	_env["GATEWAY_INTERFACE"] = "CGI/1.1";

	_env["SERVER_PROTOCOL"] = "HTTP/1.1";

	if (_headers.find("Content-Length") != _headers.end())
		_env["CONTENT_LENGTH"] = _headers["Content-Length"];
	_env["CONTENT_TYPE"] = _headers["Content-Type"];

	_env["SERVER_SOFTWARE"] = "nou";
}

void	Cgi::execute(void)
{
	pid_t	pid;
	int	pipe1[2];
	int	pipe2[2];

	if (pipe(pipe1) < 0)
		return ; //todo handle error
	if (pipe(pipe2) < 0)
		return ; //todo handle error
	
	pid = fork();
	if (pid == -1)
	{
		std::cout << "fork failed" << std::endl;
		return ; //todo handle error
	}
	else if (pid == 0) // child
	{
		std::cout << "child alive" << std::endl;
		
		std::string	path = "cgi-bin/cgi-hello";
		char**	arg;
		arg = new char*[2];
		arg[0] = new char[path.size() + 1];
		arg[0] = ::strcpy(arg[0], path.c_str());
		arg[1] = NULL;

		std::cout << "path: " << path << std::endl;
		std::cout << "arg: " << arg[0] << std::endl;

		dup2(pipe1[0], STDIN_FILENO);
		dup2(pipe2[1], STDOUT_FILENO);

		close(pipe1[0]);
		close(pipe1[1]);
		close(pipe2[0]);
		close(pipe2[1]);

		execve(path.c_str(), arg, getEnvv());
		std::cout << "child bug" << std::endl;
		perror("yo: ");
		exit(1);
	}
	else
	{
		std::cout << "parent alive" << std::endl;

		close(pipe1[0]);
		close(pipe1[1]);
		close(pipe2[1]);

		std::cout << "parent wait for " << pid << std::endl;
		int	status;
		waitpid(pid, &status, 0);
		if (WIFEXITED(status) && WEXITSTATUS(status))
		{
			// todo handle error
			std::cout << "child exit error" << std::endl;
		}
		std::cout << "parent waited for " << pid << std::endl;

		_res = readRes(pipe2[0]);
		close(pipe2[0]);
		std::cout << "all closed" << std::endl;


		
	}

}

std::string	Cgi::readRes(int fd)
{
	std::string	res;
	int		tmp = 0;
	char	buf[BUFSIZE];

	lseek(fd, 0, SEEK_SET);
	while ((tmp = read(fd, buf, BUFSIZE - 1)) > 0)
	{
		if (tmp == -1)
		{
			std::cout << "Error: cgi readRes read" << std::endl;
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
