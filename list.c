#include <stdio.h>
#include "list.h"

static bool flag = 0;
static int num = 0;                                 //how many list we have, smallest num is 0
static int cnt = 0;                                 //how many node we have, smallest cnt is 0
static int nextEmptyNode = LIST_MAX_NUM_NODES-1;    //it is used in emptyNode, smallest is -1
static int nextEmptyList = LIST_MAX_NUM_HEADS-1;    //it is used in emptyNode, smallest is -1
static Node nodeArray[LIST_MAX_NUM_NODES];
static List listArray[LIST_MAX_NUM_HEADS];
static int  emptyNode[LIST_MAX_NUM_NODES];
static List*  emptyList[LIST_MAX_NUM_HEADS];

static void doNothing(void *pItem) {
    return ;
}

void print_all_information() {
    printf("\n************************************************************************************\n");
    printf("*----------------------------- print_all_information ------------------------------*\n");
    printf("************************************************************************************\n\n");
    printf("(A).\n************************************************************\n");
    printf("\nLIST_MAX_NUM_HEADS:  %d\t LIST_MAX_NUM_NODES:  %d\n\n", LIST_MAX_NUM_HEADS, LIST_MAX_NUM_NODES);
//    printf("***********************************************************\n");
    printf("print all the information including nodeArray, emptyNode array, emptyList array, lists and check array,\n");
    printf("if prev of one node is -1, that means there is no ancestor,\n");
    printf("if next of one node is -2, that means there is no follower.");
    printf("\n\n\n");
    printf("(B).\n*************************************************************\n");
    //print nodeArray
    printf("\t\t ---- print nodeArray ----\n");
    printf("*************************************************************\n");
    printf("%d nodes are being used\n\n", cnt);
    printf("index\t| prev\t pItem\t\toccupy\tnext\n");
    printf("---------------------------------------------\n");
    for(int i = 0; i < LIST_MAX_NUM_NODES; i++) {
        printf("%d\t| %d\t %p\t  %d\t%d\n", i,nodeArray[i].prev, nodeArray[i].pItem, nodeArray[i].occupied, nodeArray[i].next);
    }
    printf("\n\n\n");
    printf("(C).\n*************************************************************\n");
    //print emptyNode array
    printf("\t\t---- print emptyNode array ----\n");
    printf("*************************************************************\n");
    printf("we still have %d empty nodes, here is the emptyNode array:\n", nextEmptyNode+1);
    for(int i = 0; i <= nextEmptyNode; i++) {
        printf("%d   ", emptyNode[i]);
    }
    printf("\n\n\n");
    printf("(D).\n*************************************************************\n");
    printf("\t\t---- print emptyList array ----\n");
    printf("*************************************************************\n");
    printf("we still have %d empty list's heads, here is the emptyList array:\n", nextEmptyList+1);
    for(int i = 0; i <= nextEmptyList; i++) {
        printf("%p  ", emptyList[i]);
    }
    printf("\n\n\n");
    printf("(E).\n*************************************************************\n");
    //print lists
    printf("\t\t---- print each list ----\n");
    printf("*************************************************************\n");
    int check[LIST_MAX_NUM_NODES] = {0};
    int cnt_check = 0;
    printf("we now have %d lists(start from 0)\n", num);
    for(int i = 0; i < LIST_MAX_NUM_HEADS; i++) {
        int next = listArray[i].begin;

        printf("\nthis is the %dth list with begin index %d and end index %d,\n", i, listArray[i].begin, listArray[i].end);
        printf("and the size of this list is %d, and the curr index is %d.\n", listArray[i].size, listArray[i].curr);
        printf("---------------------------------------------------------\n");
        for(int k = 0; next >= 0; next = nodeArray[next].next) {
            ++check[next];
            cnt_check += check[next];
            printf("the  %dth  node has index %d\n", k++, next);
        }
    }
    printf("\n\n");
    printf("(F).\n*************************************************************\n");
    //print check array
    printf("\t\t---- print check array ----\n");
    printf("*************************************************************\n");
    printf("check how many time we read each node:\n");
    printf("(if lists intersect, then it will read the same node several times)\n");
    for(int i = 0; i < LIST_MAX_NUM_NODES; i++) {
        printf("%d  ", check[i]);
    }
    printf("\n\nsum(check[0] to check[%d]) = %d", LIST_MAX_NUM_NODES-1, cnt_check);
    printf("\n\n\n");
    printf("************************************************************************************\n");
    printf("************************************************************************************\n\n");
}

