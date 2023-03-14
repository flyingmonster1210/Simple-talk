// my general.c
#include "list.h"
#include "general.h"
#include "reader.h"
#include "printer.h"
#include "receiver.h"
#include "sender.h"

#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include <netdb.h> // for myaddr
#include <unistd.h> // for gethosthome
#include <arpa/inet.h> // inet_aton


static List* plist_Reader_and_Sender = NULL;
static List* plist_Receiver_and_printer = NULL;
static struct sockaddr_in myAddr;
static struct sockaddr_in reAddr;
static int mySocket = -1;

// input you port number, the remote port number and the remote IP address
static bool initialSockets()
{
    char remoteIP[300];
    unsigned int myPort, rePort;
    printf("input myPort number: ");
    scanf("%d", &myPort);
    printf("input remotePort number: ");
    scanf("%d", &rePort);
    // getchar() is only used to get '\n' aftering inputing rePort,
    // if you do something like: myPort = 1111; rePort = 2222;
    // you should comment getchar();
    getchar();
    printf("input remote IP address: ");
    if(fgets(remoteIP, 300, stdin) == NULL) {
        printf("**failue-gen: cannot get remote IP address...\n");
        return 0;
    }

    memset((char *)&myAddr, 0, sizeof(myAddr));
    myAddr.sin_family = AF_INET;
	myAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	myAddr.sin_port = htons(myPort);
	if((mySocket = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        printf("**failure-gen: cannot create mySocket...\n");
        return 0;
	}
	if(bind(mySocket, (struct  sockaddr *)&myAddr, sizeof(myAddr)) < 0) {
        perror("**failure-gen: cannot bind myScocket and myAddr...\n");
        return 0;
    }

    memset((char *) &reAddr, 0, sizeof(reAddr));
	reAddr.sin_family = AF_INET;
	reAddr.sin_port = htons(rePort);
    reAddr.sin_addr.s_addr = inet_addr(remoteIP);

    return 1;
}

void s_talk()
{
    printf("\n**********************************************\n");
    printf("**********  s_talk starts running!  **********\n");
    printf("**********************************************\n");
    printf("--input '!' to end s_talk\n");
    printf("----------------------------------------------\n");

    plist_Reader_and_Sender = List_create();
    plist_Receiver_and_printer = List_create();
    if(plist_Reader_and_Sender == NULL || plist_Receiver_and_printer == NULL) {
        printf("**failure-gen: cannot create at lease two lists in main function...\n");
        printf("**********************************************\n");
        printf("************  s_talk is done!  ***************\n");
        printf("**********************************************\n\n");
        return ;
    }

    printf("--initializing sockets...\n");
    // port numbers and remote IP address are needed while initialzing
    if(!initialSockets()) {
        printf("**failue-gen: fail to initialize sockets...\n");
        printf("**********************************************\n");
        printf("************  s_talk is done!  ***************\n");
        printf("**********************************************\n\n");
        return ;
    }
    printf("--initialization seccuss!\n----------------------------------------------\n\n");

    Reader_init(plist_Reader_and_Sender);
    Printer_init(plist_Receiver_and_printer);
    Sender_init(plist_Reader_and_Sender, mySocket, myAddr, reAddr);
    Receiver_init(plist_Receiver_and_printer, mySocket, myAddr, reAddr);

    Reader_join();
    Printer_join();
    Sender_join();
    Receiver_join();
//    shutdown_everything();

    close(mySocket);

    printf("**********************************************\n");
    printf("************  s_talk is done!  ***************\n");
    printf("**********************************************\n\n");
}

static void itemFree(void *pItem)
{
    if(pItem != NULL) free((char*)pItem);
}


// shutdown_everything() will be called by reader when it reads a '!'
void shutdown_everything()
{
//    printf("shutdown_everything in general...\n");
    Reader_cancel();
    Printer_cancel();
    Sender_cancel();;
    Receiver_cancel();

    List_free(plist_Reader_and_Sender, itemFree);
    List_free(plist_Receiver_and_printer, itemFree);
    plist_Reader_and_Sender = plist_Receiver_and_printer = NULL;
}







