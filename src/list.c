#include "list.h"

void nshList_destroyList(List lst) {
    while (lst != NULL) {
        List tmp = lst;
        lst = lst->next;
        free(tmp);
    }
    return;
}

void nshList_destroyAll(List lst) {
    while (lst != NULL) {
        free(lst->v);
        List tmp = lst;
        lst = lst->next;
        free(tmp);
    }
    return;
}

List nshList_push(List lst, void* elem) {
    List add = (List)malloc(sizeof(struct s_list));
    add->next = NULL; add->v = elem;

    if (lst == NULL) return add;

    List tmp = lst;
    while(tmp->next != NULL) tmp = tmp->next;
    tmp->next = add;
    return lst;
}

void nshList_printListString(List lst) {
    putchar('[');
    List tmp;
    for (tmp=lst; tmp->next != NULL; tmp=tmp->next) {
        printf("\"%s\", ", (char*)(tmp->v));
    }
    printf("\"%s\"]\n", (char*)(tmp->v));
}