// Makes a new, empty list, and returns its reference on success.
// Returns a NULL pointer on failure.
List* List_create() {
    if(num >= LIST_MAX_NUM_HEADS) {
//        printf("no more list can be CREATED, and we now have %d lists and the nextEmptyList(a index) is %d.\n", num, nextEmptyList);
        return NULL;
    }
    if(nextEmptyList < 0) {
//        printf("there are some error in list creation!!!\n\n");
        return NULL;
    }
    if(!flag) {
        for(int i = 0; i < LIST_MAX_NUM_NODES; i++) {
            emptyNode[i] = i;
            nodeArray[i].next = -2;
            nodeArray[i].prev = -1;
            nodeArray[i].occupied = 0;
            nodeArray[i].pItem = NULL;
        }
        for(int i = 0; i < LIST_MAX_NUM_HEADS; i++) {
            emptyList[i] = &listArray[i];
            listArray[i].begin = listArray[i].end = listArray[i].curr = listArray[i].size = -1;
            listArray[i].pNode = NULL;
        }
        flag = 1;
    }
    ++num;
    List* thisList = emptyList[nextEmptyList--];
    thisList->curr = -1;
    thisList->size = 0;
    thisList->begin = thisList->end = -1;
    thisList->pNode = NULL;
    return thisList;
}

// Returns the number of items in pList.
int List_count(List* pList) {
    if(pList == NULL) return -1;
    return pList->size;
}

// Returns a pointer to the first item in pList and makes the first item the current item.
// Returns NULL and sets current item to NULL if list is empty.
void* List_first(List* pList) {
    if(pList == NULL) return NULL;
    if(pList->size <= 0) {
        pList->pNode = NULL;
        pList->curr = pList->begin = pList->end = -1;
        return NULL;
    }

    pList->pNode = &nodeArray[pList->begin];
    pList->curr = pList->begin;
    return pList->pNode->pItem;
}

// Returns a pointer to the last item in pList and makes the last item the current item.
// Returns NULL and sets current item to NULL if list is empty.
void* List_last(List* pList) {
    if(pList == NULL) return NULL;
    if(pList->size <= 0) {
        pList->pNode = NULL;
        pList->curr = pList->begin = pList->end = -1;
        return NULL;
    }

    pList->pNode = &nodeArray[pList->end];
    pList->curr = pList->end;
    return pList->pNode->pItem;
}

// Advances pList's current item by one, and returns a pointer to the new current item.
// If this operation advances the current item beyond the end of the pList, a NULL pointer
// is returned and the current item is set to be beyond end of pList.
void* List_next(List* pList) {
    if(pList == NULL) return NULL;
    if(pList->size <= 0) return NULL;
    if(pList->curr == -2) return NULL;
    else if(pList->curr == -1) {
        pList->curr = pList->begin;
        pList->pNode = &nodeArray[pList->curr];
    }
    else if(pList->curr == pList->end) {
        pList->pNode = NULL;
        pList->curr = -2;
        return NULL;
    }
    else {
        pList->curr = nodeArray[pList->curr].next;
        pList->pNode = &nodeArray[pList->curr];
    }
    return pList->pNode->pItem;
}

