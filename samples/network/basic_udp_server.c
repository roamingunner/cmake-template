/**************************************************************************/
/* This sample program provides a code for a connectionless server.       */
/**************************************************************************/

/**************************************************************************/
/* Header files needed for this sample program                            */
/**************************************************************************/
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

/**************************************************************************/
/* Constants used by this program                                         */
/**************************************************************************/
#define SERVER_PORT     3555
#define BUFFER_LENGTH    100
#define FALSE              0

void main()
{
/***********************************************************************/
/* Variable and structure definitions.                                 */
/***********************************************************************/
int    sd=-1, rc;
char   buffer[BUFFER_LENGTH];
struct sockaddr_in6 serveraddr;
struct sockaddr_in6 clientaddr;
int    clientaddrlen = sizeof(clientaddr);

/***********************************************************************/
/* A do/while(FALSE) loop is used to make error cleanup easier.  The   */
/* close() of each of the socket descriptors is only done once at the  */
/* very end of the program.                                            */
/***********************************************************************/
do
{
	/********************************************************************/
	/* The socket() function returns a socket descriptor, representing  */
	/* an endpoint.  The statement also identifies that the INET6       */
	/* (Internet Protocol version 6) address family with the UDP        */
	/* transport (SOCK_DGRAM) will be used for this socket.             */
	/********************************************************************/
	sd = socket(AF_INET6, SOCK_DGRAM, 0);
	if (sd < 0)
	{
		perror("socket() failed");
		break;
	}

	/********************************************************************/
	/* After the socket descriptor is created, a bind() function gets a */
	/* unique name for the socket.  In this example, the user sets the  */
	/* s_addr to zero, which means that the UDP port of 3555 will be    */
	/* bound to all IP addresses on the system.                         */
	/********************************************************************/
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin6_family      = AF_INET6;
	serveraddr.sin6_port        = htons(SERVER_PORT);
	memcpy(&serveraddr.sin6_addr, &in6addr_any, sizeof(in6addr_any));

	rc = bind(sd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
	if (rc < 0)
	{
		perror("bind() failed");
		break;
	}

	/********************************************************************/
	/* The server uses the recvfrom() function to receive that data.    */
	/* The recvfrom() function waits indefinitely for data to arrive.   */
	/********************************************************************/
	rc = recvfrom(sd, buffer, sizeof(buffer), 0,
					(struct sockaddr *)&clientaddr,
					&clientaddrlen);
	if (rc < 0)
	{
		perror("recvfrom() failed");
		break;
	}

	printf("server received the following: <%s>\n", buffer);
	inet_ntop(AF_INET6, &clientaddr.sin6_addr.s6_addr,
				buffer, sizeof(buffer));
	printf("from port %d and address %s\n",
			ntohs(clientaddr.sin6_port),
			buffer);

	/********************************************************************/
	/* Echo the data back to the client                                 */
	/********************************************************************/
	rc = sendto(sd, buffer, sizeof(buffer), 0,
				(struct sockaddr *)&clientaddr,
				sizeof(clientaddr));
	if (rc < 0)
	{
		perror("sendto() failed");
		break;
	}

	/********************************************************************/
	/* Program complete                                                 */
	/********************************************************************/

} while (FALSE);

/***********************************************************************/
/* Close down any open socket descriptors                              */
/***********************************************************************/
if (sd != -1)
	close(sd);
}
