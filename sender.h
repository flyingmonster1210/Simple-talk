#ifndef _SENDER_H_
#define _SENDER_H_

#include <sys/socket.h>
struct sockaddr_in;

void Sender_init(List* s_plist, int s_mySocket, struct sockaddr_in s_myAddr, struct sockaddr_in s_reAddr);
void Sender_join();
void Sender_signal();
void Sender_cancel();

#endif // _SENDER_H_
