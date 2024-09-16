#include <stdio.h>
#include <stdlib.h>
#include "list.h"


static List heads[LIST_MAX_NUM_HEADS];
static Node nodes[LIST_MAX_NUM_NODES];

static int nodeCounter[LIST_MAX_NUM_NODES];
static int headCounter[LIST_MAX_NUM_HEADS];
static int topNode = -1;
static int topHead = -1;
static bool firstTime = true;

void pushNode(int value){
    topNode++;
    nodeCounter[topNode] = value;
}

int popNode(){
    if (topNode == -1) return -1;
    return nodeCounter[topNode--];
    
}

void pushHead(int value){
    topHead++;
    headCounter[topHead] = value;

}

int popHead(){
    if (topHead == -1) return -1;
    return headCounter[topHead--];
    
}


List* List_create(){
    if (firstTime == true){
        int headNum = 0;
        int nodeNum = 0;
        while(headNum < LIST_MAX_NUM_HEADS){
            pushHead(headNum);
            headNum++;
        }
        while(nodeNum < LIST_MAX_NUM_NODES){
            pushNode(nodeNum);
            nodeNum++;
        }
        firstTime = false;
    }
    int num = popHead();
    if (num == -1) return NULL;
    List* newList = &heads[num];
    newList->start = NULL;
    newList->end = NULL;
    newList->counter = NULL;
    newList->itemNum = 0;
    newList->state = LIST_OOB_START;
    newList->headNum = num;
    return newList;
}

int List_count(List* pList){
    return pList->itemNum;
}

void* List_first(List* pList){
    if (!pList->start){;
        return NULL;
    }
    if (pList->state == LIST_OOB_END || pList->state == LIST_OOB_START) pList->state = -1;
    pList->counter = pList->start;
    return pList->counter->item;
}

void* List_last(List* pList){
    if (!pList->start){
        return NULL;
    }
    if (pList->state == LIST_OOB_END || pList->state == LIST_OOB_START) pList->state = -1;
    pList->counter = pList->end;
    return pList->counter->item;
}

void* List_next(List* pList){
    if (!pList->start){
        return NULL;
    }
    if (pList->counter == pList->end || pList->state == LIST_OOB_END){
        pList->counter = NULL;
        pList->state = LIST_OOB_END;
        return NULL;
    }
    if (pList->state == LIST_OOB_START){
        pList->counter = pList->start;
        pList->state = -1;
        return pList->counter->item;
    }
    pList->counter = pList->counter->next;
    return pList->counter->item;
}

void* List_prev(List* pList){
    if (!pList->start){
        return NULL;
    }
    if (pList->counter == pList->start || pList->state == LIST_OOB_START){
        pList->counter = NULL;
        pList->state = LIST_OOB_START;
        return NULL;
    }
    if (pList->state == LIST_OOB_END){
        pList->counter = pList->end;
        pList->state = -1;
        return pList->counter->item;
    }

    pList->counter = pList->counter->prev;
    return pList->counter->item;
}

void* List_curr(List* pList){
    if(pList->counter){
        return pList->counter->item;
    }
    return NULL;
}

int List_insert_after(List* pList, void* pItem){
    int num = popNode();
    if (num == -1){
        return LIST_FAIL;
    }
    
    nodes[num].item = pItem;
    nodes[num].next = NULL;
    nodes[num].prev = NULL;
    nodes[num].nodeNum = num;
    pList->itemNum++;
    if (!pList->start){
        pList->counter = &nodes[num];
        pList->start = &nodes[num];
        pList->end = &nodes[num];
        
        return LIST_SUCCESS;
    }
    if (pList->state == LIST_OOB_END || pList->counter == pList->end){
        nodes[num].prev = pList->end;
        pList->end->next = &nodes[num];
        pList->end = &nodes[num];
        pList->counter = pList->end;
        pList->state = -1;
        
        return LIST_SUCCESS;
    }
    if (pList->state == LIST_OOB_START){
        nodes[num].next = pList->start;
        pList->start->prev = &nodes[num];
        pList->start = &nodes[num];
        pList->counter = pList->start;
        pList->state = -1;
        
        return LIST_SUCCESS;
    }
    nodes[num].prev = pList->counter;
    nodes[num].next = pList->counter->next;
    pList->counter->next->prev = &nodes[num];
    pList->counter->next = &nodes[num];
    pList->counter = &nodes[num];
    
    return LIST_SUCCESS;
}

