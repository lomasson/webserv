
#ifndef CONFIG_HPP
#define CONFIG_HPP
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <sstream>

#define RED "\e[0;31m"
#define DEF "\e[0m"
#define ONE_PORT_PER_CONFIG 0

struct Methods
{
	bool isget;
	bool ispost;
	bool isdelete;
};

// this class serv to extracte and create config and serveur object in a file
namespace yd
{
	bool isSocketReadyToWrite(int socket_fd);
	bool isValidPathDir(std::string const &s);
	bool isValidPathFile(std::string const &s);
	int comparePath(const std::string &path1, const std::string &path2);
	std::string intToIP(unsigned int ip);
	bool compare_strings_ignoring_trailing_slash(const std::string &str1, const std::string &str2);
	std::string getExtension(std::string const &path);
	std::string generateSessionId();
	bool ends_with_rn(std::string const &str);
	void extractHeader(std::string &header, std::vector<char> &req);
	void copyHeader(std::string &header, std::vector<char> &req);
	size_t stoui(const std::string &s);
}

class Tokenizer;

class Location
{
public:
	Location();
	void setRoot(Tokenizer &tok);
	void setDefaultFile(Tokenizer &tok);
	void setDirectoryListing(Tokenizer &tok);
	void setCgi(Tokenizer &tok);
	void setUploadFile(Tokenizer &tok);
	void setMethod(Tokenizer &tok);
	void setPath(Tokenizer &tok);
	void setRedirection(Tokenizer &tok);
	std::string const *getCgi(std::string const &cgi) const;
	void (Location::*selectSetter(std::string const &token))(Tokenizer &tok);
	bool _upload_allow;
	std::string _default_file;
	std::string _root;
	std::string _path;
	std::string _redirection_url;
	std::string _redirection_type;
	std::map<std::string, std::string> _cgi;
	int _is_get;
	int _is_post;
	int _is_delete;
	int _directory_listing;

private:
	std::map<std::string, void (Location::*)(Tokenizer &tok)> _tokens;
};

class FormatError : public std::exception
{
public:
	FormatError(std::string token, std::string expected_format);
};

class ConfigurationError : public std::exception
{
public:
	ConfigurationError(std::string message);
};

class Server
{
public:
	Server();
	void setIp(Tokenizer &tok);
	void setServerName(Tokenizer &tok);
	void setPort(Tokenizer &tok);
	void setMaxBody(Tokenizer &tok);
	void setError(Tokenizer &tok);
	void setDefaultServer();
	void (Server::*selectSetter(std::string const &token))(Tokenizer &tok);
	void addLocation(Location location);
	void setDefault();
	bool getDefault() const;
	const std::string *getServerName() const;
	unsigned int getPort() const;
	uint32_t getIp() const;
	size_t getMaxBodySize() const;
	const std::string *getErrorPages(unsigned int error) const;
	const Location *getLocation(std::string const &path) const;
	bool checkServer();
	std::string _server_name;
	unsigned int _port;
	uint32_t _ip;

private:
	bool _default;
	size_t _max_body_size;
	std::map<unsigned int, std::string> _error_pages;
	std::vector<Location> _locations;
	std::map<std::string, void (Server::*)(Tokenizer &tok)> _tokens;
};

// this class serv to store and use Server and Location object

class Config
{
public:
	Config();
	friend class Tokenizer;
	Config(const std::string &path);
	bool selectServ(const uint32_t &ip, const uint16_t &port, const std::string &host);
	const std::string *getFile(const std::string &path);
	const std::string *getPath(const std::string &path);
	Methods getMethod(const std::string &path) const;
	const std::string *getError(const unsigned int error) const;
	const std::string *getCgi(const std::string &path, const std::string &cgi) const;
	bool getUpload(const std::string &path) const;
	const std::string getRedirectionUrl(const std::string &path);
	const std::string getRedirectionType(const std::string &path);
	const std::vector<Server> &getServerTab() const;
	std::string getDirectoryListing(const std::string &path) const;
	unsigned int getMaxSize() const;
	void addServer(Server server);
	const std::string *getName() const;
	std::string getIp() const;
	std::string getPort() const;
	Config &operator=(Config const &other);
	std::vector<Server> _server;
	unsigned int getServNumb() const;
	void selectFirstServ();
	void operator++();
	void operator--();

private:
	const Location *getLocation(std::string const &path) const;
	Server *_server_selected;
	unsigned int _pos_server;
	std::string _buffer;
};

class Tokenizer
{
public:
	Tokenizer(Config &config, std::string const &path);
	const std::string &getToken();
	const std::string &nextToken();
	const std::string &getTokenBefore();
	const char &getTokenBack();

private:
	std::string _token_before;
	std::string _token;
	std::string _str;
	std::ifstream _file;
	Server parsServer();
	Location parsLocation();
	class Unexpected : public std::exception
	{
	public:
		Unexpected(std::string token);
	};
};

#endif
