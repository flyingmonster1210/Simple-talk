#include "list.h"
#include "printer.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static pthread_t thread;
static pthread_mutex_t PrinterMutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t PrinterCondVar = PTHREAD_COND_INITIALIZER;
static List* plist;

static void* print_info_to_user()
{
    char *info = NULL;

    while(1) {
        pthread_mutex_lock(&PrinterMutex);
        pthread_cond_wait(&PrinterCondVar, &PrinterMutex);
        info = (char*)List_remove(plist);
        pthread_mutex_unlock(&PrinterMutex);

        if(info == NULL) {
            printf("**failure-prf: cannot get any node from list in printer...\n");
            continue;
        }
        printf("(from remote): %s", info);
//        printf("--Printer: strlen = %d, info = %s", (int)strlen(info), info);
        free(info);
        info = NULL;
    }
    return NULL;
}


void Printer_init(List* s_plist)
{
    plist = s_plist;
    pthread_create(&thread, NULL, print_info_to_user, NULL);
//    pthread_create(&thread, NULL, read_info_from_user, NULL);
}

void Printer_join()
{
//    pthread_cancel(thread);
    pthread_join(thread, NULL);
}

void Printer_signal()
{
    pthread_mutex_lock(&PrinterMutex);
    {
        pthread_cond_signal(&PrinterCondVar);
    }
    pthread_mutex_unlock(&PrinterMutex);
}

void Printer_cancel()
{
//    printf("printer_cancel...\n");
    pthread_cancel(thread);
}
