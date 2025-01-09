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
 * Create a new `List` object with the element `elem` in the first position
 * 
 * @param elem The first element of the new list
 * @returns An new `List` object
 */
List rootshList_new(void* elem);

/**
 * Free the current element of the list `lst` and return the next one
 * 
 * @param lst   The element to free
 * @returns The next element
 */
List rootshList_destroyElem(List lst);

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
 * Free the 2D list `list2D`.
 * Please note that it will not free the value of the 2D List.
 * For this you may want to use `destroy2DListAll`
 * 
 * @param list2D    The 2D list to free
 */
void rootshList_destroy2DList(List list2D);

/**
 * Free the 2D list `list2D` and the values inside.
 *
 * @param list2D    The 2D list to free
 */
void rootshList_destroy2DListAll(List list2D);

/**
 * Free the last element of list `lst` and free the value if `destroy` is True (or 1)
 * 
 * @param lst       The list to remove the element from
 * @param destroy   If the last element value should be free'd or not
 */
List rootshList_popDestroyValue(List lst, int destroy);

#define rootshList_pop(lst) rootshList_popDestroyValue(lst, 0)
#define rootshList_popValue(lst) rootshList_popDestroyValue(lst, 1)

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