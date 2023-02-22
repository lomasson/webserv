/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Settings.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydumaine <ydumaine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/20 11:11:03 by lomasson          #+#    #+#             */
/*   Updated: 2023/02/22 19:13:27 by ydumaine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Settings.hpp"
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

Settings::Settings(Config const& base) : config(base)
{
	this->ext[".aac"] = "audio/aac";
	this->ext[".avi "] = "video/x-msvideo";
	this->ext[".bin"] = "application/octet-stream";
	this->ext[".bmp"] = "image/bmp";
	this->ext[".bz"] = "application/x-bzip";
	this->ext[".bz2"] = "application/x-bzip2";
	this->ext[".csh"] = "application/x-csh";
	this->ext[".css"] = "text/css";
	this->ext[".csv"] = "text/csv";
	this->ext[".doc"] = "application/msword";
	this->ext[".docx "] = "application/vnd.openxmlformats-officedocument.wordprocessingml.document";
	this->ext[".gif"] = "image/gif";
	this->ext[".htm"] = "text/html";
	this->ext[".html"] = "text/html";
	this->ext[".ico"] = "image/x-icon";
	this->ext[".jar"] = "application/java-archive";
	this->ext[".jpeg"] = "image/jpeg";
	this->ext[".jpg"] = "image/jpeg";
	this->ext[".js"] = "application/javascript";
	this->ext[".json"] = "application/json";
	this->ext[".mid"] = "audio/midi";
	this->ext[".midi"] = "audio/midi";
	this->ext[".mpeg"] = "video/mpeg";
	this->ext[".mp3"] = "audio/mpeg";
	this->ext[".mp4"] = "video/mp4";
	this->ext[".odp "] = "application/vnd.oasis.opendocument.presentation";
	this->ext[".ods"] = "application/vnd.oasis.opendocument.spreadsheet";
	this->ext[".odt"] = "application/vnd.oasis.opendocument.text";
	this->ext[".oga"] = "audio/ogg";
	this->ext[".ogv"] = "video/ogg";
	this->ext[".ogx"] = "application/ogg";
	this->ext[".otf"] = "font/otf";
	this->ext[".png"] = "image/png";
	this->ext[".pdf"] = "application/pdf";
	this->ext[".ppt"] = "application/vnd.ms-powerpoint";
	this->ext[".pptx"] = "application/vnd.openxmlformats-officedocument.presentationml.presentation";
	this->ext[".rar"] = "application/x-rar-compressed";
	this->ext[".rtf"] = "application/rtf";
	this->ext[".sh"] = "application/x-sh";
	this->ext[".svg"] = "image/svg+xml";
	this->ext[".swf"] = "application/x-shockwave-flash";
	this->ext[".tar"] = "application/x-tar";
	this->ext[".tif"] = "image/tiff";
	this->ext[".tiff"] = "image/tiff";
	this->ext[".ts"] = "application/typescript";
	this->ext[".ttf"] = "font/ttf";
	this->ext[".txt"] = "text/plain";
	this->ext[".vsd"] = "application/vnd.visio";
	this->ext[".wav"] = "audio/x-wav";
	this->ext[".weba"] = "audio/webm";
	this->ext[".webm"] = "video/webm";
	this->ext[".webp "] = "image/webp";
	this->ext[".woff"] = "font/woff";
	this->ext[".woff2"] = "font/woff2";
	this->ext[".xhtml"] = "application/xhtml+xml";
	this->ext[".xls"] = "application/vnd.ms-excel";
	this->ext[".xlsx"] = "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
	this->ext[".xml"] = "application/xml";
	this->ext[".xul"] = "application/vnd.mozilla.xul+xml";
	this->ext[".zip"] = "application/zip";
	this->ext[".3gp"] = "video/3gpp";
	this->ext[".3g2"] = "video/3gpp2";
	this->ext[".7z"] = "	application/x-7z-compressed";
}

Settings::Settings()
{
	this->ext[".aac"] = "audio/aac";
	this->ext[".avi "] = "video/x-msvideo";
	this->ext[".bin"] = "application/octet-stream";
	this->ext[".bmp"] = "image/bmp";
	this->ext[".bz"] = "application/x-bzip";
	this->ext[".bz2"] = "application/x-bzip2";
	this->ext[".csh"] = "application/x-csh";
	this->ext[".css"] = "text/css";
	this->ext[".csv"] = "text/csv";
	this->ext[".doc"] = "application/msword";
	this->ext[".docx "] = "application/vnd.openxmlformats-officedocument.wordprocessingml.document";
	this->ext[".gif"] = "image/gif";
	this->ext[".htm"] = "text/html";
	this->ext[".html"] = "text/html";
	this->ext[".ico"] = "image/x-icon";
	this->ext[".jar"] = "application/java-archive";
	this->ext[".jpeg"] = "image/jpeg";
	this->ext[".jpg"] = "image/jpeg";
	this->ext[".js"] = "application/javascript";
	this->ext[".json"] = "application/json";
	this->ext[".mid"] = "audio/midi";
	this->ext[".midi"] = "audio/midi";
	this->ext[".mpeg"] = "video/mpeg";
	this->ext[".mp3"] = "audio/mpeg";
	this->ext[".mp4"] = "video/mp4";
	this->ext[".odp "] = "application/vnd.oasis.opendocument.presentation";
	this->ext[".ods"] = "application/vnd.oasis.opendocument.spreadsheet";
	this->ext[".odt"] = "application/vnd.oasis.opendocument.text";
	this->ext[".oga"] = "audio/ogg";
	this->ext[".ogv"] = "video/ogg";
	this->ext[".ogx"] = "application/ogg";
	this->ext[".otf"] = "font/otf";
	this->ext[".png"] = "image/png";
	this->ext[".pdf"] = "application/pdf";
	this->ext[".ppt"] = "application/vnd.ms-powerpoint";
	this->ext[".pptx"] = "application/vnd.openxmlformats-officedocument.presentationml.presentation";
	this->ext[".rar"] = "application/x-rar-compressed";
	this->ext[".rtf"] = "application/rtf";
	this->ext[".sh"] = "application/x-sh";
	this->ext[".svg"] = "image/svg+xml";
	this->ext[".swf"] = "application/x-shockwave-flash";
	this->ext[".tar"] = "application/x-tar";
	this->ext[".tif"] = "image/tiff";
	this->ext[".tiff"] = "image/tiff";
	this->ext[".ts"] = "application/typescript";
	this->ext[".ttf"] = "font/ttf";
	this->ext[".txt"] = "text/plain";
	this->ext[".vsd"] = "application/vnd.visio";
	this->ext[".wav"] = "audio/x-wav";
	this->ext[".weba"] = "audio/webm";
	this->ext[".webm"] = "video/webm";
	this->ext[".webp "] = "image/webp";
	this->ext[".woff"] = "font/woff";
	this->ext[".woff2"] = "font/woff2";
	this->ext[".xhtml"] = "application/xhtml+xml";
	this->ext[".xls"] = "application/vnd.ms-excel";
	this->ext[".xlsx"] = "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
	this->ext[".xml"] = "application/xml";
	this->ext[".xul"] = "application/vnd.mozilla.xul+xml";
	this->ext[".zip"] = "application/zip";
	this->ext[".3gp"] = "video/3gpp";
	this->ext[".3g2"] = "video/3gpp2";
	this->ext[".7z"] = "	application/x-7z-compressed";
}



Settings::~Settings()
{
}



void	Settings::build(int ke)
{
	static struct kevent	change;
	int						socket_fd;
	struct addrinfo			serv_addr, *res;
	std::vector<std::string>	all_ports;

	this->config.selectFirstServ();
	for(unsigned int i = 0; i < this->config.getServNumb(); i++)
	{
		if (std::find(all_ports.begin(), all_ports.end(), this->config.getPort()) == all_ports.end())
		{
			std::memset(&serv_addr , 0, sizeof(serv_addr));
			serv_addr.ai_family = AF_INET;
			serv_addr.ai_socktype = SOCK_STREAM;
			int status = getaddrinfo(this->config.getIp().c_str(), this->config.getPort().c_str(), &serv_addr, &res);
			if (status != 0)
				throw Settings::badCreation();
			socket_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
			if (socket_fd == -1)
				throw Settings::badCreation();
			int r = 1;
			if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &r, sizeof(r)) < 0)
				throw Settings::badCreation();
  			fcntl(socket_fd, F_SETFL, fcntl(socket_fd, F_GETFL, 0) | O_NONBLOCK);
			int bind_status = bind(socket_fd, res->ai_addr, res->ai_addrlen);
			if (bind_status == -1)
				throw Settings::badCreation();
			freeaddrinfo(res);
			if (listen(socket_fd, 1024) == -1)
				throw Settings::badCreation();
			EV_SET(&change, socket_fd, EVFILT_READ , EV_ADD | EV_ENABLE, 0, 0, &serv_addr);
			if (kevent(ke, &change, 1, NULL, 0, NULL) == -1)
					throw Settings::badCreation();
			all_ports.push_back(this->config.getPort());
		}
		++config;
	}
}



std::string Settings::date(void)
{
	time_t				tmm = time(0);
	std::string			rdate, s, tmp;
	std::stringstream	str(asctime(localtime(&tmm)));

	getline(str, rdate, ' ');
	getline(str, tmp, ' ');
	getline(str, s, ' ');
	rdate = "date: " + rdate + " , " + s + " " + tmp;
	getline(str, tmp, ' ');
	getline(str, s, '\n');
	rdate += " " + s + " " + tmp + " GMT";
	return (rdate);
}



std::string Settings::get(Request const &req)
{
	std::string			buffer = "";
	std::string			reponse = "HTTP/1.1";
	std::stringstream	n;
	std::fstream		fd;
	std::string			tmp;
	int					value = 0;

	if (!this->config.getMethod(req.method.path).isget)
		return (this->method_not_allowed(req));
	if (value == 10)
	{
		std::cout << "2\n";
		fd.open("http/401.html", std::fstream::in | std::fstream::out);
		std::stringstream t;
		t << fd.rdbuf();
		fd.close();
		return(t.str());
	}
	if (this->config.getFile(req.method.path)->empty())
	{
		reponse.append(" 404 Not Found\n");
		fd.open(this->config.getError(404)->c_str());
		if (!fd.is_open())
			fd.open("http/404.html");
	}
	else
	{
		char *file = (char *)this->config.getFile(req.method.path)->c_str();
		if(file[0] == '/')
			file++;
		fd.open(file, std::fstream::in | std::fstream::out);
		if (fd.is_open())
			reponse.append(" 200 OK\n");
		else if (!this->checkextension(req.method.path).empty())
		{
			reponse.append(" 404 Not Found\n");
			fd.open("http/404.html");
		}
		else if (!this->config.getDirectoryListing(req.method.path).empty())
			return (folder_gestion(req));
	}
	while (getline(fd, tmp))
		buffer += tmp + "\n";
	reponse += Settings::date() + "\n";
	reponse += "server: " + *this->config.getName() + "\n";
	n << buffer.size();
	reponse += "Content-Length: " + n.str() + "\n";
	reponse += "Content-Type: " + this->checkextension(*this->config.getFile(req.method.path)) + "\n";
	reponse += "Connection: keep-alive\n";
	reponse += "\n" + buffer;
	fd.close();
	return (reponse);
}



std::string Settings::post(Request const &req)
{
	std::stringstream reponse;
	std::string rvalue_script;
	std::fstream fd;

	reponse << "HTTP/1.1";
	if (!this->config.getMethod(req.method.path).ispost)
		return (this->method_not_allowed(req));
	fd.open(this->config.getFile(req.method.path)->c_str(), std::fstream::in);
	if (!fd.is_open())
	{
		fd.open(this->config.getError(404)->c_str(), O_RDONLY);
		if (!fd.is_open())
			fd.open("http/404.html", O_RDONLY);
		reponse << " 404 Not Found\n";
	}
	// std::cout << "Executing CGI..." << std::endl;
	if (req.body.content.size() > 30)
		rvalue_script = CGI::execute_cgi(this->config, req);
	if (rvalue_script.size() == 0)
		reponse << " 204 No Content\n";
	else if (strcmp(rvalue_script.c_str(), "Status: 500") == 0)
		reponse << "500 Internal Server Error\n";
	// std::cout << "Executing CGI end" << std::endl;
	reponse << Settings::date() << "\n";
	reponse << "server: " << *this->config.getName() << "\n";
	reponse << "Content-Length: " << rvalue_script.size();
	reponse << "\nContent-Type: " << this->checkextension(req.method.path) << "\n";
	reponse << "Connection: keep-alive\n\n";
	reponse << rvalue_script;

	std::string::size_type pos = 0;
	if ((pos = rvalue_script.find("content_length")) != std::string::npos)
		reponse << EOF;
	fd.close();
	return (reponse.str());
}



std::string Settings::reading(int socket, Request req)
{
	std::stringstream 	sbuffer;
	int					o_read = 0;
	std::memset(&req.buffer, 0, sizeof(req.buffer));
	usleep(1000);
	o_read = recv(socket, req.buffer, 35000, 0);
	if (o_read == -1 || o_read == 0)
		return (std::string());
	sbuffer << req.buffer;
	// if (o_read == 8192) {
	// 	while (o_read == 8192) {
	// 		req.resetBuffer();
	// 		// o_read = recv(socket, req.buffer, REQ_MAX_SIZE, 0);
	// 		o_read = recv(socket, req.buffer, 8192, 0);
	// 		sbuffer << req.buffer;
	// 	}
	// }
	std::cout << "\n\nREAD:\n>>" << sbuffer.str() << "<<\n\n";

	// req.splitRequest(buffer);
	// req.header.parseHeader();
	// if (!this->config.selectServ(req.header.host_ip, req.header.port))
	// 	reponse_request = this->badRequest(req);


	return (sbuffer.str());
}



void Settings::writing(int socket, Request & req, std::string sbuffer)
{
	std::string reponse_request;
	int valid = -1;

	std::fstream fd;
	if (!req.method.path.empty())
		fd.open(*this->config.getFile(req.method.path));
	// std::cout << "request:\n" << sbuffer << std::endl;
	if (req.parseRequest(sbuffer))
		reponse_request = this->method_not_allowed(req);
	if (!this->config.selectServ(req.header.host_ip, req.header.port))
		reponse_request = this->badRequest(req);
	if (!sbuffer.empty())
		valid = this->checkmethod(sbuffer, this->config.getMethod(req.method.path));
	if (valid == -2)
		reponse_request = this->method_not_allowed(req);
	else if (valid == -1)
		reponse_request = this->method_not_allowed(req);
	// else if (req.method.brut_method == "GET /directory/Yeah HTTP/1.1") // A mettre a la main dans le debugger pour passer le GET sur /directory/Yeah je sais pas pk ca doit aller en 404
	// {
	// 		std::fstream fd;
	// 		fd.open("http/401.html", std::fstream::in | std::fstream::out);
	// 		std::stringstream t;
	// 		t << fd.rdbuf();
	// 		fd.close();
	// 		reponse_request = t.str();
	// }
	else if (check_forbidden(*this->config.getFile(req.method.path)) && checkextension(*this->config.getFile(req.method.path)).empty())
		reponse_request = this->not_found();
	else if (req.method.isGet)
		reponse_request = this->get(req);
	else if (req.method.isPost || req.method.isDelete)
		reponse_request = this->post(req);
	else
		reponse_request = this->badRequest(req);
	std::cout << std::endl << std::endl << "WRITE : "  << std::endl << reponse_request << std::endl;
	write(socket, reponse_request.c_str(), reponse_request.size());
	//send(socket, reponse_request.c_str(), reponse_request.size(), 0);
}

std::string	Settings::checkextension(std::string const& path)
{
	size_t pos = path.rfind('.');
	
	if (pos != path.npos)
	{
		std::map<std::string, std::string>::iterator start = this->ext.begin();
		std::map<std::string, std::string>::iterator end = this->ext.end();
		std::string extension = path.substr(pos);
		while(start != end)
		{
			if (start->first == extension)
				return(start->second);
			start++;
		}
		return(std::string(""));
	}
	return (std::string());
}



std::string Settings::folder_gestion(Request const& req)
{
	std::stringstream	reponse;
	std::stringstream	buffer;

	reponse << "HTTP/1.1 ";
	DIR *dir = opendir(this->config.getDirectoryListing(req.method.path).c_str());
	if (dir)
	{
		reponse << "200 OK\r\n";
		buffer << "<html><body><ul>";
		struct dirent *entry;
		while ((entry = readdir(dir)) != nullptr)
			if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
				buffer << "<li>" + std::string(entry->d_name) << "</li>\n";
		buffer << "</ul></body></html>\r\n";
		closedir(dir);
	}
	else
	{
		std::fstream fd;
		if (this->config.getError(404))
			fd.open(this->config.getError(404)->c_str());
		if (!fd.is_open())
			fd.open("http/404.html");
		reponse << "404 Not Found\n";
		buffer << fd.rdbuf();
		
	}
	reponse << Settings::date() << "\n";
	reponse << "server: " << *this->config.getName() << "\n";
	reponse << "Content-Length: ";
	reponse << buffer.str().size();
	reponse << "\nContent-Type: text/html\n";
	reponse << "Connection: keep-alive\n\n";
	reponse << buffer;
	return (reponse.str());
}



int Settings::checkmethod(std::string const& request, Methods const& t)
{
	std::string	method;
	
	for(int i = 0; request.c_str()[i] != ' ' && request.c_str()[i] ; i++)
		method += request.c_str()[i];
	if (method == "PUT" || method == "HEAD" || method == "OPTIONS" || method == "TRACE" || method == "CONNECT")
		return (-1);
	else if (method == "POST" && !t.ispost)
		return (-2);
	else if (method == "GET" && !t.isget)
		return (-2);
	else if (method == "DELETE" && !t.isdelete)
		return (-2);
	else
		return (0);
}


void	Settings::set_event(int ke, int socket, short filter, short flag)
{
	struct kevent changeEvent;
	EV_SET(&changeEvent, socket, filter, flag, 0, 0, nullptr);
	if (kevent(ke, &changeEvent, 1, nullptr, 0, nullptr) == -1)
		std::cerr << "Could not add client " << socket <<" socket to kqueue" << std::endl;
}



/********************************************/
/*											*/
/*				ERROR FONC					*/
/*											*/
/********************************************/




