#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
queue_t *q_new()
{
    queue_t *q = malloc(sizeof(queue_t));
    if (!q)
        return NULL;

    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    /* TODO: How about freeing the list elements and the strings? */
    /* Free queue structure */
    if (!q)
        return;

    while (q->head) {
        list_ele_t *tmp = q->head;
        q->head = q->head->next;
        free(tmp->value);
        free(tmp);
    }
    free(q);
}

/*
 * Attempt to insert element at head of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(queue_t *q, char *s)
{
    /* What should you do if the q is NULL? */
    if (!q)
        return false;

    /* allocate space for list_ele_t */
    list_ele_t *newh;
    newh = malloc(sizeof(list_ele_t));
    if (!newh)
        return false;

    /* allocate space for the string and copy it */
    newh->value = malloc(sizeof(char) * (strlen(s) + 1));
    if (!newh->value) {
        free(newh);
        return false;
    }
    memset(newh->value, '\0', strlen(s) + 1);
    strncpy(newh->value, s, strlen(s));

    newh->next = q->head;
    q->head = newh;
    q->tail = q->size ? q->tail : newh;
    q->size++;
    return true;
}

/*
 * Attempt to insert element at tail of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)
{
    /* What should you do if the q is NULL? */
    if (!q)
        return false;

    /* allocate space for list_ele_t */
    list_ele_t *newt;
    newt = malloc(sizeof(list_ele_t));
    if (!newt)
        return false;

    /* allocate space for the string and copy it */

    newt->value = malloc(sizeof(char) * (strlen(s) + 1));
    if (!newt->value) {
        free(newt);
        return false;
    }
    memset(newt->value, '\0', strlen(s) + 1);
    strncpy(newt->value, s, strlen(s));
    newt->next = NULL;

    if (q->tail) {
        q->tail->next = newt;
    } else {
        q->head = newt;
    }
    q->tail = newt;
    q->size++;
    return true;
}

/*
 * Attempt to remove element from head of queue.
 * Return true if successful.
 * Return false if queue is NULL or empty.
 * If sp is non-NULL and an element is removed, copy the removed string to *sp
 * (up to a maximum of bufsize-1 characters, plus a null terminator.)
 * The space used by the list element and the string should be freed.
 */
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    /* if queue is NULL or empty */
    if (!q || !q->size)
        return false;

    list_ele_t *tmp = q->head;
    /* copy the removed string to *sp */
    if (sp) {
        memset(sp, '\0', bufsize);
        strncpy(sp, tmp->value, bufsize - 1);
    }

    q->head = q->head->next;
    if (!q->head)
        q->tail = q->head;

    q->size--;

    /* Free all storage used by q->head */
    free(tmp->value);
    free(tmp);
    return true;
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    return q ? q->size : 0;
}

/*
 * Reverse elements in queue
 * No effect if q is NULL or empty
 * This function should not allocate or free any list elements
 * (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
 * It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
    /* No effect if q is NULL or empty */
    if (!q || !q->size)
        return;

    list_ele_t *curr = q->head, *prev = curr->next;
    q->tail = curr;
    q->tail->next = NULL;
    while (prev) {
        list_ele_t *tmp = prev->next;
        prev->next = curr;
        curr = prev;
        prev = tmp;
    }
    q->head = curr;
}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */

list_ele_t *merge(list_ele_t *left, list_ele_t *right)
{
    if (!left)
        return right;
    if (!right)
        return left;

    list_ele_t *head, *merge;
    for (merge = NULL; right || left;) {
        if (!right) {
            merge->next = left;
            break;
        } else if (!left) {
            merge->next = right;
            break;
        }

        if (strcmp(left->value, right->value) > 0) {
            if (!merge) {
                head = merge = right;
            } else {
                merge->next = right;
                merge = merge->next;
            }
            right = right->next;
        } else {
            if (!merge) {
                head = merge = left;
            } else {
                merge->next = left;
                merge = merge->next;
            }
            left = left->next;
        }
    }

    return head;
}


list_ele_t *mergesort(list_ele_t *head)
{
    if (!head || !head->next)
        return head;

    // Split on half
    list_ele_t *fast, *slow;
    fast = head->next;
    slow = head;

    for (; fast && fast->next; fast = fast->next->next) {
        slow = slow->next;
    }

    fast = slow->next;
    slow->next = NULL;

    slow = mergesort(head);
    fast = mergesort(fast);

    return merge(slow, fast);
}

void q_sort(queue_t *q)
{
    /* No effect if q is NULL or empty.*/
    if (!q || !q->head) {
        return;
    }
    q->tail = q->head = mergesort(q->head);
    for (; q->tail->next; q->tail = q->tail->next)
        ;
}