// Backs up pList's current item by one, and returns a pointer to the new current item.
// If this operation backs up the current item beyond the start of the pList, a NULL pointer
// is returned and the current item is set to be before the start of pList.
void* List_prev(List* pList) {
    if(pList == NULL) return NULL;
    if(pList->size <= 0) return NULL;
    if(pList->curr == -1) return NULL;
    else if(pList->curr == -2) {
        pList->curr = pList->end;
        pList->pNode = &nodeArray[pList->curr];
    }
    else if(pList->curr == pList->begin) {
        pList->pNode = NULL;
        pList->curr = -1;
        return NULL;
    }
    else {
        pList->curr = nodeArray[pList->curr].prev;
        pList->pNode = &nodeArray[pList->curr];
    }
    return pList->pNode->pItem;
}

// Returns a pointer to the current item in pList.
void* List_curr(List* pList) {
    if(pList == NULL) return NULL;
    if(pList->pNode == NULL) return NULL;
    return pList->pNode->pItem;
}

// Adds the new item to pList directly after the current item, and makes item the current item.
// If the current pointer is before the start of the pList, the item is added at the start. If
// the current pointer is beyond the end of the pList, the item is added at the end.
// Returns 0 on success, -1 on failure.
int List_add(List* pList, void* pItem) {
    if(pList == NULL) return -1;
    if(cnt >= LIST_MAX_NUM_NODES) return -1;

    cnt++;
    int index = emptyNode[nextEmptyNode--];
    if(pList->size <= 0) {
        pList->begin = pList->end = pList->curr = index;
        pList->size = 1;
        pList->pNode = &nodeArray[index];
        pList->pNode->pItem = pItem;
        pList->pNode->next = -2;
        pList->pNode->prev = -1;
        pList->pNode->occupied = 1;
    }
    else if(pList->curr == pList->end) {
        nodeArray[index].prev = pList->end;
        nodeArray[index].next = -2;
        nodeArray[index].pItem = pItem;
        nodeArray[index].occupied = 1;
        pList->size++;
        pList->end = index;
        pList->pNode->next = index;
        pList->pNode = &nodeArray[index];
    }
    else if(pList->pNode == NULL) {
        pList->size++;
        if(pList->curr == -1) {
            nodeArray[index].pItem = pItem;
            nodeArray[index].next = pList->begin;
            nodeArray[index].prev = -1;
            nodeArray[index].occupied = 1;
            nodeArray[pList->begin].prev = index;
            pList->begin = index;
        }
        else {
            nodeArray[index].pItem = pItem;
            nodeArray[index].next = -2;
            nodeArray[index].prev = pList->end;
            nodeArray[index].occupied = 1;
            nodeArray[pList->end].next = index;
            pList->end = index;
        }
        pList->pNode = &nodeArray[index];
        pList->curr = index;
    }
    else {
        pList->size++;
        nodeArray[index].prev = pList->curr;
        nodeArray[index].pItem = pItem;
        nodeArray[index].occupied = 1;
        nodeArray[index].next = nodeArray[pList->curr].next;
        nodeArray[nodeArray[index].next].prev = index;
        nodeArray[pList->curr].next = index;
    }

    return 0;
}

// Adds item to pList directly before the current item, and makes the new item the current one.
// If the current pointer is before the start of the pList, the item is added at the start.
// If the current pointer is beyond the end of the pList, the item is added at the end.
// Returns 0 on success, -1 on failure.
int List_insert(List* pList, void* pItem) {
    if(pList == NULL) return -1;
    if(cnt >= LIST_MAX_NUM_NODES) return -1;

    if(pList->pNode == NULL) {
        return List_add(pList, pItem);
    }

    int index = emptyNode[nextEmptyNode--];
    cnt++;
    pList->size++;
    if(pList->curr == pList->begin) {
        nodeArray[index].prev = -1;
        nodeArray[index].pItem = pItem;
        nodeArray[index].occupied = 1;
        nodeArray[index].next = pList->begin;
        nodeArray[pList->begin].prev = index;
        pList->curr = index;
        pList->pNode = &nodeArray[index];
        pList->begin = index;
    }
    else {
        nodeArray[index].prev = nodeArray[pList->curr].prev;
        nodeArray[index].pItem = pItem;
        nodeArray[index].occupied = 1;
        nodeArray[index].next = pList->curr;
        nodeArray[nodeArray[index].prev].next = index;
        nodeArray[nodeArray[index].next].prev = index;
        pList->curr = index;
        pList->pNode = &nodeArray[index];
    }

    return 0;
}