int List_insert_before(List* pList, void* pItem){
    int num = popNode();
    if (num == -1){
        return LIST_FAIL;
    }
    
    nodes[num].item = pItem;
    nodes[num].next = NULL;
    nodes[num].prev = NULL;
    nodes[num].nodeNum = num;
    pList->itemNum++;
    if (!pList->start){
        pList->counter = &nodes[num];
        pList->start = &nodes[num];
        pList->end = &nodes[num];
        
    }
    if (pList->state == LIST_OOB_START || pList->counter == pList->start){
        nodes[num].next = pList->start;
        pList->start->prev = &nodes[num];
        pList->start = &nodes[num];
        pList->counter = pList->start;
        pList->state = -1;
        
        return LIST_SUCCESS;
    }
    if (pList->state == LIST_OOB_END){
        nodes[num].prev = pList->end;
        pList->end->next = &nodes[num];
        pList->end = &nodes[num];
        pList->counter = pList->end;
        pList->state = -1;
        
        return LIST_SUCCESS;
    }
    nodes[num].next = pList->counter;
    nodes[num].prev = pList->counter->prev;
    pList->counter->prev->next = &nodes[num];
    pList->counter->prev = &nodes[num];
    pList->counter = &nodes[num];
    
    return LIST_SUCCESS;
}

int List_append(List* pList, void* pItem){
    int num = popNode();
    if (num == -1) return LIST_FAIL;
    nodes[num].item = pItem;
    nodes[num].next = NULL;
    nodes[num].prev = NULL;
    nodes[num].nodeNum = num;
    if (!pList->start){
        pList->start = &nodes[num];
        pList->end = &nodes[num];
        pList->counter= &nodes[num];
        pList->itemNum++;
        return LIST_SUCCESS;
    }

    nodes[num].prev = pList->end;
    pList->end->next = &nodes[num];
    pList->end = &nodes[num];
    pList->counter =pList->end;
    pList->itemNum++;
    pList->state = -1;
    return LIST_SUCCESS;
}

int List_prepend(List* pList, void* pItem){
    int num = popNode();
    if (num == -1) return LIST_FAIL;
    nodes[num].item = pItem;
    nodes[num].next = NULL;
    nodes[num].prev = NULL;
    nodes[num].nodeNum = num;
    if (!pList->start){
        pList->start = &nodes[num];
        pList->end = &nodes[num];
        pList->counter= &nodes[num];
        pList->itemNum++;
        return LIST_SUCCESS;
    }
    nodes[num].next = pList->start;
    pList->start->prev = &nodes[num];
    pList->start = &nodes[num];
    pList->counter = pList->start;
    pList->itemNum++;
    pList->state = -1;
    return LIST_SUCCESS;
}

