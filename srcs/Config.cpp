/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydumaine <ydumaine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/06 11:31:34 by ydumaine          #+#    #+#             */
/*   Updated: 2023/02/08 21:42:57y ydumaine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//--------------------------------------------------------------------------------------//
//                                        Config                                        //
//--------------------------------------------------------------------------------------//

#include "Config.hpp"
#include <dirent.h>
#include <sys/select.h>

Config &Config::operator=(Config const &other)
{
	if (this != &other)
	{
		_server = other._server;
		_server_selected = other._server_selected;
	}
	return (*this);
}

Config::Config() : _server(), _server_selected(), _pos_server(), _buffer()
{
}

bool Config::selectServ(const uint32_t &int_ip ,const uint16_t &port, const std::string &host)
{
	uint32_t convert_ip = int_ip;
	convert_ip = (((convert_ip >> 24) & 0xFF)       | // échange des octets 1 et 4
    ((convert_ip << 8) & 0xFF0000)   | // échange des octets 2 et 3
    ((convert_ip >> 8) & 0xFF00)      | // échange des octets 3 et 2
    ((convert_ip << 24) & 0xFF000000));
	bool first_serv = 0;
	for (unsigned int i = 0; i < _server.size(); i++)
	{
		if ((uint32_t)_server[i].getIp() == convert_ip && (uint32_t)_server[i].getPort() == port)
		{
			if (!first_serv)
			{
				_server_selected = &_server[i];
				first_serv = 1;
			}
			const std::string *server_name = _server[i].getServerName();
			if ((*server_name).size() > 0)
			{
				for (int j = 0; (*server_name)[j] == host[j]; j++)
				{
					if ((*server_name)[j] == '\0' && host[j] == '\0')
					{
						_server_selected = &_server[i];
						return (1);
					}
				}
			}
		}
	}
	if (first_serv == 0)
		return (0);
	else
		return (1);
}
const Location *Config::getLocation(const std::string &path) const
{
	return (_server_selected->getLocation(path));
}
const std::string *Config::getFile(const std::string &path)
{
	const Location *loc = getLocation(path);
	if (loc == NULL)
		return (NULL);
	else
	{
		_buffer = path;
		for (int i = 1; path[i]; i++)
		{
			if (path[i] != loc->_path[i])
			{
				if (i > 1)
					_buffer.erase(0, i);
				break;
			}
		}
		_buffer = (loc->_root + _buffer);
		if (_buffer.back() == '/')
			_buffer.pop_back();
		if (yd::compare_strings_ignoring_trailing_slash(path, loc->_path))
			return (&loc->_default_file);
		else
		{
			int i = 0;
			if (_buffer.c_str()[0] == '/')
				i = 1;
			DIR *dir = opendir(_buffer.c_str() + i);
			if (dir)
			{
				closedir(dir);
				return (&loc->_default_file);
			}
			return (&_buffer);
		}
	}
}

const std::string *Config::getPath(const std::string &path)
{
	const Location *loc = getLocation(path);
	if (loc == NULL)
		return (NULL);
	else
	{
		_buffer = path;
		for (int i = 1; path[i]; i++)
		{
			if (path[i] != loc->_path[i])
			{
				if (i > 1)
					_buffer.erase(0, i);
				break;
			}
		}
		_buffer = (loc->_root);
		if (_buffer.back() != '/')
			_buffer.push_back('/');
		return (&_buffer);
		}
}
Methods Config::getMethod(const std::string &path) const
{
	const Location *loc = getLocation(path);
	if (loc == NULL)
		return (Methods());
	Methods rt;
	rt.isget = false;
	rt.ispost = false;
	rt.isdelete = false;
	if (loc->_is_get == 1)
		rt.isget = true;
	if (loc->_is_post == 1)
		rt.ispost = true;
	if (loc->_is_delete == 1)
		rt.isdelete = true;
	return (rt);
}

const std::string *Config::getError(const unsigned int error) const
{
	return (_server_selected->getErrorPages(error));
}
const std::string *Config::getCgi(const std::string &path, const std::string &cgi) const
{
	const Location *loc = getLocation(path);
	if (loc == NULL)
		return (NULL);
	return (loc->getCgi(cgi));
}

const std::string Config::getRedirectionType(const std::string &path)
{
	const Location *loc = getLocation(path);
	if (loc == NULL)
		return ("");
	return (loc->_redirection_type);
}

const std::string Config::getRedirectionUrl(const std::string &path)
{
	const Location *loc = getLocation(path);
	if (loc == NULL)
		return ("");
	return (loc->_redirection_url);
}

unsigned int Config::getMaxSize() const
{
	return (_server_selected->getMaxBodySize());
}

bool Config::getUpload(const std::string &path) const
{
	const Location *loc = getLocation(path);
	if (loc == NULL)
		return (0);
	if (loc->_upload_allow == 1)
		return (1);
	return (0);
}

const std::string *Config::getName() const
{
	return (_server_selected->getServerName());
}

std::string Config::getIp() const
{
	return (yd::intToIP(_server_selected->getIp()));
}
std::string Config::getPort() const
{
	unsigned int p = _server_selected->getPort();
	std::stringstream ss;
	ss << p;
	return (ss.str());
}

std::string Config::getDirectoryListing(const std::string &path) const
{
	const Location *loc = getLocation(path);
	if (loc == NULL || loc->_directory_listing == 0)
		return ("");
	return (loc->_root);
}

unsigned int Config::getServNumb() const
{
	return (_server.size());
}

void Config::selectFirstServ()
{
	_server_selected = &_server[0];
	_pos_server = 0;
}

const std::vector<Server> &Config::getServerTab() const
{
	return (_server);
}

void Config::operator++()
{
	if (_pos_server + 1 == _server.size())
		return;
	else
		_server_selected = &_server[++_pos_server];
}
void Config::operator--()
{
	if (_pos_server == 0)
		return;
	else
		_server_selected = &_server[--_pos_server];
}

//--------------------------------------------------------------------------------------//
//                                      libft Yann                                      //
//--------------------------------------------------------------------------------------//

bool yd::isSocketReadyToWrite(int socket_fd)
{
	fd_set write_fds;

	FD_ZERO(&write_fds);
	FD_SET(socket_fd, &write_fds);

	int result = select(socket_fd + 1, NULL, &write_fds, NULL, NULL);
	if (result > 0 && FD_ISSET(socket_fd, &write_fds))
	{
		return true;
		std::cout << "select OK" << std::endl;
	}
	else
	{
		std::cout << "select error" << std::endl;
		return false;
	}
}

size_t yd::stoui(const std::string &s)
{
	size_t rt = 0;
	for (unsigned int i = 0; i < s.size(); i++)
	{
		if (s[i] < '0' || s[i] > '9')
			return (0);
		rt = rt * 10 + (s[i] - '0');
	}
	return (rt);
}

bool yd::isValidPathDir(std::string const &s)
{
	for (unsigned int i = 0; i < s.size(); i++)
	{
		if (s[i] == '/' && s[i + 1] == '/')
		{
			return false;
		}
	}
	return true;
}

bool yd::compare_strings_ignoring_trailing_slash(const std::string &str1, const std::string &str2)
{
	size_t str1_size = str1.size(), str2_size = str2.size();
	if (str1_size - (str1.back() == '/') != str2_size - (str2.back() == '/'))
		return false;
	for (size_t i = 0; i < str1_size - (str1.back() == '/'); i++)
		if (str1[i] != str2[i])
			return false;
	return true;
}

bool yd::isValidPathFile(std::string const &s)
{
	std::string::size_type pos = s.find("//");
	if (pos != std::string::npos)
	{
		return false;
	}
	return true;
}

std::string yd::getExtension(std::string const &path)
{
	std::string::size_type position = 0;
	std::string scriptName;
	if ((position = path.rfind("/")) != std::string::npos)
	{
		scriptName = path.substr(position + 1);
	}
	std::string::size_type dotpos;
	if ((dotpos = scriptName.rfind(".")) != std::string::npos)
		return (scriptName.substr(dotpos));
	return ("");
}

std::string yd::generateSessionId()
{
	static int i;
	if (i == INT_MAX)
		i = 0;
	std::string chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
	std::string sessionId = "";
	srand(time(NULL) + i);
	for (int i = 0; i < 16; i++)
	{
		sessionId += chars[rand() % chars.length()];
	}
	i++;
	return sessionId;
}

bool yd::ends_with_rn(std::string const &str)
{
	std::string const delimiter = "\r\n\r\n";
	size_t const len = delimiter.size();

	return (str.length() >= len && str.find(delimiter) == str.length() - len);
}

// path1 must be location path

int yd::comparePath(const std::string &path1, const std::string &path2)
{
	int count = 0;
	unsigned int i = 1;
	for (; i < path1.size(); i++)
	{
		if (path1[i] != path2[i])
		{
			if (path1[i] != '/' || (path2[i] != '/' && path2[i] != '\0'))
				return (0);
			else
				break;
		}
		if ((path1[i] == '/' && path2[i] == '/'))
		{
			count++;
		}
	}
	if (path1.size() == 1 && path1[0] == '/')
	{
		count++;
		return (count);
	}
	if (path1[i] == '/' && path2[i] == '/')
	{
		count++;
		count++;
		if (path1[i + 1] != '\0')
			count = 0;
		return (count);
	}
	if ((path1[i - 1] == '/' && path2[i - 1] == '/'))
	{
		count++;
		return (count);
	}
	if (path1[i] == '/' && path2[i] == '\0')
	{
		count++;
		count++;
		if (path1[i + 1] != '\0')
			count = 0;
		return (count);
	}
	if (path1[i] == '\0' && path2[i] == '/')
	{
		count++;
		count++;
		return (count);
	}
	if ((path1[i - 1] != '/' && path1[i] == '\0') && (path2[i - 1] != '/' && path2[i] == '\0'))
	{
		count++;
		count++;
		return (count);
	}
	if (path1.size() >= path2.size() + 1)
		count = 0;
	if (path2.size() >= path1.size() + 1)
		count = 0;
	return (count);
}
std::string yd::intToIP(unsigned int ip)
{
	std::stringstream ss;
	ss << ((ip >> 24) & 0xff) << "."
		 << ((ip >> 16) & 0xff) << "."
		 << ((ip >> 8) & 0xff) << "."
		 << (ip & 0xff);
	return ss.str();
}

void yd::extractHeader(std::string &header, std::vector<char> &req)
{

	char sep[4] = {'\r', '\n', '\r', '\n'};
	size_t header_size = 0;
	if (req.size() < 4)
		return;
	for (size_t i = 0; i < req.size() - 3; i++)
	{
		if (req[i] == sep[0] && req[i + 1] == sep[1] && req[i + 2] == sep[2] && req[i + 3] == sep[3])
		{
			header_size = i + 4;
			break;
		}
	}
	for (size_t i = 0; i < header_size; i++)
	{
		header.push_back(req[i]);
	}
	std::vector<char>::const_iterator end = req.begin() + header_size;
	req.erase(req.begin(), end);
}

void yd::copyHeader(std::string &header, std::vector<char> &req)
{

	char sep[4] = {'\r', '\n', '\r', '\n'};
	size_t header_size = 0;
	if (req.size() < 4)
		return;
	for (size_t i = 0; i < req.size() - 3; i++)
	{
		if (req[i] == sep[0] && req[i + 1] == sep[1] && req[i + 2] == sep[2] && req[i + 3] == sep[3])
		{
			header_size = i + 4;
			break;
		}
	}
	for (size_t i = 0; i < header_size; i++)
	{
		header.push_back(req[i]);
	}
}

//--------------------------------------------------------------------------------------//
//                                       Location                                       //
//--------------------------------------------------------------------------------------//

std::string const *Location::getCgi(std::string const &cgi) const
{
	std::string const *ptr;
	try
	{
		ptr = &_cgi.at(cgi);
		return (ptr);
	}
	catch (std::out_of_range)
	{
		return (NULL);
	}
}

void Location::setCgi(Tokenizer &tok)
{
	if (tok.getToken()[0] != '.')
		throw(FormatError(tok.getToken(), ".extension"));
	for (unsigned int i = 1; i < tok.getToken().length(); i++)
	{
		if (!isalpha(tok.getToken()[i]))
			throw(FormatError(tok.getToken(), ".extension"));
	}
	if (tok.nextToken()[0] == ';')
		throw(ConfigurationError("cgi need two arguments"));
	if (!yd::isValidPathDir(tok.getToken()) && !yd::isValidPathFile(tok.getToken()))
		throw(FormatError(tok.getToken(), "path /x/x/x"));
	_cgi[tok.getTokenBefore()] = tok.getToken();
}

void Location::setMethod(Tokenizer &tok)
{
	if (tok.getToken() == "GET")
		_is_get = 1;
	else if (tok.getToken() == "POST")
		_is_post = 1;
	else if (tok.getToken() == "DELETE")
		_is_delete = 1;
	else
		throw(FormatError(tok.getToken(), "must be GET, POST or DELETE"));
}

void Location::setRedirection(Tokenizer &tok)
{
	_redirection_url = tok.getToken();
	if (tok.nextToken() == "temporary")	
	{
		_redirection_type = "temporary";	
	}
	else if (tok.getToken() == "permanent")
	{
		_redirection_type = "permanent";
	}
	else 
		throw(FormatError(tok.getToken(), "Redirection must be temporary or permanent"));
}

void Location::setDefaultFile(Tokenizer &tok)
{
	if (!yd::isValidPathFile(tok.getToken()))
		throw(FormatError(tok.getToken(), "// forbiden in a path"));
	else
		_default_file = tok.getToken();
}

void Location::setDirectoryListing(Tokenizer &tok)
{
	if (tok.getToken() == "on")
		_directory_listing = true;
	else if (tok.getToken() == "off")
		_directory_listing = false;
	else
		throw(FormatError(tok.getToken(), "must be \"on\" or \"off\""));
}

void Location::setRoot(Tokenizer &tok)
{
	if (!yd::isValidPathDir(tok.getToken()))
		throw(FormatError(tok.getToken(), "// forbiden in a path"));
	if (tok.getToken().back() == '/')
		throw(FormatError(tok.getToken(), "/x/x/x"));
	else
		_root = tok.getToken();
}

void Location::setUploadFile(Tokenizer &tok)
{
	if (tok.getToken() == "on")
	{
		this->_upload_allow = true;
	}
	else if (tok.getToken() != "off")
		throw(FormatError(tok.getToken(), "Unexcpected token, expected \"on\" or \"off\""));
}

void Location::setPath(Tokenizer &tok)
{
	if (!yd::isValidPathDir(tok.getToken()))
		throw(FormatError(tok.getToken(), "// forbiden in a path"));
	if (tok.getToken()[0] != '/')
		throw(FormatError(tok.getToken(), "location path must start with /"));
	if (tok.getToken().size() > 1 && tok.getToken().back() == '/')
		throw(FormatError(tok.getToken(), "path /x/x/x not /x/x/x/"));
	else
		_path = tok.getToken();
}

Location::Location() : _upload_allow(false), _default_file(), _root(), _path(), _redirection_url(), _redirection_type(),
						 _cgi(), _is_get(), _is_post(), _is_delete(), _directory_listing()
{
	_tokens["root"] = &Location::setRoot;
	_tokens["cgi"] = &Location::setCgi;
	_tokens["upload_file"] = &Location::setUploadFile;
	_tokens["root"] = &Location::setRoot;
	_tokens["allow"] = &Location::setMethod;
	_tokens["default_file"] = &Location::setDefaultFile;
	_tokens["directory_listing"] = &Location::setDirectoryListing;
	_tokens["rewrite"] = &Location::setRedirection;
}

void (Location::*Location::selectSetter(std::string const &token))(Tokenizer &tok)
{
	std::map<std::string, void (Location::*)(Tokenizer & tok)>::iterator start = _tokens.begin();
	std::map<std::string, void (Location::*)(Tokenizer & tok)>::iterator end = _tokens.end();
	for (; start != end; start++)
	{
		if (token == (*start).first)
			return (start->second);
	}
	return (NULL);
}
//--------------------------------------------------------------------------------------//
//                                        Server                                        //
//--------------------------------------------------------------------------------------//

bool Server::checkServer()
{
	if (_port == INT_MAX)
		throw(ConfigurationError("port configuration is missing for a server"));
	if (_ip == 0)
		throw(ConfigurationError("ip configuration is missing for a server"));
	if (_locations.size(	) == 0)
		throw(ConfigurationError("a server need at least one Location"));
	for (unsigned int i = 0; i < _locations.size(); i++)
	{
		if (_locations[i]._root.empty())
			throw(ConfigurationError("a location need at least a root"));
	}
	return (0);
}
const Location *Server::getLocation(std::string const &path) const
{
	std::string path_select;
	Location const *location_select;
	Location const *location_choose = NULL;
	int length = 0;
	int prev_length = 0;

	// std::cout << "test" << std::endl;
	for (unsigned int i = 0; i < _locations.size(); i++)
	{
		location_select = &_locations[i];
		prev_length = length;
		length = yd::comparePath(location_select->_path, path);
		if (length > prev_length)
			location_choose = location_select;
	}
	// std::cout << "test 2 : "<<  location_choose->_is_get << std::endl;
	return (location_choose);
}

unsigned int Server::getPort() const
{
	return (_port);
}
uint32_t Server::getIp() const
{
	return (_ip);
}
size_t Server::getMaxBodySize() const
{
	return (_max_body_size);
}
const std::string *Server::getServerName() const
{
	return (&_server_name);
}
void Server::setDefault()
{
	_default = 1;
}
bool Server::getDefault() const
{
	return (_default);
}

const std::string *Server::getErrorPages(unsigned int error) const
{
	std::string const *ptr;
	try
	{
		ptr = &_error_pages.at(error);
		return (ptr);
	}
	catch (std::out_of_range)
	{
		return (NULL);
	}
}

void Server::setPort(Tokenizer &tok)
{
	if (yd::stoui(tok.getToken().c_str()) <= 0 || yd::stoui(tok.getToken().c_str()) > 65535)
		throw(FormatError(tok.getToken(), "a number enter 1 and 65535 "));
	if (_port != INT_MAX)
		throw(ConfigurationError("can't set port twice for a server"));
	_port = yd::stoui(tok.getToken().c_str());
}

void Server::setServerName(Tokenizer &tok)
{
	if (tok.getToken().find('/', 0) != (unsigned long)-1)
		throw(FormatError(tok.getToken(), "/ forbiden in server_name"));
	_server_name = tok.getToken();
}

void Server::setMaxBody(Tokenizer &tok)
{
	_max_body_size = yd::stoui(tok.getToken().c_str());
}

void Server::setIp(Tokenizer &tok)
{
	int dots = 0;
	for (unsigned int i = 0; i < tok.getToken().length(); i++)
	{
		if (tok.getToken()[i] == '.')
		{
			dots++;
		}
		else if (!isdigit(tok.getToken()[i]))
		{
			throw(FormatError(tok.getToken(), "Only numbers"));
		}
	}
	if (dots != 3)
	{
		throw(FormatError(tok.getToken(), "xxx.xxx.xxx.xxx"));
	}

	unsigned int num = 0;
	unsigned int final_num = 0;
	unsigned int octet = 3;
	for (unsigned int i = 0; i <= tok.getToken().length(); i++)
	{
		if (tok.getToken()[i] == '.' || i == tok.getToken().length())
		{
			if (num < 0 || num > 255)
			{
				throw(FormatError(tok.getToken(), "numbers between 0 and 255"));
			}
			final_num = ((num << (8 * octet)) + final_num);
			octet--;
			num = 0;
		}
		else
		{
			num = num * 10 + (tok.getToken()[i] - '0');
		}
	}
	_ip = final_num;
}

void Server::setError(Tokenizer &tok)
{
	unsigned int code = yd::stoui(tok.getToken().c_str());
	if (code < 100 || code > 600)
		throw(FormatError(tok.getToken(), "numbers between 100 and 600"));
	if (_error_pages[code].empty())
	{
		_error_pages[code] = tok.nextToken();
	}
	else
		throw(ConfigurationError("redefinition of error page"));
}

Server::Server() : _server_name(), _port(INT_MAX), _ip(0), _max_body_size(std::numeric_limits<size_t>::max()),
									 _error_pages(), _locations(), _tokens()
{
	_tokens["server_name"] = &Server::setServerName;
	_tokens["max_body_size"] = &Server::setMaxBody;
	_tokens["port"] = &Server::setPort;
	_tokens["ip"] = &Server::setIp;
	_tokens["error"] = &Server::setError;
}

void (Server::*Server::selectSetter(std::string const &token))(Tokenizer &tok)
{
	std::map<std::string, void (Server::*)(Tokenizer & tok)>::iterator start = _tokens.begin();
	std::map<std::string, void (Server::*)(Tokenizer & tok)>::iterator end = _tokens.end();
	for (; start != end; start++)
	{
		if (token == start->first)
			return (start->second);
	}
	return (NULL);
}

void Server::addLocation(Location location)
{
	this->_locations.push_back(location);
}

// Exception gestion for token
Tokenizer::Unexpected::Unexpected(std::string token)
{
	std::cout << "\e[0;31mWebServ$> Syntax error: near unexpected token `" + token + "\".\e[0m " << std::endl;
}

FormatError::FormatError(std::string token, std::string expected_format)
{
	std::cout << "\e[0;31mWebServ$> Format error: \"" << token << "\" "
						<< "Format expected : \"" << expected_format << "\".\e[0m" << std::endl;
}

ConfigurationError::ConfigurationError(std::string message)
{
	std::cout << "\e[0;31mWebServ$> Configuration error: " << message << ".\e[0m " << std::endl;
}

//--------------------------------------------------------------------------------------//
//                                        Config                                        //
//--------------------------------------------------------------------------------------//

Config::Config(std::string const &path)
{
	Tokenizer tokenizer(*this, path);
	std::cout << "\e[0;32m\e[1mWebServ$> Configuration OK. \e[0m" << std::endl;
}

void Config::addServer(Server server)
{
	bool default_already_set = 0;
	for (unsigned int i = 0; i < _server.size(); i++)
	{
		if (_server[i].getPort())
			_server[i].getDefault();
	}
	if (default_already_set == 0)
		server.setDefault();
	this->_server.push_back(server);
}

//--------------------------------------------------------------------------------------//
//                                        Tokenizer                                     //
//--------------------------------------------------------------------------------------//

const std::string &Tokenizer::nextToken()
{
	_token_before = _token;
	if (_str.empty())
		_file >> _str;
	if (_str.find(';') || _str[0] == ';')
	{
		if (_str[0] == ';')
			_token = ";";
		else
			_token = _str.substr(0, _str.find_first_of(';'));
		if (_str[0] == ';')
			_str.erase(0, 1);
		else
			_str.erase(0, _str.find_first_of(';'));
	}
	return (_token);
}

const std::string &Tokenizer::getToken()
{
	return (_token);
}

const std::string &Tokenizer::getTokenBefore()
{
	return (_token_before);
}

Tokenizer::Tokenizer(Config &config, std::string const &path)
{
	_file.open(path, std::ios::in);
	bool atLeastOneServer = 0;
	if (_file.is_open())
	{
		while (_file)
		{
			nextToken();
			if (!_file)
				break;
			;
			if (_token == "server")
			{
				nextToken();
				if (_token == "{")
				{
					nextToken();
					config.addServer(parsServer());
					atLeastOneServer = 1;
				}
				else
					throw Tokenizer::Unexpected(_token);
			}
			else
				throw Tokenizer::Unexpected(_token);
		}
	}
	else
		throw std::runtime_error("\e[0;31mWebServ$> File error: couldn't open config file. \e[0m");
	if (atLeastOneServer == 0)
		throw(ConfigurationError("Webserv need at least one server configure to working"));
	std::vector<Server>::iterator start = config._server.begin();
	/*for (; start != config._server.end(); start++)
	{
		if ((*start).getIp() != ip)
			throw(ConfigurationError("Webserv can't handle multiple ip address"));
	}*/
	start = config._server.begin();

