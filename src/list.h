#ifndef LISTG
#define LISTG

#include <stdio.h>
#include <stdlib.h>
#include "constants.h"

typedef struct s_list {
    void* v;
    struct s_list* next;
}*List;

/**
 * Free the whole list `lst`.
 * Please note that it will not free the value, for this you want to use `destroyAll`
 *
 * @short Free the `lst` list
 * @param lst       The list to free
 */
void rootshList_destroyList(List lst);

/**
 * Free the whole list `lst` and the value inside every cell of the list
 *
 * @short Free list cell and list values
 * @param lst       The list to free
 */
void rootshList_destroyAll(List lst);

/**
 * Push the element `elem` at the end of the list `lst` by creating a new list cell
 *
 * @short Put the element `elem` in the list `lst`
 * @param lst       The list where the element is pushed
 * @param elem      The element to push in the list
 * @return A list with the new element in it
 */
List rootshList_push(List lst, void *elem);

/**
 * Print the list of char* `lst` 
 * 
 * @param lst   The list to print
 */
void rootshList_printListString(List lst);

#endif