#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "skipList.h"
#include <time.h>

/* ************************************************************************
   Main Function for Testing
 ************************************************************************ */
/*
param: no parameters
pre:	no parameters
post: prints out the contents of the skip list 
*/
int main(){
   int i,M;
   struct skipList *slst1, *slst2;


   srand ( time(NULL) );

   /* allocate memory to the pointers of Skip List */
   slst1=(struct skipList *)malloc(sizeof(struct skipList));  
   slst2=(struct skipList *)malloc(sizeof(struct skipList));  
   assert(slst1); assert(slst2);


   /*  Initialize the two skip lists */
   initSkipList(slst1); 
   initSkipList(slst2);  

   /*  Add to the skip list  M = 20 random integers in [0,100] */
   M=20;
   for(i=0;i<M;i++) {
      addSkipList(slst1, rand()%101);
   }
   for(i=0;i<M;i++) addSkipList(slst2, rand()%101);

   /*  Print out the skip list 
       in the breadth-first order, starting from top. 
       In printing, move to a new printing line every time
       the end of the current level is reached.
       E.g., the print out of a skip list 
       with 5 elements should look like

       7
       7 14 29
       3 7 9 14 20

*/

   printf("\nPrinting of Skip List 1 started ... \n");
   printSkipList(slst1);
   printf("Printing of Skip List 1 finished. \n\n\n");

   printf("\nPrinting of Skip List 2 started ... \n");
   printSkipList(slst2);
   printf("Printing of Skip List 2 finished. \n\n\n");




   /* DIFFERENCE AND MERGING */
   printf("TESTING DIFFERENCE \n");
   printf("Adding the same numbers to both skip lists \n");

   for(i=200; i<=205; i++) {
      addSkipList(slst1, i); 
      addSkipList(slst2, i);
   }

   diffSkipList(slst1, slst2);

   printf("\nPrinting of Skip List 1 after difference started ... \n");
   printSkipList(slst1);
   printf("Printing of Skip List 1 after difference finished. \n\n\n");

   printf("TESTING MERGING \n");
   mergeSkipList(slst1, slst2);

   printf("\nPrinting of Skip List 1 after merging started ... \n");
   printSkipList(slst1);
   printf("Printing of Skip List 1 after merging finished. \n\n\n");



   /* Develop test cases for evaluating your functions:  
      int containsSkipList(struct skipList *slst, TYPE e) 
      int removeSkipList(struct skipList *slst, TYPE e)
      */
   printf("TESTING CONTAINS \n");
   for(i=200; i<=205; i++)
      printf("\n%d %s in the list!.\n",i,containsSkipList(slst1, i)==1?"IS":"IS NOT");

   printf("TESTING REMOVE \n");
   printf("\nRemoving 200,202 and 205 from Skip List 1 started ... \n");
      removeSkipList(slst1, 200);
      removeSkipList(slst1, 202);
      removeSkipList(slst1, 205);

   printf("\nPrinting of Skip List 1 after removing started ...\n");
   printSkipList(slst1);
   printf("\nRemoving 200,202 and 205 from Skip list 1 finished ... \n\n\n");


   for(i=200; i<=205; i++)
      printf("\n%d %s in the list!.\n",i,containsSkipList(slst1, i)==1?"IS":"IS NOT");
   freeSkipList(slst1);
   return 0;

}


/* ************************************************************************
   Internal Functions
 ************************************************************************ */

/* Coin toss function:
param: 	no parameters
pre:	no parameres
post: output is a random intiger number in {0,1} */
int flipSkipLink(void)
{
   return rand() % 2; 	
}

/* Move to the right as long as the next element is smaller than the input value:
param: 	current -- pointer to a place in the list from where we need to slide to the right
param:	e --  input value
pre:	current is not NULL
post: returns one link before the link that contains the input value e */
struct skipLink * slideRightSkipList(struct skipLink *current, TYPE e){
   while ((current->next != 0) && LT(current->next->value, e))
      current = current->next;
   return current;
}


/* Create a new skip link for a value
param: e	 -- the value to create a link for
param: nextLnk	 -- the new link's next should point to nextLnk
param: downLnk   -- the new link's down should point to downLnk
pre:	none
post:	a link to store the value */
struct skipLink* newSkipLink(TYPE e, struct skipLink * nextLnk, struct skipLink* downLnk) {
   struct skipLink * tmp = (struct skipLink *) malloc(sizeof(struct skipLink));
   assert(tmp != 0);
   tmp->value = e;
   tmp->next = nextLnk;
   tmp->down = downLnk;
   return tmp;
}


/* Add a new skip link recursively
param: current -- pointer to a place in the list where the new element should be inserted
param: e       -- the value to create a link for
pre:	current is not NULL
post: a link to store the value */