// Adds item to the end of pList, and makes the new item the current one.
// Returns 0 on success, -1 on failure.
int List_append(List* pList, void* pItem) {
    if(pList == NULL) return -1;
    if(cnt >= LIST_MAX_NUM_NODES) return -1;

//    printf("%p", pList);
//    for(int i = pList->begin; i > 0; i = nodeArray[i].next){
//        printf(" %d", i);
//    }
//    printf(" --- %d --- %d\n", num, cnt);

    if(pList->size > 0) {
        pList->curr = pList->end;
        pList->pNode = &nodeArray[pList->end];
    }
    return List_add(pList, pItem);
}

// Adds item to the front of pList, and makes the new item the current one.
// Returns 0 on success, -1 on failure.
int List_prepend(List* pList, void* pItem) {
    if(pList == NULL) return -1;
    if(cnt >= LIST_MAX_NUM_NODES) return -1;

    pList->curr = -1;
    pList->pNode = NULL;
//    printf("%p", pList);
//    for(int i = pList->begin; i > 0; i++){
//        printf(" %d", i);
//    }
//    printf("\n");
    return List_add(pList, pItem);
}

// Return current item and take it out of pList. Make the next item the current one.
// If the current pointer is before the start of the pList, or beyond the end of the pList,
// then do not change the pList and return NULL.
void* List_remove(List* pList) {    // remove the current node
    if(pList == NULL) return NULL;
    if(pList->curr < 0) return NULL;
    if(pList->size <= 0) return NULL;
    if(cnt <= 0) return NULL;

    cnt--;
    int curr = pList->curr;
    int next = nodeArray[curr].next;
    int prev = nodeArray[curr].prev;
    void *item = nodeArray[curr].pItem;
    nodeArray[curr].pItem = NULL;
    nodeArray[curr].occupied = 0;
    nodeArray[curr].next = -2;
    nodeArray[curr].prev = -1;
    emptyNode[++nextEmptyNode] = curr;
    if(pList->size == 1) {
        pList->size = 0;
        pList->begin = pList->end = pList->curr = -1;
        pList->pNode = NULL;
    }
    else if(curr == pList->begin) {
        pList->size--;
        nodeArray[next].prev = -1;
        pList->begin = next;
        pList->curr = next;
        pList->pNode = &nodeArray[next];
    }
    else if(curr == pList->end) {
        pList->size--;
        nodeArray[prev].next = -2;
        pList->end = prev;
        pList->curr = -2;
        pList->pNode = NULL;
    }
    else {
        pList->size--;
        nodeArray[prev].next = next;
        nodeArray[next].prev = prev;
        pList->curr = next;
        pList->pNode = &nodeArray[next];
    }

    return item;
}