// For the moment webserv can handle multiple server with same ip, port, add this code to prevent it
#if ONE_PORT_PER_CONFIG

	std::vector<Server>::iterator it = config._server.begin();
	for (; start != config._server.end(); start++)
	{
		it = config._server.begin();
		for (; it != config._server.end(); it++)
		{
			if (it != start && (*it).getPort() == (*start).getPort() && (*it).getIp() == (*start).getIp())
				throw(ConfigurationError("Webserv can't handle multiple server with same ip, port"));
		}
	}
#endif
}

Server Tokenizer::parsServer()
{
	Server server;
	void (Server::*pf)(Tokenizer & tok);
	while (_token != "}")
	{
		if (_token == "location")
		{
			nextToken();
			server.addLocation(parsLocation());
			nextToken();
		}
		else
		{
			pf = server.selectSetter(_token);
			if (pf == NULL)
				throw Tokenizer::Unexpected(_token);
			nextToken();
			if (getToken() == ";")
				throw(ConfigurationError("`" + getTokenBefore() + "\" need at least one argument"));
			(server.*pf)(*this);
			nextToken();
			if (getToken() != ";")
				throw Tokenizer::Unexpected(_token);
			nextToken();
		}
	}
	server.checkServer();
	return (server);
}

Location Tokenizer::parsLocation()
{
	Location location;
	void (Location::*pf)(Tokenizer & tok);
	location.setPath(*this);
	nextToken();
	if (getToken() != "{")
		throw Tokenizer::Unexpected(_token);
	nextToken();
	while (getToken() != "}")
	{
		pf = location.selectSetter(_token);
		if (pf == NULL)
			throw Tokenizer::Unexpected(_token);
		if (getToken() == ";")
			throw(ConfigurationError(getTokenBefore() + "need at least one argument"));
		nextToken();
		(location.*pf)(*this);
		nextToken();
		if (getToken() != ";")
			throw Tokenizer::Unexpected(_token);
		nextToken();
	}
	return (location);
}
