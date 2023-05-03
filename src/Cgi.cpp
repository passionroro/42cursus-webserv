#include "Cgi.hpp"

Cgi::Cgi(Request & request) : _r(request), _headers(request.getRequestHeaders())
{ initEnv();
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
		return ; //todo handle error
	else if (pid == 0) // child
	{

		dup2(pipe1[0], STDIN_FILENO);
		dup2(pipe2[1], STDOUT_FILENO);

		close(pipe1[0]);
		close(pipe1[1]);
		close(pipe2[0]);
		close(pipe2[1]);

		//execve(path, , _env);
	}

}

char**	Cgi::getEnvv(void) const
{
	char	**envv;
	int	i = 0;

	envv = new char*[_env.size() + 1];
	for (std::map<std::string, std::string>::iterator it = _env.begin() ; it != _env.end() ; it++)
	{
		std::string	str = it->first + "=" + it->second;
		envv[i] = new char[str.size() + 1];
		envv[i] = ::strcpy(envv[j], str.c_str());
		i++;
	}
	env[i] = NULL;
	return (env);
}

//	private

Cgi::Cgi(void)
{
}
