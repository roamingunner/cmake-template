// File: socket-echo-server.cpp 
// Desc: Simple socket server with a single thread. 

#include <iostream>
#include <string> 
#include <vector>
#include <cassert> 
#include <cstring>  // memcpy 

// ----- Unix/Linux headers -----// 
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>    
#include <sys/socket.h>
#include <netdb.h>

// -------------  MAIN() ------------------------------//

int main(int argc, char** argv)
{
	std::puts(" [INFO] Program started. ");

	// The port should be a 16 bits number (Note: binding to some low port numbers 
	// requires administrative/root privilege.)
	std::uint16_t port = 8095;

	// The address can be: 
	//   '0.0.0.0'   for listening all hosts 
	//   '127.0.0.1' for local host 
	const char* hostname = "0.0.0.0";

	// --- Create socket file descriptor -----------------------//

	int sockfd = socket (  AF_INET       // domain:   IPv4
						 , SOCK_STREAM   // type:     TCP 
						 , 0             // protocol: (value 0) 
						);
	// Returns (-1) on failure 
	assert( sockfd != - 1);

	// ---------- query address ------------------------------//

	// Note: the keyword 'struct' is not necessary here (redundant). 
	struct hostent* h = gethostbyname(hostname);
	assert(h != nullptr);

	struct sockaddr_in sa; 
	// memset(&sa, 0x00, sizeof(sa));
	// set address 
	memcpy(&sa.sin_addr.s_addr, h->h_addr, h->h_length);

	//bcopy( h->h_addr, &sa.sin_addr.s_addr, h->h_length );

	sa.sin_family = AF_INET;
	// The function htons convert a number to big-endian format. 
	sa.sin_port   = htons(port); 

	// ----- Bind to Port and wait for client connections ---------//

	if( ::bind(sockfd, (sockaddr *) &sa, sizeof(sa)) == -1)
	{
		fprintf(stderr, "Error: unable to bind socket \n");
		return EXIT_FAILURE;
	}

	// Enables binding to the same address 
	int enable_reuseaddr = 1;
	if ( setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &enable_reuseaddr, sizeof(int)) < 0 )
	{
		fprintf(stderr, "Error: unable to set socket option. \n");
		return EXIT_FAILURE;
	}


	fprintf(stderr, " [TRACE] Listening client connection \n");

	int backlog = 5;
	assert( listen(sockfd, backlog) != -1 );

	// --------------- Server Main Loop --------------------------//

	// Infinite loop where client connections are handled
	while(true)
	{	
		/* [[ ============== BEGIN client loop =========== ]] */

		struct sockaddr_in client_sa; 
		socklen_t addrlen = sizeof(sockaddr_in); 

		fprintf(stderr, " [TRACE] Waiting for client connection. \n");
		int sock_client = accept(sockfd, (sockaddr *) &client_sa, &addrlen );
		
		if(sock_client == -1)
		{
			fprintf(stderr, " Error: failure to handle client socket. Check errno \n");
			close(sock_client);
			continue;
		}
	 	const char* welcome_msg = "\n => [INFO] Hello world client side!! \n";		
		
		// Send buffer content to client socket 
		send(sock_client, welcome_msg, strlen(welcome_msg), 0);

		char buffer[300];

		while(true){
			// Read message from client socket 
			ssize_t n = recv(sock_client, &buffer, 300, 0);
			if(n == 0){ break; }

			std::fprintf(stdout, " [INFO] Received: ");			
			::write(STDOUT_FILENO, buffer, n);

			// Send content back 
			send(sock_client, buffer, n, 0);

			// Compare 4 first bytes of buffer and 'quit' 
			if( strncmp(buffer, "quit", 4) == 0)
			{
				fprintf(stderr, " [TRACE] Shutdown connection. Ok. \n");
				// Close client connection 
				shutdown(sock_client, SHUT_RDWR);
				close(sock_client);
				break;
			}			
		}
	
	} /* [[ ============== END client loop =========== ]] */

	close(sockfd);

	return 0;
}