int Settings::check_forbidden(std::string const& path)
{
	struct stat info;
	
	// std::fstream fd;
	int i = 0;
	if (path.c_str()[0] == '/')
		i = 1;
	// fd.open(path.c_str() + i);
	// if (fd.is_open())
	if (stat(path.c_str() + i, &info) != 0)
		return(1);
	if (S_ISDIR(info.st_mode))
		return(1);
	else
		return(0);
}


std::string Settings::not_found( void )
{
	std::fstream		file("http/404.html");
	std::stringstream	contentefile;
	std::stringstream	reponse;
	
	if (file.is_open())
		contentefile << file.rdbuf();
	reponse << "HTTP/1.1 404 Not Found\n";
	reponse << this->date() << "\n";
	reponse << "server: " << *this->config.getName() << "\n";
	reponse << "Content-Type: text/html\n";
	if (file.is_open())
	{
		reponse << "Content-Length: " << contentefile.str().size() << "\n\n";
		reponse << contentefile.str() << "\n";
	}
	else
		reponse << "Content-Length: 0\n\n"; 
	return (reponse.str());
}



std::string Settings::Unauthorized( void )
{
	std::fstream		file("http/401.html");
	std::stringstream	contentefile;
	std::stringstream	reponse;
	
	if (file.is_open())
		contentefile << file.rdbuf();
	reponse << "HTTP/1.1 401 Unauthorized\n";
	reponse << this->date() << "\n";
	reponse << "server: " << *this->config.getName() << "\n";
	reponse << "Content-Type: text/html\n";
	if (file.is_open())
	{
		reponse << "Content-Length: " << contentefile.str().size() << "\n\n";
		reponse << contentefile.str() << "\n";
	}
	else
		reponse << "Content-Length: 0\n\n"; 
	return (reponse.str());
}



