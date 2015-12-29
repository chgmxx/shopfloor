#include "error.h"

int main(){

}

int QueryRoutingParameterList(){
    WSADATA WSAData;
    int erreur = WSAStartup(MAKEWORD(2, 2), &WSAData);
    if(erreur) {
        puts("Cannot start WSA");
        exit(EXIT_FAILURE);
    }
}
