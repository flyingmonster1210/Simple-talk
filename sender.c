#include "list.h"
#include "sender.h"
#include "general.h"

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>			// for strncmp()
#include <unistd.h>			// for close()

static pthread_t thread;
static pthread_mutex_t SenderMutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t SenderCondVar = PTHREAD_COND_INITIALIZER;
static List* plist = NULL;
static int mySocket;
static struct sockaddr_in myAddr;
static struct sockaddr_in reAddr;

static void* send_info_to_others()
{
    char *info = NULL;
    while(1) {
        pthread_mutex_lock(&SenderMutex);
        pthread_cond_wait(&SenderCondVar, &SenderMutex);
        info = List_remove(plist);
        pthread_mutex_unlock(&SenderMutex);

        if(info == NULL) {
            printf("**failure-sed: cannot get any node from list...\n");
            continue;
        }
//        printf("--Sender: strlen = %d, info = %s", (int)strlen(info), info);
        socklen_t size_reAddr = sizeof(reAddr);
        sendto(mySocket, info, strlen(info), 0,
               (struct sockaddr*) &reAddr, size_reAddr);
        free(info);
        info = NULL;
    }
    return NULL;
}

void Sender_init(List* s_plist, int s_mySocket, struct sockaddr_in s_myAddr, struct sockaddr_in s_reAddr)
{
    mySocket = s_mySocket;
    myAddr = s_myAddr;
    reAddr = s_reAddr;
    plist = s_plist;
    pthread_create(&thread, NULL, send_info_to_others, NULL);
}

void Sender_join()
{
    pthread_join(thread, NULL);
}

void Sender_signal()
{
    pthread_mutex_lock(&SenderMutex);
    {
        pthread_cond_signal(&SenderCondVar);
    }
    pthread_mutex_unlock(&SenderMutex);
}

void Sender_cancel()
{
    pthread_cancel(thread);
}
