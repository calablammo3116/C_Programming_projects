/* Assignment 4 Entrance Evaluation
   Due October 26, 2022
   Caleb Gibson
   COP3502 - 0004
   Professor Travis Meade
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define INVALID -1  // value to denote invalid data

// struct definitions + prototypes
typedef struct Group
{
    long long int size, arrvl, prcssTime;
} Group;

typedef struct Node
{
    Group data;
    struct Node * prev;
    struct Node * next;
} Node;

typedef struct Queue
{
    Node * head;
    Node * tail;
} Queue;

// function prototypes
// queue function prototypes
Group front(Queue *);
void dequeue(Queue *);
void enqueue(Queue *, Group);
Queue * createQueue();
void cleanQueue(Queue *);
// circular ll function prototypes
Node * createNode(Group);
Node * addTail(Node *, Node *, Group);
Node * removeHead(Node *, Node *);
// sorting function prototype
void mergeSort(Group *, int);

int main()
{
    long long int wait = 0, lP = 0, rP = 0;   // lp = "left people", rp = "right people".
    int numGrps = 0;

    scanf("%d", &numGrps);

    Group * grps = (Group *)calloc(numGrps, sizeof(Group));

    for(int i = 0; i < numGrps; i++)
    {
        scanf("%lld %lld %lld", &grps[i].size, &grps[i].arrvl, &grps[i].prcssTime);
    }

    // Create the left and right queues
    Queue * lQ = createQueue(); // left queue
    Queue * rQ = createQueue(); // right queue

    // Sort the groups in ascending order of time of arrival
    mergeSort(grps, numGrps);

    // Do the simulation.
    for (int i = 0; i < numGrps; i++)
    {
        // while either queue contains groups at their respective fronts with (arrival times + processing times) greater than the current
        // group's arrival time, have those queues process those groups at their respective fronts and update a) the person counters for both
        // queues and b) the total wait based on those groups' (processing times * numbers of people in those groups) for those groups.
        if (lQ->tail != NULL && lQ->head != NULL)
        {
            while (lQ->head->data.arrvl + lQ->head->data.prcssTime < grps[i].arrvl && (lQ->tail != NULL && lQ->head != NULL))
            {
                lP = lP - lQ->head->data.size;   // update the counter for the number of people currently in the left queue.
                wait = wait + lQ->head->data.prcssTime * lQ->head->data.size;   // update the total wait time for the queues
                // Take the front group out of the queues.
                dequeue(lQ);

                if (lQ->tail == NULL && lQ->head == NULL)
                {
                    break;
                }
            }
        }
        // Now do the same for the right queue.
        if (rQ->tail != NULL && rQ->head != NULL)
        {
            while (rQ->head->data.arrvl + rQ->head->data.prcssTime < grps[i].arrvl && (rQ->tail != NULL && rQ->head != NULL))
            {
                rP = rP - rQ->head->data.size;
                wait = wait + rQ->head->data.prcssTime * rQ->head->data.size;
                // Take the front group out of the queue.
                dequeue(rQ);

                if (rQ->tail == NULL && rQ->head == NULL)
                {
                    break;
                }
            }
        }

        // Determine which of the two queues the group should go into.

        // Case for if the group should go into the LEFT queue.
        if (rP >= lP)
        {
            enqueue(lQ, grps[i]);

            // Update the group's processing time.
            // Should be ((difference in previous group's finish time and current group's arrival time) + current group's process time).
            if (lQ->tail == lQ->head){   // case if 1 group in left queue
                lP += grps[i].size;   // update the left queue's person counter
                continue;   // go to next iteration in the loop
            } else{   // case if more than 1 group in left queue
                lQ->tail->data.prcssTime += lQ->tail->prev->data.arrvl + lQ->tail->prev->data.prcssTime - lQ->tail->data.arrvl;
            }

            // Update the left queue's person counter.
            lP = lP + grps[i].size;
        }

        // Case for if the group should go into the RIGHT queue.
        else
        {
            enqueue(rQ, grps[i]);

            // Update the group's processing time.
            if (rQ->tail == rQ->head){   // case if 1 group in right queue
                rP += grps[i].size;   // update the right queue's person counter
                continue;   // go to next iteration in the loop
            } else{   // case if more than 1 group in right queue
                rQ->tail->data.prcssTime += rQ->tail->prev->data.arrvl + rQ->tail->prev->data.prcssTime - rQ->tail->data.arrvl;
            }

            // Update the right queue's person counter.
            rP = rP + grps[i].size;
        }
    }


    // Loop through the remaining groups for both queues, and process those groups in both queues.

    // Loop through the remaining groups in the LEFT queue.
    if (lQ->tail != NULL && lQ->head != NULL){
        while (lQ->tail != NULL && lQ->head != NULL)
        {
            wait += lQ->head->data.prcssTime * lQ->head->data.size;
            dequeue(lQ);
        }
    }

    // Loop through the remaining groups in the RIGHT queue.
    if (rQ->tail != NULL && rQ->head != NULL){
        while (rQ->tail != NULL && rQ->head != NULL)
        {
            wait += rQ->head->data.prcssTime * rQ->head->data.size;
            dequeue(rQ);
        }
    }

    // Print out the result.
    printf("%lld\n", wait);

    // Free dynamically allocated memory
    cleanQueue(lQ);
    cleanQueue(rQ);
    free(lQ);
    free(rQ);
    free(grps);

    return 0;
}

// non-main function definitions
// functions for a queue and a circular linked list, as well as a merge sort function
// queue functions
// function to dequeue (remove element from the front of a queue)
void dequeue(Queue * q)
{
    q->tail = removeHead(q->tail, q->head);
    if (q->tail != NULL){
        q->head = q->tail->next;
        q->head->prev = q->tail;
    } else{
        q->head = NULL;
    }
}

// function to enqueue a value (add a node to the end of the queue)
void enqueue(Queue * q, Group newGroup)
{
    q->tail = addTail(q->tail, q->head, newGroup);
    if (q->tail != NULL){
        q->head = q->tail->next;
        q->head->prev = q->tail;
    }
}

// function to create queue
Queue * createQueue()
{
    Queue * newQueue = (Queue *)malloc(sizeof(Queue));
    newQueue->tail = NULL;
    newQueue->head = NULL;
    return newQueue;
}

// function to clean queue
void cleanQueue(Queue * q)
{
    while(q->head != NULL && q->tail != NULL){
        dequeue(q);
    }
}

// circular linked list (ll) functions
// function to create a node in an empty list
Node * createNode(Group newGroup)
{
    Node * newNode = (Node *)malloc(sizeof(Node));

    // read in data into new node
    newNode->data.size = newGroup.size;
    newNode->data.arrvl = newGroup.arrvl;
    newNode->data.prcssTime = newGroup.prcssTime;

    // new node points to itself by default
    newNode->next = newNode;
    newNode->prev = newNode;

    return newNode;
}

// function to add element to tail of a circular ll (enqueue)
// return the tail of the ll
Node * addTail(Node * oldTail, Node * oldHead, Group newGroup)
{
    // case if list empty
    if(oldTail == NULL){
        // create new node to be the tail & head
        oldTail = createNode(newGroup);

        oldHead = oldTail;

        // return the resulting tail
        return oldTail;
    }

    // case if list not empty
    // create new tail node to be inserted at end of the list
    Node * newTail = createNode(newGroup);

    // set new tail to have previous point to old tail
    newTail->prev = oldTail;

    // set new tail to have next point to old head
    newTail->next = oldHead;

    // set old head to have previous point to new tail
    oldHead->prev = newTail;

    // set old tail to have next point to new tail
    oldTail->next = newTail;

    // return the tail
    return newTail;
}

// function to remove head of circular ll (dequeue)
// return the tail of the ll
Node * removeHead(Node * oldTail, Node * oldHead)
{
    // case for if list empty (it shouldn't be)
    if(oldTail == NULL && oldHead == NULL){
        return NULL;
    }

    // case for if list has 1 node
    if(oldTail->next == oldTail && oldHead->next == oldHead){
        free(oldTail);
        oldTail = NULL;
        oldHead = NULL;
        return NULL;
    }

    // case for if list has more than 1 node
    // Store new head
    Node * newHead = oldHead->next;

    // set new head to have previous point to old tail
    newHead->prev = oldTail;

    // set new head to have next point to old head's next next
    newHead->next = oldHead->next->next;

    // delete the old head
    free(oldHead);

    // set the old tail to point to the new head
    oldTail->next = newHead;

    // return the old tail
    return oldTail;
}

// function to sort groups using merge sort
// sorts in order from least to greatest arrival time
void mergeSort(Group * arr, int len)
{
    if(len <= 1){
        return;
    }

    mergeSort(arr, len / 2);
    mergeSort(arr + len / 2, len - len / 2);

    Group * temp = (Group *)malloc(len * sizeof(Group));

    int frontPtr = 0;
    int backPtr = len / 2;
    int mergePtr = 0;

    while(mergePtr < len){
        if(frontPtr == len / 2){
            // Front half of merged array is empty
            temp[mergePtr] = arr[backPtr];
            mergePtr++;
            backPtr++;
        } else if(backPtr == len){
            // Back half of merged array is empty
            temp[mergePtr] = arr[frontPtr];
            mergePtr++;
            frontPtr++;
        } else{
            // Neither are empty, so compare the front of both halves and use smallest value
            if(arr[frontPtr].arrvl < arr[backPtr].arrvl){
                temp[mergePtr] = arr[frontPtr];
                mergePtr++;
                frontPtr++;
            } else{
                temp[mergePtr] = arr[backPtr];
                mergePtr++;
                backPtr++;
            }
        }
    }

    // Copy the merged array into the final array
    for(int i = 0; i < len; i++){
        arr[i] = temp[i];
    }
    // free dynamically-allocated memory
    free(temp);
}
