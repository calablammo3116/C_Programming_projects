/*
 * Caleb Gibson
 * COP3502
 * Professor Travis Meade
 * Assignment #5 Monkey Business
 * due W 11/9/2022
*/

#include <stdio.h>
#include <stdlib.h>

// Struct prototypes and definitions
typedef struct Node
{
    int key, data;
    struct Node * parent;
} Node;

typedef struct Stack
{
    Node * tail;
} Stack;

// Function prototypes
void push(Stack * stack, int ID, int data);
void pop(Stack * stack);
Node * peek(Stack * stack);
void cleanStack(Stack * stack);

// Function definitions
// main function
int main()
{
    int numExhibits;

    // Read in the number of exhibits.
    scanf("%d", &numExhibits);

    // Create the stack for storing the tree.
    Stack * exhibitStack = (Stack *)malloc(sizeof(Stack));
    exhibitStack->tail = NULL;

    // Create the array of parents.
    int parentsNM[numExhibits]; // "NM" = number of monkeys

    // Read in the data

    // Read in each exhibit
    // Reference each index of the parents array using the exhibit's ID minus 1.
    int ID, nM; // ID = exhibit identifier, nM = "number of monkeys"
    int inputCt = numExhibits * 2 - 2;  // Keeps track of the number of inputs we have taken so far
    int totMInNode; // totMInNode = "total monkeys in Node" (total monkeys seen by guests on their way to the exhibit Node)
    // Read in the first exhibit
    scanf("%d%d", &ID, &nM);
    push(exhibitStack, ID, nM);
    parentsNM[0] = nM;
    // Read in the rest of the exhibits.
    for (int i = 1; i <= inputCt; i++) {
        // Read in the command.
        // If the command is -1, then pop the element from the top of the stack.
        scanf("%d", &ID);
        if (ID == -1) {
            pop(exhibitStack);
        }
        // If the command is NOT -1, then it's an ID.
        // So, we: 1) increment the input counter, 2) read in the number of monkeys, 3) make the node's total monkeys count (in the "data"
        // field of the new node) equal to the number of monkeys in the current top of the stack PLUS the number of read-in monkeys, 4) push
        // the node to the top of the stack, 5) store the total monkeys count in the (ID - 1)-th index of the "parentsNM" integer array.
        else
        {
            scanf("%d", &nM);
            if (peek(exhibitStack) == NULL) {
                push(exhibitStack, ID, nM);
                parentsNM[ID - 1] = peek(exhibitStack)->data;
            } else {
                totMInNode = peek(exhibitStack)->data + nM;
                push(exhibitStack, ID, totMInNode);
                parentsNM[ID - 1] = totMInNode;
            }
        }
    }

    // Read in the number of visitors
    int nVistors;
    scanf("%d", &nVistors);
    // Next, we have to read in which monkey exhibit each visitor is going to.
    // Create the array for storing each "vistor's" monkey exhibit that they are going to.
    int vistorsExhibits[nVistors];
    int nMSeenByVistors[nVistors];
    // Read in each "vistor's" exhibit that they are going to and count the number of monkeys they will see, and then store it into the
    // "nMSeenByVistors" integer array. The number of monkeys they will see is represented by ((the ID of the exhibit they are reading in) - 1)-th
    // index of the "parentsNM" integer array.
    for (int i = 0; i < nVistors; i++) {
        scanf("%d", &vistorsExhibits[i]);
        nMSeenByVistors[i] = parentsNM[vistorsExhibits[i] - 1];
    }
    // Print the total number of monkeys that each "vistor" will see on their way through the park.
    for (int i = 0; i < nVistors; i++) {
        printf("%d\n", nMSeenByVistors[i]);
    }

    // Free all dynamically-allocated memory
    cleanStack(exhibitStack);
    free(exhibitStack);

	return 0;
}

// Function to push a node onto the stack.
void push(Stack * stack, int ID, int data)
{
    // Case if stack has no nodes in it.
    if (stack == NULL || stack->tail == NULL) {
        Node * pushNode = (Node *)malloc(sizeof(Node));
        pushNode->key = ID;
        pushNode->data = data;
        pushNode->parent = NULL;
        // Update the stack tail.
        stack->tail = pushNode;
        return;
    }
    // Create the new node to be pushed onto the top of the stack.
    Node * pushNode = (Node *)malloc(sizeof(Node));
    pushNode->key = ID;
    pushNode->data = data;
    pushNode->parent = stack->tail;
    // Update the stack tail.
    stack->tail = pushNode;
    return;
}

// Function to pop a node off the top of the stack.
void pop(Stack * stack)
{
    // Case if stack is empty (it shouldn't be).
    if (stack->tail == NULL) {
        return;
    }
    // Case if stack has 1 node left in it.
    if (stack->tail->parent == NULL) {
        free(stack->tail);
        stack->tail = NULL;
        return;
    }
    // Case if stack has more than 1 node left in it.
    // Store the new tail.
    Node * newTail = stack->tail->parent;

    // Free the old tail.
    free(stack->tail);

    // Set the stack tail to point to the new tail and then return.
    stack->tail = newTail;
    return;
}

// Function to look at the top of the stack WITHOUT manipulating it.
Node * peek(Stack * stack)
{
    // Since cannot dereference NULL, must check to make sure that stack is not NULL first.
    if (stack == NULL) {
        return NULL;
    }
    return stack->tail;
}

// Function to delete the whole stack.
void cleanStack(Stack * stack)
{
    // Case if stack is empty (it shouldn't be).
    if (stack == NULL) {
        return;
    }
    // Case if stack is not empty.
    while (stack->tail != NULL) {
        pop(stack);
    }
}
