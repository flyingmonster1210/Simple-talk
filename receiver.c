#include "list.h"
#include "general.h"
#include "printer.h"
#include "receiver.h"

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>

static pthread_t thread;
static pthread_mutex_t ReceiverMutex = PTHREAD_MUTEX_INITIALIZER;
static List* plist = NULL;
static int mySocket;
static struct sockaddr_in myAddr;
static struct sockaddr_in reAddr;

static void* receive_info_from_others()
{
    char* msg = NULL;
    while(1) {
        msg = malloc(MAX_STRING_LEN);
        socklen_t size_reAddr = sizeof(reAddr);
        int len = recvfrom(mySocket, msg, MAX_STRING_LEN, 0,
                           (struct sockaddr*) &reAddr, &size_reAddr);
        int terminateIdx = (len < MAX_STRING_LEN) ? len : MAX_STRING_LEN - 1;
		msg[terminateIdx] = 0;
//        printf("--Receiver: strlen = %d, info = %s", strlen(msg), msg);
        pthread_mutex_lock(&ReceiverMutex);
        if(List_add(plist, msg) == -1) {
            printf("**failure-rev: running out of free nodes, do nothing...\n");
        }
        pthread_mutex_unlock(&ReceiverMutex);
        Printer_signal();
    }
    return NULL;
}

void Receiver_init(List* s_plist, int s_mySocket, struct sockaddr_in s_myAddr, struct sockaddr_in s_reAddr)
{
    mySocket = s_mySocket;
    myAddr = s_myAddr;
    reAddr = s_reAddr;
    plist = s_plist;
    pthread_create(&thread, NULL, receive_info_from_others, NULL);
}

void Receiver_join()
{
//    pthread_cancel(thread);
    pthread_join(thread, NULL);
}

void Receiver_cancel()
{
//    printf("receiver_cancel...\n");
    pthread_cancel(thread);
//    close(socketDescriptor);
}
