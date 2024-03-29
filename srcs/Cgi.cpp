/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydumaine <ydumaine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/28 09:46:36 by ydumaine          #+#    #+#             */
/*   Updated: 2023/01/30 10:33:33y ydumaine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cgi.hpp"
#include "Config.hpp"
#include "Settings.hpp"

CGI::CGI() : _body(), _env(), _arg(), _file_stdin(), _file_stdout(), _fd_stdin(), _fd_stdout(), _readed(), _cgi_process_body_ready() {}

void CGI::build(Config &conf, const Request &req, struct sockaddr_in const &client_addr)
{
	char client_ip[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &(client_addr.sin_addr), client_ip, INET_ADDRSTRLEN);

	std::map<std::string, std::string> env;
	env["REQUEST_METHOD"] = req.method.type;
	env["QUERY_STRING"] = req.method.parameters;
	env["CONTENT_LENGTH"] = req.header.content_length;
	env["CONTENT_TYPE"] = req.header.content_type;
	env["PATH_INFO"] = req.method.path;
	env["HTTP_USER_AGENT"] = req.header.str_user_agent;
	env["REMOTE_ADDR"] = client_ip;
	env["REMOTE_HOST"] = req.header.host;
	env["REMOTE_USER"] = "";
	env["REMOTE_PASSWORD"] = "";
	env["SERVER_PROTOCOL"] = "HTTP/1.1";
	env["SERVER_SOFTWARE"] = "WebServ/0.1";
	env["SERVER_NAME"] = *conf.getName();
	env["GATEWAY_INTERFACE"] = "CGI/1.1";
	env["SCRIPT_NAME"] = req.method.path;

	Header::t_cookie_it it = req.header.cookies.begin();
	Header::t_cookie_it ite = req.header.cookies.end();
	std::string cookie;
	for (; it != ite; it++)
	{
		cookie += it->first + "=" + it->second + ";";
	}
	env["HTTP_COOKIE"] = cookie;

	// Transformer la map en char ** (excve prend un char **)
	_env = new char *[env.size() + 1];
	if (_env == NULL)
		throw std::exception();
	std::map<std::string, std::string>::iterator start = env.begin();
	std::map<std::string, std::string>::iterator end = env.end();
	int i = 0;
	for (; start != end; start++)
	{
		std::string el = (start->first + "=" + start->second).c_str();
		_env[i] = new char[el.size() + 1];
		if (_env[i] == NULL)
			throw std::exception();
		_env[i] = strcpy(_env[i], el.c_str());
		++i;
	}
	_env[i] = NULL;
	_file_stdin = std::tmpfile();
	_file_stdout = std::tmpfile();
	if (_file_stdin == NULL || _file_stdout == NULL)
		throw std::exception();
	_fd_stdin = fileno(_file_stdin);
	_fd_stdout = fileno(_file_stdout);

	string ext = yd::getExtension(req.method.path);
	if (ext == "")
		throw std::exception();
	std::string const *cgi_path_str = conf.getCgi(req.method.path, ext);
	if (cgi_path_str == NULL)
		throw std::exception();

	this->_arg = new char *[3];
	if (this->_arg == NULL)
		throw std::exception();

	size_t length = 0; 

	length = cgi_path_str->length();
	this->_arg[0] = new char[length + 1];
	if (this->_arg[0] == NULL)
		throw std::exception();
	this->_arg[0][length] = 0;
	cgi_path_str->copy(_arg[0], length);
	
	length = conf.getFile(req.method.path)->length();
	this->_arg[1] = new char[length + 1];
	if (this->_arg[1] == NULL)
		throw std::exception();
	this->_arg[1][length] = 0;
	conf.getFile(req.method.path)->copy(_arg[1], length);

	this->_arg[2] = NULL;
}

CGI::~CGI()
{
	if (_env != NULL)
	{
		for (int i = 0; _env[i] != NULL; i++)
			delete[] _env[i];
		delete[] _env;
		_env = NULL;
	}
	if (_arg != NULL)
	{
		if (_arg[0] != NULL)
			delete _arg[0];
		if (_arg[1] != NULL)
			delete _arg[1];
		delete[] _arg;
		_arg = NULL;
	}
	if (_file_stdin != NULL)
		fclose(_file_stdin);
	if (_file_stdout != NULL)
		fclose(_file_stdin);
	if (_fd_stdin != 0)
		close(_fd_stdin);
	if (_fd_stdout != 0)
		close(_fd_stdout);
	_cgi_process_body_ready = 0;
	_readed = 0;
	_fd_stdout = 0;
	_fd_stdin = 0;
	_body.clear();
	_file_stdin = NULL;
	_file_stdout = NULL;
}

void CGI::execute_cgi(Config &config, Sbuffer &client, struct sockaddr_in const &client_addr)
{
		if (client._status == REQUEST_PARSED)
		{
			client._buffer = CGI::launchProcess(client, config, client_addr);
			client._status = CGI_PROCESS_LAUNCHED;
			if (client._buffer.size() > 0)
				client._status = BODY_GENERATED;
		}
		else 
		{
			CGI::handleProcessResponse(client);
		}
		if (client._status == BODY_GENERATED)
		{
			client._cgi_data.~CGI();
		}
}

void CGI::handleProcessResponse(Sbuffer &client)
{
	if (client._cgi_data._cgi_process_body_ready == false)
	{
		int rt = 0;
		int rc = 0;
		rc = waitpid(client._pid, &rt, WNOHANG);
		//std::cout << "rc" << client._pid << std::endl;
		if (rc == 0) {
			return ;
		} else if (rc == client._pid && client._cgi_data._cgi_process_body_ready == false) {
			if (rt == 1) {
				client._buffer =  error_500(); 
				client._status = BODY_GENERATED; 
			}
			else {
				std::fseek(client._cgi_data._file_stdout, 0, SEEK_END);
				long fileSize = std::ftell(client._cgi_data._file_stdout);
				client._buffer.resize(fileSize);
				client._cgi_data._cgi_process_body_ready = true;
				fseek(client._cgi_data._file_stdout, 0, SEEK_SET);
			}
		} else {
			client._buffer =  error_500(); 
			client._status = BODY_GENERATED; }
	}
	if (client._cgi_data._cgi_process_body_ready == true) 
	{
		int rt = 0;
		rt = fread(&*client._buffer.begin() + client._cgi_data._readed, sizeof(char), 32668, client._cgi_data._file_stdout);
		client._cgi_data._readed += rt;
		if (rt < 32668)
			client._status = BODY_GENERATED;
	}
}

std::vector<char> CGI::launchProcess(Sbuffer &client,  Config &config, struct sockaddr_in const &client_addr)
{
	string scriptName;
	std::vector<char> empty_vector;
	try
	{
		client._cgi_data.build(config, client._req, client_addr);
	}
	catch (std::exception &e)
	{
		return (error_500());
	}
	std::vector<char>::iterator start = client._buffer.begin();
	size_t size_data = client._buffer.size();
	write(client._cgi_data._fd_stdin, &*start, size_data);
	//write(1, &*start, 1000);
	//exit(0);
	client._buffer.clear();
	fseek(client._cgi_data._file_stdin, 0, SEEK_SET);
	pid_t pid = fork(); 
	if (pid == -1)
		return (error_500());
	if (pid == 0)
	{
		dup2(client._cgi_data._fd_stdin, 0);
		dup2(client._cgi_data._fd_stdout, 1);
		execve(client._cgi_data._arg[0], client._cgi_data._arg, client._cgi_data._env);
		std::cerr << "\e[0;31mWebServ$> "
				  << "Execve hasss crashed "
				  << "\e[0m" << std::endl;
		write(1, "Status: 500\r\n\r\n", std::strlen("Status: 500\r\n\r\n"));
		exit(1);
	}
	client._pid = pid;
	return (empty_vector);
}

std::vector<char> CGI::error_500()
{
	std::vector<char> return_value;
	std::string error = "Status: 500\r\n\r\n";
	return_value.insert(return_value.end(), error.begin(), error.end());
	return (return_value);
}
std::vector<char> CGI::error_404()
{
	std::vector<char> return_value;
	std::string error = "Status: 404\r\n\r\n";
	return_value.insert(return_value.end(), error.begin(), error.end());
	return (return_value);
}