void* List_remove(List* pList){
    if (!pList->start || pList->state == LIST_OOB_END || pList->state == LIST_OOB_START) return NULL;
    void* tempItem = pList->counter->item;

    if (pList->itemNum == 1){
        pList->end = NULL;
        pList->start = NULL;
        pushNode(pList->counter->nodeNum);
        pList->counter->item = NULL;
        pList->counter->next = NULL;
        pList->counter->prev = NULL;
        pList->counter = NULL;
        pList->state = LIST_OOB_END;
        pList->itemNum--;
        return tempItem;
    }

    if (pList->counter == pList->start){
        pList->start = pList->start->next;
        pList->start->prev = NULL;
        pushNode(pList->counter->nodeNum);
        pList->counter->item = NULL;
        pList->counter->next = NULL;
        pList->counter->prev = NULL;
        pList->counter = pList->start;
        pList->itemNum--;
        return tempItem;
    }

    if (pList->counter == pList->end){
        pList->end = pList->end->prev;
        pList->end->next = NULL;
        pushNode(pList->counter->nodeNum);
        pList->counter->item = NULL;
        pList->counter->next = NULL;
        pList->counter->prev = NULL;
        pList->counter = NULL;
        pList->state = LIST_OOB_END;
        pList->itemNum--;
        return tempItem;
    }

    

    Node* temp = pList->counter->next;
    pList->counter->prev->next = temp;
    pList->counter->next->prev = pList->counter->prev;
    pushNode(pList->counter->nodeNum);
    pList->counter->item = NULL;
    pList->counter->next = NULL;
    pList->counter->prev = NULL;
    pList->counter = temp;
    pList->itemNum--;
    return tempItem;

}

void* List_trim(List* pList){
    if (!pList->start) return NULL;
    void* temp = pList->end->item;

    //Test this out to see if free the node makes pList start, end, and current pointers
    //equal the value NULL
    if (pList->itemNum == 1){
        pushNode(pList->end->nodeNum);
        pList->end = NULL;
        pList->start = NULL;
        pList->counter = NULL;
        pList->itemNum--;
        if (pList->state != -1) pList->state = -1;
        return temp;
    }

    pList->counter = pList->end;
    pList->end = pList->end->prev;
    pList->end->next = NULL;
    pushNode(pList->counter->nodeNum);
    pList->counter->item = NULL;
    pList->counter->next = NULL;
    pList->counter->prev = NULL;
    pList->counter = pList->end;
    pList->itemNum--;
    if (pList->state != -1) pList->state = -1;
    return temp;
}

void List_concat(List* pList1, List* pList2){

    if (!pList2->start){
        pushHead(pList2->headNum);
        pList2->start = NULL;
        pList2->end = NULL;
        pList2->counter = NULL;
        pList2->itemNum = 0;
        pList2->state = -1;
    }else if (!pList1->start){
        pList1->start = pList2->start;
        pList1->end = pList2->end;
        pList1->itemNum+=pList2->itemNum;
        pushHead(pList2->headNum);
        pList2->start = NULL;
        pList2->end = NULL;
        pList2->counter = NULL;
        pList2->itemNum = 0;
        pList2->state = -1;

    }else{
        pList2->counter = pList1->end;
        pList1->end->next = pList2->start;
        pList2->start->prev = pList2->counter;
        pList1->end = pList2->end;
        pList1->itemNum+=pList2->itemNum;
        pushHead(pList2->headNum);
        pList2->start = NULL;
        pList2->end = NULL;
        pList2->counter = NULL;
        pList2->itemNum = 0;
        pList2->state = -1;
    }

    

}

void List_free(List* pList, FREE_FN pItemFreeFn){
    
    while (pList->start){
        pList->counter = pList->start;
        pList->start = pList->start->next;
        pList->start->prev = NULL;
        (*pItemFreeFn)(pList->counter->item);
        pushNode(pList->counter->nodeNum);
        pList->counter->item = NULL;
        pList->counter->next = NULL;
        pList->counter->prev = NULL;
    }

    pushHead(pList->headNum);
    pList->start = NULL;
    pList->end = NULL;
    pList->counter = NULL;
    pList->itemNum = 0;
    pList->state = LIST_OOB_START;
}

void* List_search(List* pList, COMPARATOR_FN pComparator, void* pComparisonArg){
    if (pList->itemNum == 0) return NULL;

    if (pList->state == LIST_OOB_START){
        pList->counter = pList->start;
    }


    while (pList->counter){
        if ((*pComparator)(pList->counter->item, pComparisonArg) == 1) return pList->counter->item;
        pList->counter = pList->counter->next;
    }
    pList->state = LIST_OOB_END;
    return NULL;
}