std::string Settings::method_not_allowed(Request const& req)
{
	std::stringstream reponse;
	
	reponse << "HTTP/1.1 405 Method Not Allowed\n";
	reponse << "Allow:";
	if (this->config.getMethod(req.method.path).ispost)
		reponse << " POST";
	if (this->config.getMethod(req.method.path).isget)
		reponse << " GET";
	reponse << "\nContent-Type: text/plain\n"; 

	reponse << Settings::date() << "\n";
	reponse << "server: " << *this->config.getName() + "\n";
	reponse << "Content-Length: 25\n";
	reponse << "Connection: keep-alive\n";
	reponse << "\nMethod Not Allowed (405)\n";
	return (reponse.str());
}



std::string Settings::forbidden_error( void )
{
	std::fstream		file("http/403.html");
	std::stringstream	contentefile;
	std::stringstream	reponse;
	
	if (file.is_open())
		contentefile << file.rdbuf();
	reponse << "HTTP/1.1 403 Forbidden\n";
	reponse << this->date() << "\n";
	reponse << "server: " << *this->config.getName() << "\n";
	reponse << "Content-Type: text/html\n";
	if (file.is_open())
	{
		reponse << "Content-Length: " << contentefile.str().size() << "\n\n";
		reponse << contentefile.str() << "\n";
	}
	else
		reponse << "Content-Length: 0\n\n"; 
	return (reponse.str());
}



std::string Settings::badRequest(Request const& req)
{
	std::stringstream reponse;
	
	reponse << "HTTP/1.1 400 Bad Request\n";
	reponse << Settings::date() << "\n";
	reponse << "server: " << *this->config.getName() + "\n";
	reponse << "server: " << "myserv" << "\n";
	reponse << "Content-Length: 21\n";
	reponse << "Connection: keep-alive\n";
	reponse << "\nMalformed ";
	if (this->config.getMethod(req.method.path).isget)
		reponse << "GET";
	else if (this->config.getMethod(req.method.path).ispost)
		reponse << "POST";
	reponse << " request\n";
	return (reponse.str());
}


/*
POST / HTTP/1.1
Host: 127.0.0.1:4241
User-Agent: Go-http-client/1.1\
Transfer-Encoding: chunked
Content-Type: test/file
Accept-Encoding: gzip

0

HEAD / HTTP/1.1
Host: 127.0.0.1:4241
User-Agent: Go-http-client/1.1


*/