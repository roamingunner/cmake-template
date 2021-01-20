#include <iostream>
#include <cassert> // Use: assert() for marking placeholders for future error handling.
#include <string>  // std::string 
#include <cstring> // memcpy, memset and so on

// ----- Unix/Linux headers -----// 
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>    
#include <sys/socket.h>
#include <netdb.h>
#include <limits.h>

constexpr int OPERATION_FAILURE = -1;

// Read buffer from file descriptor handling partial reads.
// Note: It should only be used if the counterpart socket, sends 
// a buffer of equal to the size of this 'size' parameter.
void read_buffer(int fd, void* buffer, size_t size);

// Write buffer to file descriptor handling partial writes;
void write_buffer(int fd, const void* buffer, size_t size);

// Send text with '\n' UNIX (EOL) end line character.
void write_line(int fd, std::string const& text);

int main(int argc, char** argv)
{
    // ======= Server port and hostname configuration ===========//

     // The port should be a 16 bits number 
     // (Ports less than 1024 requires administrative/root privilege.)
     std::uint16_t port = 8095;
     // '127.0.0.1' for local host (LO Network interface)
     const char* hostname = "127.0.0.1";

     // ======= Create socket file descriptor =====================//

     int sockfd = socket (  AF_INET       // domain:   IPv4
                          , SOCK_STREAM   // type:     TCP 
                          , 0             // protocol: (value 0) 
                         );
    // Returns (-1) on failure      
    assert( sockfd != OPERATION_FAILURE);    

    // =========== Query Address (DNS query) =======================//

     // Note: the keyword 'struct' is not necessary here (redundant). 
     struct hostent* h = gethostbyname(hostname);
     assert(h != nullptr);

     struct sockaddr_in sa; 
     // memset(&sa, 0x00, sizeof(sa));
     // set address 
    std::memcpy(&sa.sin_addr.s_addr, h->h_addr, h->h_length);

     //bcopy( h->h_addr, &sa.sin_addr.s_addr, h->h_length );

     sa.sin_family = AF_INET;
     // The function htons convert a number to big-endian format. 
     sa.sin_port   = htons(port); 

    // ======== Attempt connection to server ========================//
    
    int conn_result = connect( // Socket file descriptor 
                               sockfd
                               // Pointer sockaddr => Structure containing host address 
                             , reinterpret_cast<sockaddr*>(&sa)
                               // Size of sockaddr buffer in bytes 
                             , sizeof(sockaddr_in));

    if( conn_result == OPERATION_FAILURE )
    {
        std::fprintf(stderr, " [ERROR] Unable to connect to server. \n");
        exit(EXIT_FAILURE);
    }

    //============= Session Loop ===================================//

    // --- Send a buffer (untyped memory location) ----------// 
    char start_message[500];
    std::memset(start_message, '\0', 500);
    std::strcpy(start_message, " [CLIENT] <MESSAGE 1> Hi there, server, I am the client socket \n");    
    write_buffer( sockfd, start_message, strlen(start_message) );

    // --- Send text --------------------------------------//
    write_line(sockfd, " [CLIENT] <MESSAGE 2> Hi there, server, I was sent by the client-side \n");

    constexpr size_t BUFFER_SIZE = 500;
    char buffer[BUFFER_SIZE];

    // ---------- Session Loop ------------------------------//
    for(;;)
    {        
        // Clean buffer before each iteratiion 
        std::memset(buffer, 0x00, BUFFER_SIZE);

        std::fprintf(stdout, " [TRACE] Waiting for server input. " 
                             " =>> Type something in the server netcat terminal and hit RETURN. \n");

        ssize_t r = read(sockfd, buffer, BUFFER_SIZE);
        if(r == 0){
            std::fprintf(stdout, " [TRACE] Server has closed the connection. " 
                               "User types CTRL + D in the server-side terminal. \n");
            break;
        } 
        if( r == -1){
            std::fprintf(stdout, " [TRACE] An error has happend Check ERRNO. ");
            break;
        } 

        std::fprintf(stdout, " [SERVER DATA] Server has sent: %s \n", buffer);
        // Send data back to server 
        write_buffer(sockfd, buffer, BUFFER_SIZE);

    } // --- End of session loop ---------------------------//

    std::fprintf(stderr, " [INFO] Session shutdown OK \n");

    close(sockfd);
    return 0;
}

    // ------------------------------------------------------//
    //          I M P L E M E N T A T I O N S                //
    //-------------------------------------------------------//

void read_buffer(int fd, void* buffer, size_t size)
{
   // Pre-condition 
   assert( size <= SSIZE_MAX  );

   size_t  len  = size;
   ssize_t nr   = -1;
   // Note: std::byte (C++17) is a better replacement for char* or std::uint8_t 
   // as std::byte is more effective for conveying the intent.
   auto pbuf = reinterpret_cast<std::byte*>(buffer); 

   std::fprintf(stderr, " [TRACE] Waiting input \n" );

   while(  len != 0 && ( nr = read(fd, pbuf, len) ) != 0 ) 
   {
       
       if(nr == -1 && errno == EINTR){ continue; } 
       if(nr == -1) { 
           throw std::runtime_error("An error has happened, check ERRNO ");
           break;
       }
       len  = len - nr; 
       pbuf = pbuf + nr;

       std::cerr << " [TRACE] len = " << len << " ; nr = " << nr << '\n';
   }

   std::fprintf(stderr, " [TRACE] Loop finished. \n" );
}

void write_buffer(int fd, const void* buffer, size_t size)
{
   // Pre-condition 
   assert( size <= SSIZE_MAX  );
   size_t  len  = size;
   ssize_t nr   = -1;
   auto pbuf = reinterpret_cast<const std::uint8_t*>(buffer); 

  while(  len != 0 && ( nr = write(fd, pbuf, len) ) != 0 ) 
  {
      if(nr == -1 && errno == EINTR){ continue; } 
      if(nr == -1) { 
          throw std::runtime_error("An error has happened, check ERRNO ");
          break;
      }
      len  = len - nr; 
      pbuf = pbuf + nr;
   }
}

void write_line(int fd, std::string const& text)
{
    write_buffer(fd, text.data(), text.size());
    char ch = '\n';
    write(fd, &ch, 1);
}