/* 
 * Developed by R. E. Bryant, 2017
 * Extended to store strings, 2018
 */

/*
 * This program implements a queue supporting both FIFO and LIFO
 * operations.
 *
 * It uses a singly-linked list to represent the set of queue elements
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

/*
  Create empty queue.
  Return NULL if could not allocate space.
*/

queue_t *q_new()
{
    queue_t *q =  malloc(sizeof(queue_t));
    /* What if malloc returned NULL? */
    if(q == NULL){ //must test to make sure q is pointing to somewhere in memory
      return NULL; //return NULL if it is not
    }
    q->head = NULL;
    q->countQ = 0; //declaring the sizeOfQ variable which keeps track of size for later use
    q->tail = NULL; //declaring tail node -- assigning this to next node when inserting so it falls on the last node
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    if(q == NULL){
      return;
    }
    /* How about freeing the list elements and the strings? */
    /* Free queue structure */
     while(q->head != NULL){ //loop through entire list deleting the node's values & the node itself
      //i would like to call q_remove_head here but i dont think it will work considering the perameters
      //instead this code is copied from that method minus the copying string part
       list_ele_t *toDelete = q->head;
       q->head = q->head->next;
       free(toDelete->value);
       free(toDelete);
       
    }
    
    free(q);

}

/*
  Attempt to insert element at head of queue.
  Return true if successful.
  Return false if q is NULL or could not allocate space.
  Argument s points to the string to be stored.
  The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(queue_t *q, char *s)
{
    /* What should you do if the q is NULL? */
    if(q == NULL){ //test for if q is null
      return false;
    }
    list_ele_t *newh = malloc(sizeof(list_ele_t)); //creates new "node" & allocating memory for node 
    /* Don't forget to allocate space for the string and copy it */
    /* What if either call to malloc returns NULL? */
    if(newh == NULL){ //testing if either malloc call returned null
      return false;
    }
    newh->value = (char *)malloc(strlen(s) + 1); //creating new space in memory for the string size of len + 1 for the terminating 0
    if(newh->value == NULL){ //testing if either malloc call returned null
      free(newh); //at this point, newh was already declared in memory so we must free it
      return false;
    }
    strcpy(newh->value, s); //copying the string into the newly assigned memory
    if(q->head == NULL){ //if this is true, we assign our tail pointer to the first inserted element b/c this func is insert at front so the tail elem would be the first inserted
      q->tail = newh;
      newh->next = NULL;
    }
    newh->next = q->head; //this code actually inserts the node
    q->head = newh;
    q->countQ += 1; //incrememnting count by 1 to keep track of size
    
    return true;
}


/*
  Attempt to insert element at tail of queue.
  Return true if successful.
  Return false if q is NULL or could not allocate space.
  Argument s points to the string to be stored.
  The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)
{
    if(q == NULL){
      return false;
    }

    list_ele_t *newhTail = malloc(sizeof(list_ele_t)); //creates new "node" for tail & allocating memory for node
    if(newhTail == NULL){ //testing if either malloc call returned null
      return false;
    }
    newhTail->value = (char *)malloc(strlen(s) + 1); //creating new space in memory for the string size of len + 1 for the terminating 0

    if(newhTail->value == NULL){ //testing if either malloc call returned null
      free(newhTail); //at this point, newh was already declared in memory so we must free it
      return false;
    }
    strcpy(newhTail->value, s); //copying the string into the newly assigned memory

    //list is empty base case
    if(q->head == NULL){ //if the list is empty we must assign head to point to the elem and the tail pointer to this elem
      q->head = newhTail; //head points to new elem
      newhTail->next = NULL; //new elem points to NULL
      q->countQ += 1; //incrememnting count by 1 to keep track of size
      q->tail = newhTail; //assigning new tail to this element (only element in list)
      return true;
      // free(newhTail);
      // free(newhTail->value);
      
      // return q_insert_head(*q, (char *)s);  <-- the code above does the same thing as an insertathead call when the queue is empty
      // that is a more efficient approach instead of re-writing this code above
    }
    //if we reach this point we assume the list has at least 1 element therefore the the next elem should
    //be appended onto the tail elem
    //remember tail is pointing to last elem
    q->tail->next = newhTail; //assigning tail -> next (so the last element) to point at new elem
    q->tail = newhTail; //newElem is now the tail 
    newhTail->next = NULL; //newElem points to NULL b/c it's the tail
    q->countQ += 1; //incrememnting count by 1 to keep track of size
    
    /* You need to write the complete code for this function */
    /* Remember: It should operate in O(1) time */
    return true;
}

/*
  Attempt to remove element from head of queue.
  Return true if successful.
  Return false if queue is NULL or empty.
  If sp is non-NULL and an element is removed, copy the removed string to *sp
  (up to a maximum of bufsize-1 characters, plus a null terminator.)
  The space used by the list element and the string should be freed.
*/
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    if(q == NULL || q->head == NULL || sp == NULL){ //testing all NULL cases in one line
      return false;
    }
    char *toCopy = q->head->value;
    
    //char *sp
    while(bufsize-1 > 0){ //copying the string to be deleted into sp with size bufsize
      *sp = *toCopy;
      sp+=1;
      toCopy+=1;
      bufsize-=1;
    }
    *sp = '\0'; //remember to set the terminating 0
    
    //at this point we know the list has at least 1 elem so we can remove it
    if(q->head == q->tail){ //this needs to be done if there is only one element in the list so we can remove the tail pointer
      q->tail = NULL;
    }
    list_ele_t *toDelete = q->head;
    /* You need to fix up this code. */
    q->head = q->head->next;
    free(toDelete->value);
    free(toDelete);
    q->countQ -= 1; //subtracting count b/c we just removed an element
    return true;
}

/*
  Return number of elements in queue.
  Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
  if(q == NULL || q->head == NULL){
    return false;
  }
    /* You need to write the code for this function */
    /* Remember: It should operate in O(1) time */
  return q->countQ;
}

/*
  Reverse elements in queue
  No effect if q is NULL or empty
  This function should not allocate or free any list elements
  (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
  It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
    /* You need to write the code for this function */
    if(q == NULL || q->head == NULL){
      return;
    }
    list_ele_t *currentElem = q->head; //basically points to the current element we are on
    list_ele_t *previousElem = NULL; //this is to be able to swap the pointers to the next elem
    list_ele_t *nextElem = NULL; //this is to work our way down the list

    q->tail = q->head; //this is true because the very first element in the list will end up being the last
    while(currentElem !=NULL){
      nextElem = currentElem->next; //this is the current elem we are working on
      currentElem->next = previousElem; //this assigns current to the previous value aka swapping the pointers 180
      previousElem = currentElem; //moves previous to the next elem
      currentElem = nextElem; //moves current to the next elem
    }
    q->head = previousElem;
    
}

