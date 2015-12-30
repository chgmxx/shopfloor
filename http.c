#include "http.h"

char* host_to_ip(const char* hostname)
{
  struct hostent *host_entry;
  host_entry=gethostbyname(hostname);
  if(host_entry){
    return inet_ntoa (*(struct in_addr *)*host_entry->h_addr_list);
  }
  else return NULL;
}
 
bool http_create_socket(SOCKET *sock, SOCKADDR_IN *sin, const char *ip)
{
  *sock = socket(AF_INET, SOCK_STREAM, 0);		//init the socket
 
  sin->sin_addr.s_addr = inet_addr(ip);			//init the socket address on ip / port / network
  sin->sin_port = htons(PORT);
  sin->sin_family = AF_INET;
 
  if(connect(*sock, (SOCKADDR*)sin, sizeof(*sin)) == SOCKET_ERROR)return false;

#ifdef linux
  fcntl(*sock, F_SETFL, O_NONBLOCK);
#elif defined WIN32
  u_long mode=1;
  ioctlsocket(*sock,FIONBIO,&mode); 
#endif

  return true;
 
}
 
char* http_request(SOCKET sock, const char *hostname, const char *url)
{
  char buf[BUFSIZ];
  char *result = NULL;
  int l = 0;
  int selection;

  #if 0
  l = sprintf(buf, "GET %s %s\r\n", page, HTTP_VERSION);
  send(sock, buf, l, 0);
  buf[0] = '\0';
  l = sprintf(buf, "Host: %s\r\n", hostname);
  send(sock, buf, l, 0);
  buf[0] = '\0';
  send(sock, USER_AGENT, strlen(USER_AGENT), 0);
  send(sock, ENCODE_TYPE, strlen(ENCODE_TYPE), 0);
  send(sock, CONNECTION_TYPE, strlen(CONNECTION_TYPE), 0);
  send(sock, "\r\n", strlen("\r\n"), 0);
  #endif
//#define IP_CONNECT_STRING "http://%s/MLB/AddGK?cmd=%s&wip_no=%s&station_type=%s"
  l = sprintf(buf, IP_CONNECT_STRING, IP_ADDRESS, routing_para->cmd, routing_para->wip_no, routing_para->station_type);
  send(sock, buf, l, 0);
  // timeout
  struct timeval tv;
  tv.tv_sec = TIMEOUT_SEC;
  tv.tv_usec = TIMEOUT_MSEC;

  fd_set fdread;
  FD_ZERO(&fdread);
  FD_SET(sock, &fdread);

  while(1) {

    selection = select(sock + 1, &fdread, NULL, NULL, &tv);

    if(!selection) {
      l = 0;
      break;
    } else {
      if(FD_ISSET(sock, &fdread)) {
        l = recv(sock, buf, BUFSIZ, 0);
        if(l > 0) {
          buf[l] = '\0';
          if(result) {
            result = realloc(result, (strlen(result) + l + 1) * sizeof(char));
          } else {
            result = malloc((l + 1) * sizeof(char));  //allocate size of received data + end of string character
            result[0] = '\0';
          }

          strncat(result, buf, l);
        }
      }

    }
  }


  return result;
}

char* http_header_strip(char* content)
{
  return strstr(content, "\r\n\r\n")+4;
}

int QueryRoutingParameterList() {
  WSADATA WSAData;
  char url[BUFSIZ];
  int erreur = WSAStartup(MAKEWORD(2, 2), &WSAData);
  if(erreur) {
    puts("Cannot start WSA");
    exit(EXIT_FAILURE);
  }
  SOCKET sock;
  SOCKADDR_IN sin;

  if(!http_create_socket(&sock, &sin, IP_ADDRESS)) {
    puts("Cannot create http socket");
    exit(EXIT_FAILURE);
  }
  printf("Connected to %s/%s (%s:%d)\n", IP_ADDRESS,page, inet_ntoa(sin.sin_addr), htons(sin.sin_port));
  char *content = NULL;
  s_query_routing_parameter_list routing_para = { 0 };
  routing_para.cmd = 0 ;
  routing_para.model="00";
  routing_para.station_type = "00";
  routing_para.wip_no = "00";
  sprintf(url, IP_CONNECT_STRING, IP_ADDRESS, routing_para.cmd, routing_para.wip_no, routing_para.station_type);
  content = http_request(sock, NULL, url);

  if(content == NULL) {
    puts(ERR_NODATA);
    exit(EXIT_FAILURE);
  }

  puts(content);

  //char *content_core = http_header_strip(content);

  free(content);
  closesocket(sock);

#if defined (WIN32)
  WSACleanup();
#endif

  return EXIT_SUCCESS;
}
//http://10.195.226.56/MLB/AddGK?cmd=QUERY_ROUTING&wip_no=BIR0000000000000&station_type=BB1
int parseRoutingResponse(char *content ){

}
int getElement(const char *content, char *tag, char *value) {
  char buf[100] = { 0 };
  char *subString = NULL;
  char tempChar = 0;
  int i = 0;
  if(NULL == tag || NULL == content || NULL == value) {
    return -1;
  }
  if((subString = strstr(content, tag)) == NULL) {
    return -1;
  }
  subString += strlen(tag) + strlen("="); //include '='
  tempChar = *subString;
  while(tempChar != '\n'
        && tempChar != '\0'
        && tempChar != '&') {
    buf[i] = tempChar;
    tempChar = *(++subString);
    i++;
  }
  strcpy(value, buf);
  return 0;
}
