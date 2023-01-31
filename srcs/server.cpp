/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomasson <lomasson@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/13 11:44:18 by lomasson          #+#    #+#             */
/*   Updated: 2023/01/31 10:25:00 by lomasson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include "Settings.hpp"
#include "Config.hpp"

int	ft_strlen(char *str)
{
	int i = 0;
	while(str[i])
		i++;
	return (i);
}

int main( void )
{
	Config config;
	try 
	{
	config = Config("srcs/conf.conf");
	}
	catch (std::runtime_error &e)
	{
		std::cout << e.what() << std::endl;
		return (0);
	}
	catch (std::exception &e)
	{
		return (0);
	}
	Settings		server;
	int				socket_server;
	int				timeout = 0;
	struct kevent	change;
	struct kevent	event;
	char buffer[8000] = {0};

	memset(&server, 0, sizeof(server));
	config.selectServ();
	std::cout << "file:" <<*config.getFile("/bg") << std::endl;
	int fd = open("http/index.html", O_RDWR);
	int ke = kqueue();
	try
	{
		socket_server = server.build();
		EV_SET(&change, socket_server, EVFILT_READ , EV_ADD | EV_ENABLE, 0, 0, 0);
		if (socket_server == -1 || ke == -1 || listen(socket_server, 100) == -1 || kevent(ke, &change, 1, &event, 1, NULL) == -1)
			throw Settings::badCreation();
		char test[550] = {0};
		read(fd, &test, 550);
		while(1)
		{
			
			std::string reponse_request;
			int nevents = kevent(ke, NULL, 0, &event, 1, NULL);
			printf("\n+++++++ Waiting for new connection ++++++++\n\n");
			if (nevents == -1)
				printf("KO\n");
			else if (nevents > 0)
			{
				int socket_client = accept(socket_server, (struct sockaddr *)&server.interface, (socklen_t *)&server.interface);
				read(socket_client, buffer, 8000);
				printf("%s\n", buffer);
				if (strncmp(buffer, "GET", 3) == 0)
					reponse_request = server.get(config);
				else if (strncmp(buffer, "POST", 4) == 0)
					reponse_request = server.post(config);
				// else if (strncmp(buffer, "DELETE", 6) == 0)
				// 	reponse_request = server.del(config);
				send(socket_client, reponse_request.c_str(), strlen(reponse_request.c_str()),0);
				std::cout << reponse_request << std::endl;
				printf("------------------Hello message sent-------------------\n");
				EV_SET(&event, socket_client, EVFILT_READ | EVFILT_WRITE , EV_ADD | EV_ENABLE, 0, 0, &timeout);
				if (kevent(ke, &event, 1, NULL, 0, NULL) == -1){}
				close(socket_client);
			}
		}
	}
	catch (const std::exception &e) {
		
	}
}

// int main( void )
// {
// 	// Config			config();
// 	Settings		server;
// 	int				socket_server;
// 	int				timeout = 0;
// 	struct kevent	srv;

// 	int ke = kqueue();
// 	int fd = open("http/index.html", O_RDWR);
// 	try
// 	{
// 		socket_server = server.build();
// 		EV_SET(&srv, socket_server, EVFILT_READ | EVFILT_WRITE , EV_ADD | EV_ENABLE, 0, 0, NULL);
// 		if (socket_server == -1 || ke == -1 || listen(socket_server, 10) == -1 || kevent(ke, &srv, 1, NULL, 0, NULL) == -1)
// 			throw Settings::badCreation();
// 		char test[550] = {0};
// 		read(fd, &test, 550);
// 		while(1)
// 		{
// 			// char buffer[1024] = {0};
// 			struct kevent event;

// 			printf("\n+++++++ Waiting for new connection ++++++++\n\n");
// 			int nevents = kevent(ke, NULL, 0, &event, 1, NULL);
// 			if (nevents == -1)
// 				printf("KO\n");
// 			else if (nevents > 0)
// 			{
// 				int socket_client = accept(socket_server, (struct sockaddr *)&server.interface, (socklen_t *)&server.interface);

// 				/* get request */
// 				char *brut_request = new char[5000];
// 				recv(socket_client, brut_request, 5000, 0);
// 				cout << "brut request : " << brut_request << endl;

// 				/* parse request */
// 				Request req;
// 				req.parse(brut_request);
// 				cout << "is get : " << req.method.isGet << endl;

// 				/* send response */
// 				FILE *file = fopen("/Users/jrasser/cursus/webservtmp/http/index.html", "r");
// 				if (file == NULL) {
// 					printf("Error opening file");
// 					return 1;
// 				}
// 				/* set size */
// 				unsigned int file_size = 0;
// 				fseek(file, 0, SEEK_END);
// 				file_size = ftell(file);
// 				fseek(file, 0, SEEK_SET);
// 				/* read file */
// 				char *buffer = new char[file_size];
// 				if (buffer == NULL) {
// 					printf("Error allocating memory");
// 					return 1;
// 				}
// 				int bytes_read = fread(buffer, 1, file_size, file);
// 				if (bytes_read == -1) {
// 					printf("Error reading file");
// 					return 1;
// 				}
// 				send(socket_client, buffer, file_size, 0);

// 				fclose(file);
// 				close(socket_client);

// 				// printf("%s\n", buffer);
// 				// close(socket_client);
// 				// write(socket_client, test, 550);

// 				EV_SET(&event, socket_client, EVFILT_WRITE , EV_ADD | EV_ENABLE, 0, 0, &timeout);
// 				if (kevent(ke, &event, 1, NULL, 0, NULL) == -1){}
// 			}
// 		}
// 	}
// 	catch(const exception& e)
// 	{
// 		close(fd);
// 		close(ke);
// 		cerr << e.what() << '\n';
// 	}
// }


/* PLAN
Si GET:
	recuperer le lien vers la ressource voulu
	verifier son existence
	recuperer son contenu
	Si Cgi:
		execve et dup vert fichier tmp
	generer les headers
	assembler headers et contenu
	si la taille du contenu > 2 083 caractères split
	envoyer le ou les paquets
Si POST:
	recuperer la cible
	verifier l'existence du dossier
	upload avec le nom donne
Si DELETE:
	recuperer la cible
	verifier l'existence du fichier
	supprimer la cible
*/