/*struct skipLink* skipLinkAdd(struct skipLink * current, TYPE e) {
   struct skipLink* temp;
   struct skipLink* prev;
   if(current->down == NULL){
      printf("== in skipLinkAdd base call; e = %g\n", e);
      temp = current;
      while(temp->next != NULL && LT(temp->next->value, e)){
	 printf("temp == temp->next");
	 temp = temp->next;
      }
      struct skipLink * newLink = newSkipLink(e, temp->next, NULL);
      temp->next = newLink;
      return newLink;
   }
   else{
      printf("== in skipLinkAdd recursive call; e = %g\n", e);
      temp = current->down; 
      while(temp->next != NULL && LT(temp->next->value, e)){
	 temp = temp->next;
      }
      struct skipLink * newLink = newSkipLink(e, temp->next, skipLinkAdd(temp, e));
      temp->next = newLink;
      return newLink;
   }
    FIXME *//*
}
*/

/* Add a new skip link recursively - new test function
param: current -- pointer to a place in the list where the new element should be inserted
param: e       -- the value to create a link for
pre:	current is not NULL
post: a link to store the value */
struct skipLink* skipLinkAdd(struct skipLink * current, TYPE e) {
   struct skipLink* prev;
   struct skipLink * newLink;
   while(current->next != NULL && LT(current->next->value, e)){
      prev = current;
      current = current->next;
   }
   if(current->down == NULL){
      newLink = newSkipLink(e, current->next, NULL);
      current->next = newLink;
   }
   else{
      newLink = newSkipLink(e, current->next, skipLinkAdd(current->down, e));
      current->next = newLink;
   }
   return newLink;
   /* FIXME */
}

/* ************************************************************************
   Public Functions
 ************************************************************************ */

/* Initialize skip list:
param:  slst -- pointer to the skip list
pre:	slst is not null
post: the sentinels are allocated, the pointers are set, and the list size equals zero */
void initSkipList (struct skipList *slst) 
{
   slst->size=0;
   slst->topSentinel=(struct skipLink * )malloc(sizeof(struct skipLink));
   assert(slst->topSentinel);
   slst->topSentinel->next=NULL;
   slst->topSentinel->down=NULL;
}

/* Checks if an element is in the skip list:
param: slst -- pointer to the skip list
param: e -- element to be checked
pre:	slst is not null
post: returns true or false  */
/*
int containsSkipList(struct skipList *slst, TYPE e)
{
   printf("\n== in containsSkipList\n");
   if(slst == NULL){
      return 0;
   }
   struct skipLink* temp = slst->topSentinel;
   while(temp->down != NULL || temp->next != NULL){
      if(temp->next != NULL && EQ(e, temp->next->value)){
         return 1;
      }
      if(temp->next == NULL){
         temp = temp->down;
	 continue;
      }
      if(temp->down == NULL && LT(e, temp->next->value)){
         return 0;
      }
      if(temp->next != NULL && LT(temp->next->value, e)){
         temp = temp->next;
	 continue;
      }
      if(temp->next != NULL && LT(e, temp->next->value)){
         temp = temp->down;
	 continue;
      }
   }
   return 0;
}
*/
int containsSkipList(struct skipList *slst, TYPE e)
{
   if(slst == NULL){
      return 0;
   }
   struct skipLink* temp = slst->topSentinel;
   while(temp->down != NULL){
      if(temp->next != NULL && LT(temp->next->value, e)){
         temp = temp->next;
      }
      else{
         temp = temp->down;
      }
   }
   while(temp->next != NULL){
      if(LT(temp->next->value, e)){
         temp = temp->next;
      }
      else{
         break;
      }
   }
   return isEqual(temp->next, e);
}

/* Test for link equality:
param: test -- value to be checked
param: e -- valuse to be checked against
pre:   
post:  return tested bool */
int isEqual(struct skipLink* test, TYPE e){
   if(test == NULL){
      return 0;
   }
   if(test->value == e){
      return 1;
   }
   else return 0;
}
/* FIXME */


/* Remove an element from the skip list:
param: slst -- pointer to the skip list
param: e -- element to be removed
pre:   slst is not null
post:  the new element is removed from all internal sorted lists */
/*void removeSkipList(struct skipList *slst, TYPE e)
{
   assert(slst != NULL);
   if(!containsSkipList(slst, e)){
      return;
   }
   struct skipLink* temp;
   struct skipLink* temp2;
   temp = slst->topSentinel;
   while(temp->down != NULL){
      if(temp->next == NULL){
         temp = temp->down;
      }
      else if(temp->next != NULL && LT(e, temp->next->value)){
         temp = temp->down;
      }
      else if(temp->next != NULL && LT(temp->next->value, e)){
         temp = temp->next;
      }
      if(temp->next != NULL && EQ(e, temp->next->value)){
	 temp2 = temp->next;
	 temp->next = temp->next->next;
	 free(temp2); 
      }
   }
   if(temp->next == NULL && temp->down == NULL){
      return;
   }
   else if(temp->down == NULL){
      while(temp != NULL){
	 if(temp->next != NULL && EQ(temp->next->value, e)){
	    temp2 = temp->next;
	    temp->next = temp->next->next;
	    free(temp2);
	 }
	 temp = temp->next;
      }
   }
}*/
   /* FIXME */

