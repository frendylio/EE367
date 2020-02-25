/*
 * Frendy
 */
#include <stdio.h>
#include <stdlib.h>

struct Node {
    int val;
    struct Node *next;
};

void displayList(struct Node *head);
struct Node * destroyList(struct Node *head);
struct Node *createList(int startVal, int finishVal);

struct Node *reorderList(struct Node *head);

void main() 
{

struct Node *p = createList(3,13);

p = createList(3,13);
printf("\nCreated a list:\n");
displayList(p);
p =reorderList(p);
printf("Reordered list:\n");
displayList(p);
p = destroyList(p);

p = createList(3,12);
printf("\nCreated a list:\n");
displayList(p);
p =reorderList(p);
printf("Reordered list:\n");
displayList(p);
p = destroyList(p);

}

// Helper funct to reverseList

struct Node *reverseList(struct Node *head){
    // Base case: 1 element or empty
    if(!head || !(head->next)){
        return head;
    }

    // Variables
    struct Node *Last = head;
    struct Node *Current = head->next;
    struct Node *Next = NULL;
    
    //destroy head
    head->next = NULL;

    // reverse
    while(Current){
        Next = Current->next;
        Current->next = Last;
        Last = Current;
        Current = Next;
    }

    return Last;
}

// reorder
struct Node *reorderList(struct Node *head)
{
    // Base case: 1 element or empty
    if(!head || !(head->next)){
        return head;
    }

    int i;
    int length;
    struct Node *Current = head;

    // Length
    for(length = 0; Current; length++, Current = Current->next);

    // Get middle
    Current = head;
    for(i=1; i<(length+1)/2; i++, Current = Current->next );
    
    struct Node *Left = head;
    struct Node *Right = Current->next;
    Current->next = NULL;
    Right = reverseList(Right);

    //Merge
    struct Node *LeftNext;
    struct Node *RightNext;

    while(Left && Right){
        LeftNext = Left->next;
        RightNext = Right->next;

        Left->next = Right;
        Right->next = LeftNext;

        Left = LeftNext;
        Right = RightNext;
    }

    return head;
}


void displayList(struct Node *head)
{
printf("List: ");
for (struct Node *p=head; p!=NULL; p=p->next) {
    printf("->%d ",p->val);
}
printf("\n");
}

struct Node * destroyList(struct Node *head)
{
struct Node *p = head;
while (p!=NULL) {
	struct Node *next = p->next;
	free(p);
	p = next;
}
return NULL;
}

struct Node *createList(int startVal, int finishVal)
{
struct Node *head = NULL;  /* Head of the list */
struct Node *last = NULL;  /* Last node in the list */
for (int i=startVal; i<=finishVal; i++) {
    /* Create node */
    struct Node *p = (struct Node *) malloc(sizeof(struct Node));
    p->val = i;
    p->next = NULL;
    if (i == startVal) {
        head = p;
    }
    else {
	last->next = p;
    }
    last = p;
}
return head;
}


