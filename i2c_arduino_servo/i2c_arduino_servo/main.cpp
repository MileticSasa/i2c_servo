#include <wiringPi.h>
#include <string.h>
#include <iostream>
#include <wiringPiI2C.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdint.h>

#define	SLAVE_ADD	0x68	//address of my arduino

int sockfd, newsockfd;
int portno = 40300;
int buffer[1024];
struct sockaddr_in6 serv_addr, cli_addr;
socklen_t size;
int n;


int main(void)
{
	wiringPiSetupSys();

	//Connection with Arduino
	int fd = wiringPiI2CSetup(SLAVE_ADD);
	if (fd == -1)
	{
		std::cout << "Connecting failed..." << std::endl;
		return -1;
	}

	std::cout << "Connected with arduino!" << std::endl;

	//Connection with mobile phone
	sockfd = socket(AF_INET6, SOCK_STREAM, 0);
	if (sockfd < 0)
	{
		std::cout << "Error..." << std::endl;
		exit(1);
	}

	std::cout << "Connection created." << std::endl;

	bzero((sockaddr*)&serv_addr, sizeof(serv_addr));

	serv_addr.sin6_family = AF_INET6;
	serv_addr.sin6_addr = in6addr_any;
	serv_addr.sin6_port = htons(portno);

	int one = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));

	if (bind(sockfd, (sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
	{
		std::cout << "Error binding socket..." << std::endl;
		exit(1);
	}

	size = sizeof(cli_addr);
	std::cout << "Looking for clients..." << std::endl;

	listen(sockfd, 1);

	do 
	{
		newsockfd = accept(sockfd, (struct sockaddr*)&cli_addr, &size);

		if (newsockfd < 0)
		{
			std::cout << "Error on accepting" << std::endl;
			exit(1);
		}
		else do
		{
			if ((n = read(newsockfd, buffer, sizeof(buffer))) < 0)
			{
				std::cout << "Error..." << std::endl;
				exit(1);
			}
			else
			{
				std::cout << "Move to position " << buffer[0] << std::endl;
				uint8_t position = (uint8_t)buffer[0];
				wiringPiI2CWrite(fd, position);
			
			}
		} while (n > 0);
	} while (true);

	close(newsockfd);
	close(sockfd);

	return 0;
}

