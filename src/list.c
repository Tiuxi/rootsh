#include "list.h"

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

void rootshList_printListString(List lst) {
    putchar('[');
    List tmp;
    for (tmp=lst; tmp->next != NULL; tmp=tmp->next) {
        printf("\"%s\", ", (char*)(tmp->v));
    }
    printf("\"%s\"]\n", (char*)(tmp->v));
}