// Adds pList2 to the end of pList1. The current pointer is set to the current pointer of pList1.
// pList2 no longer exists after the operation; its head is available
// for future operations.
void List_concat(List* pList1, List* pList2) {
    if(pList1 == pList2) {
//        printf("error:\twhen you concat pList1 and pList2, pList1 must be different from pList2!\n");
        return ;
    }
    if(pList2 == NULL) return ;
    if(pList1 == NULL) {
        pList1 = pList2;
        pList2 = NULL;
    }
    else if(pList1->size <= 0) {
        pList1->size = pList2->size;
        pList1->begin = pList2->begin;
        pList1->end = pList2->end;
        pList2->begin = pList2->end = pList2->curr = -1;
        pList2->pNode = NULL;
        pList2->size = -1;
    }
    else if(pList2->size <= 0) {
        pList2->begin = pList2->end = pList2->curr = -1;
        pList2->pNode = NULL;
        pList2->size = -1;
    }
    else {
        nodeArray[pList1->end].next = pList2->begin;
        nodeArray[pList2->begin].prev = pList1->end;
        pList1->size = pList1->size+pList2->size;
        pList1->end = pList2->end;
        pList2->begin = pList2->end = pList2->curr = pList2->size = -1;
        pList2->pNode = NULL;
    }

    List_free(pList2, doNothing);
}

// Delete pList. itemFree is a pointer to a routine that frees an item.
// It should be invoked (within List_free) as: (*pItemFree)(itemToBeFreedFromNode);
// pList and all its nodes no longer exists after the operation; its head and nodes are
// available for future operations.
//------------------------------------------------------------------------
    // next time maybe we should not change the function prototype,
    // the return type of two function pointers are not the same.
    // I don't know it has been changed until I run the sampleTest file,
    // and that takes lots of time to find this small bug...
    // void List_free(List* pList, void* pItemFree);
//------------------------------------------------------------------------
typedef void (*FREE_FN)(void* pItem);
void List_free(List* pList, FREE_FN pItemFreeFn) {
    if(num <= 0) {
//        printf("error:\tno more list can be FREED, and we now have %d list and the nextEmptyList(a index) is %d.\n", num, nextEmptyList);
        return ;
    }
    if(pList == NULL) {
//        printf("warning:\tcannot free a NULL list!\n");
        return ;
    }

    --num;
    emptyList[++nextEmptyList] = pList;
    if(pList->size <= 0) {
        pList->size = -1;
        return ;
    }
    List_first(pList);
    while(pList->curr >= 0) {
        ( (void*(*)(List*))pItemFreeFn)(pList->pNode->pItem);
        List_remove(pList);
    }
    pList->begin = pList->end = pList->curr = pList->size = -1;
    pList->pNode = NULL;
}

// Return last item and take it out of pList. Make the new last item the current one.
// Return NULL if pList is initially empty.
void* List_trim(List* pList) {
    if(pList == NULL) return NULL;
    if(pList->size <= 0) return NULL;

    pList->curr = pList->end;
    pList->pNode = &nodeArray[pList->curr];
    void *tmp = List_remove(pList);
    List_last(pList);
    return tmp;
}

// Search pList, starting at the current item, until the end is reached or a match is found.
// In this context, a match is determined by the comparator parameter. This parameter is a
// pointer to a routine that takes as its first argument an item pointer, and as its second
// argument pComparisonArg. Comparator returns 0 if the item and comparisonArg don't match,
// or 1 if they do. Exactly what constitutes a match is up to the implementor of comparator.
//
// If a match is found, the current pointer is left at the matched item and the pointer to
// that item is returned. If no match is found, the current pointer is left beyond the end of
// the list and a NULL pointer is returned.
typedef bool (*COMPARATOR_FN)(void* pItem, void* pComparisonArg);
void* List_search(List* pList, COMPARATOR_FN pComparator, void* pComparisonArg) {
    if(pList == NULL) return NULL;
    if(pList->size <= 0) return NULL;
    if(pList->curr == -2) return NULL;

    if(pList->curr == -1) {
        pList->curr = pList->begin;
        pList->pNode = &nodeArray[pList->begin];
    }
    while(pList->curr >= 0 && !pComparator(pList->pNode->pItem, pComparisonArg)) {
        pList->curr = nodeArray[pList->curr].next;
        if(pList->curr >= 0) pList->pNode = &nodeArray[pList->curr];
    }
    if(pList->curr < 0) {
        pList->pNode = NULL;
        return NULL;
    }
    return pList->pNode->pItem;
}



