void removeSkipList(struct skipList *slst, TYPE e)
{
   assert(slst != NULL);
   if(!containsSkipList(slst, e)){
      return;
   }
   struct skipLink* temp;
   struct skipLink* freeLink;
   temp = slst->topSentinel;
   while(temp != NULL){
      while(temp->next != NULL && LT(temp->next->value, e)){
	 temp = temp->next;
      }
      if(isEqual(temp->next, e)){
         freeLink = temp->next;
	 temp->next = temp->next->next;
	 free(freeLink);
      }
      temp = temp->down; 
   }
   slst->size--;
}


/* Add a new element to the skip list:
param: slst -- pointer to the skip list
param: e -- element to be added
pre:	slst is not null
post:	the new element is added to the lowest list and randomly to higher-level lists */
void addSkipList(struct skipList *slst, TYPE e)
{ 
   if(containsSkipList(slst, e)){
      return;
   }
   int n_lists = 1;
   int counter = 1;
   int i;
   struct skipLink* temp = slst->topSentinel;

   while(temp->down != NULL){
      n_lists++;
      temp = temp->down;
   }

   while(flipSkipLink()){
      counter++;
   }

   if(counter > n_lists){
      for(i = 0; i < counter - n_lists; i++){
	 addTopSentinel(slst);
      }
   }
   temp = slst->topSentinel;
   while(counter < n_lists){
      temp = temp->down;
      counter++;
   }

   while(temp->next != NULL && LT(temp->next->value, e)){
      temp = temp->next;
   }

   skipLinkAdd(temp, e);
   slst->size++;
   /* FIXME */

}

/* Add a new topSentinel to slst
param: slst -- pointer to the skip list
pre:	slst is not null
post: new empty list added above top sentinel */
void addTopSentinel(struct skipList *slst){
   slst->topSentinel = newSkipLink(-1, NULL, slst->topSentinel);
}

/* Find the number of elements in the skip list:
param: slst -- pointer to the skip list
pre:	slst is not null
post: the number of elements */
int sizeSkipList(struct skipList *slst){

   return slst->size;

}


/* Print the links in the skip list:
param: slst -- pointer to the skip list
pre:	slst is not null and slst is not empty
post: the links in the skip list are printed breadth-first, top-down */
void printSkipList(struct skipList *slst)
{
   printf("== in print skiplist\n");
   struct skipLink* sentTemp = slst->topSentinel;
   struct skipLink* listTemp = slst->topSentinel;
   while(sentTemp != NULL){
      listTemp = sentTemp->next;
      while(listTemp != NULL){
	 printf(" %g", listTemp->value);
	 listTemp = listTemp->next;
      }
      printf("\n");
      sentTemp = sentTemp->down;
   }
   printf("\n");

   /* FIXME*/

}


/* Merge two skip lists, by adding elements of skip list 2 to skip list 1 
   that are not already contained in skip list 1. 
   The function should also remove the entire skip list 2 from the memory.
param: slst1 -- pointer to the skip list 1
param: slst2 -- pointer to the skip list 2
pre: slst1 and slst2 are not null, and skip list 1 and skip list 2 are not empty
post: slst1 points to the merger,  slst2 is null*/
void mergeSkipList(struct skipList *slst1, struct skipList *slst2)
{
   struct skipLink* iterator;
   TYPE tempVal;
   iterator = slst2->topSentinel; 
   while(iterator->down != NULL){ /*loop puts the iterator at the bottom list sentinel of slst2*/
      iterator = iterator->down;
   }
   while(iterator->next != NULL){
      tempVal = iterator->next->value;
      addSkipList(slst1, tempVal);
      removeSkipList(slst2, tempVal);
      iterator = iterator->next;
   }
   freeSkipList(slst2);
}


/* free a skip list 
param: slst -- pointer to the skip list
pre: 
post: slst points to NULL*/
void freeSkipList(struct skipList* slst){
   if(slst == NULL) return;
   struct skipLink* temp = slst->topSentinel;
   struct skipLink* freeLink;
   while(slst->topSentinel != NULL){
      temp = slst->topSentinel;
      while(temp->next != NULL){
	 freeLink = temp->next;
	 temp->next = temp->next->next;
	 free(freeLink);
      }
      slst->topSentinel = temp->down;
      free(temp);
   }
   free(slst);
}


/* Find a difference of two skip lists 
   by removing elements of skip list 2 from skip list 1. 
param: slst1 -- pointer to the skip list 1
param: slst2 -- pointer to the skip list 2
pre: slst1 and slst2 are not null, and skip list 1 and skip list 2 are not empty
post: slst1 points to the merger skip list*/
void diffSkipList(struct skipList *slst1, struct skipList *slst2)
{
   /* FIXME */


} /* end of the function */
