#include "list.h"

List rootshList_new(void* elem) {
    return rootshList_push(NULL, elem);
}

void rootshList_destroyList(List lst) {
    while (lst != NULL) {
        List tmp = lst;
        lst = lst->next;
        free(tmp);
    }
    return;
}

List rootshList_destroyElem(List lst) {
    List next = lst->next;
    free(lst);
    return next;
}

void rootshList_destroy2DList(List list2D) {
    while (list2D != NULL) {
        rootshList_destroyList(list2D->v);
        list2D = rootshList_destroyElem(list2D);
    }
}

void rootshList_destroy2DListAll(List list2D) {
    while (list2D != NULL) {
        rootshList_destroyAll(list2D->v);
        list2D = rootshList_destroyElem(list2D);
    }
}

void rootshList_destroyAll(List lst) {
    while (lst != NULL) {
        free(lst->v);
        List tmp = lst;
        lst = lst->next;
        free(tmp);
    }
    return;
}

List rootshList_push(List lst, void* elem) {
    List add = (List)malloc(sizeof(struct s_list));
    add->next = NULL; add->v = elem;

    if (lst == NULL) return add;

    List tmp = lst;
    while(tmp->next != NULL) tmp = tmp->next;
    tmp->next = add;
    return lst;
}

List rootshList_popDestroyValue(List lst, int destroy) {
    List tmp = lst;
    
    if (tmp->next == NULL) {
        return rootshList_destroyElem(tmp);
    }

    while (tmp->next->next != NULL) tmp = tmp->next;

    if (destroy == 1) {
        free(tmp->next->v);
        tmp->next = rootshList_destroyElem(tmp->next);
    }else {
        tmp->next = rootshList_destroyElem(tmp->next);
    }
    
    return lst;
}

void rootshList_printListString(List lst) {
    putchar('[');
    List tmp;
    for (tmp=lst; tmp->next != NULL; tmp=tmp->next) {
        printf("\"%s\", ", (char*)(tmp->v));
    }
    printf("\"%s\"]\n", (char*)(tmp->v));
}