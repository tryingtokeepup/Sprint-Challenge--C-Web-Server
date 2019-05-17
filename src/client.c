#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "lib.h"

#define BUFSIZE 4096 // max number of bytes we can get at once

/**
 * Struct to hold all three pieces of a URL
 */
typedef struct urlinfo_t
{
  char *hostname;
  char *port;
  char *path;
} urlinfo_t;

/**
 * Tokenize the given URL into hostname, path, and port.
 *
 * url: The input URL to parse.
 *
 * Store hostname, path, and port in a urlinfo_t struct and return the struct.
*/
urlinfo_t *parse_url(char *url)
{
  // copy the input URL so as not to mutate the original
  char *hostname = strdup(url);
  char *port;
  char *path;

  urlinfo_t *urlinfo = malloc(sizeof(urlinfo_t));

  /*
    We can parse the input URL by doing the following:

    1. Use strchr to find the first slash in the URL (this is assuming there is no http:// or https:// in the URL).
    2. Set the path pointer to 1 character after the spot returned by strchr.
    3. Overwrite the slash with a '\0' so that we are no longer considering anything after the slash.
    4. Use strchr to find the first colon in the URL.
    5. Set the port pointer to 1 character after the spot returned by strchr.
    6. Overwrite the colon with a '\0' so that we are just left with the hostname.
  */

  ///////////////////
  // IMPLEMENT ME! //
  ///////////////////
  // Okay, here goes nothing. First, let's use strchr to find that first slash.
  // I might need some sort of pointer, don't want to use the pointers given. Might need them later.

  char *temp_path_parser;
  temp_path_parser = strstr(hostname, "/");

  // need to check for NULL condition:
  if (temp_path_parser != NULL)
  {

    path = temp_path_parser + 1;
    // Overwrite the slash with a '\0' so that we are no longer considering anything after the slash.
    *(temp_path_parser) = '\0';
  }
  temp_path_parser = strstr(hostname, ":");
  if (temp_path_parser != NULL)
  {

    port = temp_path_parser + 1;
    // Overwrite the slash with a '\0' so that we are no longer considering anything after the slash.
    *(temp_path_parser) = '\0';
    //printf(temp_path_parser);
  }
  // copy over everything and return the modified urlinfo
  urlinfo->hostname = hostname;
  urlinfo->port = port;
  urlinfo->path = path;

  return urlinfo;
}

/**
 * Constructs and sends an HTTP request
 *
 * fd:       The file descriptor of the connection.
 * hostname: The hostname string.
 * port:     The port string.
 * path:     The path string.
 *
 * Return the value from the send() function.
*/
int send_request(int fd, char *hostname, char *port, char *path)
{
  const int max_request_size = 16384;
  char request[max_request_size];
  int rv;
  // calculate the request_length by writing up all that stuff and sprintf it
  // first thingy will be the path, second is the hostname, third is the path. i think this is how to do it.
  //GET /path HTTP/1.1
  //Host: hostname:port
  //Connection: close
  // guess we have to keep the connection closed
  int request_length = sprintf(request, "GET /%s HTTP/1.1\n"
                                        "Host: %s:%s\n"
                                        "Connection: Close\n",
                               path, hostname, port);

  // sen
  rv = send(fd, request, request_length, 0);

  if (rv < 0)
  {
    perror("send");
  }

  return rv;
}

int main(int argc, char *argv[])
{
  int sockfd, numbytes;
  char buf[BUFSIZE];

  if (argc != 2)
  {
    fprintf(stderr, "usage: client HOSTNAME:PORT/PATH\n");
    exit(1);
  }

  /*
    1. Parse the input URL
    2. Initialize a socket by calling the `get_socket` function from lib.c
    3. Call `send_request` to construct the request and send it
    4. Call `recv` in a loop until there is no more data to receive from the server. Print the received response to stdout.
    5. Clean up any allocated memory and open file descriptors.
  */

  ///////////////////
  // IMPLEMENT ME! //
  ///////////////////
  // parse it, catch it - make sure to free any malloc'd memory
  urlinfo_t *urlinfo = parse_url(argv[1]);
  // init socket by calling get_socket
  sockfd = get_socket(urlinfo->hostname, urlinfo->port);

  if (sockfd == -1)
  {

    printf("Yo, it didn't work out. Sorry, retry your prompt.\n");
  }
  int status_request = send_request(sockfd, (*urlinfo).hostname, urlinfo->port, urlinfo->path);
  int byte_count;
  while ((byte_count = recv(sockfd, buf, BUFSIZE - 1, 0)) > 0)
  {
    printf("recv()'d %d bytes of data in buf\n", byte_count);
    printf("%s", buf);
  }

  //5. Clean up any allocated memory and open file descriptors.
  free(urlinfo);

  return 0;
}
