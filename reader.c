#include "list.h"
#include "reader.h"
#include "sender.h"
#include "general.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static pthread_t thread;
static pthread_mutex_t ReaderMutex = PTHREAD_MUTEX_INITIALIZER;
static List* plist = NULL;

static void* read_info_from_user()
{
    char *info = NULL;

    while(1) {
        info = (char*)malloc(MAX_STRING_LEN);
        if(fgets(info, MAX_STRING_LEN, stdin) == NULL) {
            printf("**failure-rea: cannot read above information...\n");
            continue;
        }
//        printf("--Reader: strlen = %d, info = %s", (int)strlen(info), info);
        int len = strlen(info);
        if(len == 2 && info[0] == '!') {    // shutdown all the threads we have
            printf("\n----------------------------------------------\n");
//            printf("show the list\n");
//            print_all_information();
            printf("--s_talk is going to end...\n");
            shutdown_everything();
//            pthread_cancel(thread);
        }

        pthread_mutex_lock(&ReaderMutex);
        if(List_add(plist, info) == -1) {
            printf("**failure-rea: running out of free nodes, do nothing in reader...\n");
        }
        pthread_mutex_unlock(&ReaderMutex);
        Sender_signal();
    }
    return NULL;
}

void Reader_init(List* s_plist)
{
    plist = s_plist;
    pthread_create(&thread, NULL, read_info_from_user, NULL);
}

void Reader_join()
{
//    pthread_cancel(thread);
    pthread_join(thread, NULL);
}

void Reader_cancel()
{
//    printf("reader_cancel...\n");
    pthread_cancel(thread);
//    printf("%d\n", pthread_cancel(thread));
}










