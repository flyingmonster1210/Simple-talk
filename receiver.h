#ifndef _PRINTERER_H_
#define _PRINTERER_H_

#include <sys/socket.h>
struct sockaddr_in;

void Receiver_init(List* s_plist, int s_mySocket, struct sockaddr_in s_myAddr, struct sockaddr_in s_reAddr);
void Receiver_join();
void Receiver_cancel();

#endif // _PRINTERER_H_
