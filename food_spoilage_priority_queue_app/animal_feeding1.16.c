/*
    Caleb Gibson
    COP 3502C
    Section 4
    Assignment #7 Animal Feeding
    Professor Travis Meade
    due December 2, 2022
*/

#include <stdio.h>
#include <stdlib.h>

// define maximum absolute error tolerance, 10^-5
#define ERR_TOL     0.00001

// define empty shipment values
#define EMPTY       -1

// define macros for heap navigation
#define PARENT(x)   (((x)-1)/2)
#define LEFT(x)     (((x)*2)+1)
#define RIGHT(x)    (((x)+1)*2)

double prevGreatestSpoilDiff = 0;

// Struct and function prototypes
// struct for the Shipment
typedef struct Shipment {
    int startTime, spoilTime, originalNumMangoes;
    double curNumMangoes;
} Shipment;

// struct for the Heap
typedef struct ArrayList {
    int cap, size;
    Shipment * array;
} ArrayList;

// ArrayList functions
ArrayList * createArrayList(int);
void cleanArrayList(ArrayList *);
void append(ArrayList *, Shipment);

// Heap functions
void swap(ArrayList *, int, int);
int compare(ArrayList *, int, int);
void percolateUp(ArrayList *, int);
void percolateDown(ArrayList *, int);
void heapAdd(ArrayList *, Shipment);
void heapRemove(ArrayList *);
Shipment front(ArrayList *);

// Sorting function
void mergeSort(Shipment *, int);

// See if the guessed rate can work with the current shipments array
// Do update(ArrayList *, int, int, double) within this canDo(Shipment *, double, int) function
int canDo(Shipment *, double, int);

// Absolute value function for doubles
double absDouble(double);

int main()
{
    int numShipments = 0, totOrigNumMangoes = 0;

    // read in the number of shipments
    scanf("%d", &numShipments);

    // Create an array for all the shipments, to be sorted in ascending order by the shipment's arrival time
    Shipment * shipments = (Shipment *)malloc(numShipments * sizeof(Shipment));

    // Read in the input data for the shipments
    for (int i = 0; i < numShipments; i++){
        scanf("%d%d%d", &shipments[i].startTime, &shipments[i].spoilTime, &shipments[i].originalNumMangoes);
        shipments[i].curNumMangoes = (double)shipments[i].originalNumMangoes;

        // Update the original fastest possible rate
        totOrigNumMangoes += shipments[i].originalNumMangoes;

        // Set the original least tight spoil range
        if(shipments[i].spoilTime - shipments[i].startTime > prevGreatestSpoilDiff) {
            prevGreatestSpoilDiff = shipments[i].spoilTime - shipments[i].startTime;
        }
    }

    // Sort the shipments array in ascending order by the shipment's arrival time
    mergeSort(shipments, numShipments);

    // Binary search to find the appropriate consumption rate
    double low = 0;
    double up = totOrigNumMangoes;
    double solution;
    int solutFound = -1;

    while(solutFound != 0){
        solution = (low + up)/2;

        solutFound = canDo(shipments, solution, numShipments);

        // Update the bounds, if necessary
        if(solutFound == -1){
            // rate too slow, increase lower bound
            low = solution;
        } else if(solutFound == 1){
            // rate too fast, reduce upper bound
            up = solution;
        } else{
            // rate within tolerance, result found!
            break;
        }
    }

    // Print result
    printf("%.6lf\n", solution);

    // Clean up any memory
    free(shipments);
    shipments = NULL;

    return 0;
}

// Function to check if the currently guessed consumption rate is acceptable
// If returning -1, it means the guessed rate is too low and the lower bound should be heightened
// If returning 0, it means the guessed rate is within acceptable margins for error and is therfore effective
// If returning 1, it means the guessed rate is too high and the upper bound should be reduced
int canDo(Shipment * shipments, double rate, int total)
{
    int valid = 0;
    int isTightestSpoilRange = 0;
    double greatestSpoilDifference = 0; // to be updated as you go through the heap; if the difference between this and the previous greatest spoil difference is greater than the tolerance times the rate, then if the greatest spoil difference is smaller than the previous one, then speed up the rate, but if it is not, then slow down the rate, otherwise, we are converging on a solution and it is close enough to within error tolerance that it can be used, so return it as the acceptable solution.
    double leastSpoilRange = 0;
    double currentSpoilRange = 0;
    double currentSpoilTime = 0;
    double tightestIdleTime = 0;
    double curTime = 0;
    double timeOfFinish = 0;
    double timeLeft = 0;
    double leastTimeLeftOver = 0;

    // Create a heap
    ArrayList * Heap = createArrayList(total);

    // Special case the first shipment
    // In the case of the first shipment, the arrival time is not impacted by any of the previous shipments.
    // Consumption begins immediately as the shipment is arriving. No heap updating needs to be done with it.
    // Just need to check to make sure that it can be done before spoiling with the given consumption rate.
    heapAdd(Heap, shipments[0]);
    leastSpoilRange = (double)(shipments[0].spoilTime - shipments[0].startTime)/shipments[0].originalNumMangoes;
    timeLeft = shipments[0].curNumMangoes/rate;

    // If the very first shipment is invalid, no need to do anything more
    if(timeLeft > shipments[0].spoilTime - shipments[0].startTime){
        // rate is too slow!!!
        valid = -1;
        // clean any dynamically allocated memory
        cleanArrayList(Heap);
        // return based on the validity of the check
        return valid;
    }

    // Set the first least time left over
    currentSpoilTime = shipments[0].spoilTime - timeLeft;

    curTime = shipments[0].startTime;

    for(int i = 1; i < total; i++) {
        // Update the total idle time initially in the for loop
        currentSpoilRange = (double)(front(Heap).spoilTime - front(Heap).startTime)/front(Heap).originalNumMangoes;
        if(currentSpoilRange < leastSpoilRange) {
            leastSpoilRange = currentSpoilRange;
        }
    }

    // Loop through the remaining shipments
    for(int i = 1; i < total; i++){
        // Update the heap based on the time of the current shipment
        //---------------------------------------------------------------------------
        // Keep track of the current time
        // Loop while there is some value in the heap
            // Determine the time required to finish consuming the current shipment
            // Determine the time when the shipment would finish consumption
            // Check if we cannot finish the shipment before spoiling
            // Check if we can finish the shipment before the end of the update
                // Update time and remove from heap
            // Otherwise
                // Update the remaining size of the shipment and stop the simulation
        while(Heap->size > 0){
            // Determine the time required to finish consuming the current shipment
            timeLeft = front(Heap).curNumMangoes/rate;

            // Determine if this is the shipment with the tightest spoil range
            currentSpoilRange = (double)(front(Heap).spoilTime - front(Heap).startTime)/front(Heap).originalNumMangoes;
            if(absDouble(currentSpoilRange - leastSpoilRange) < ERR_TOL) {
                isTightestSpoilRange = 1;
            }
            else {
                isTightestSpoilRange = 0;
            }

            // Determine the time when the shipment would finish consumption
            timeOfFinish = timeLeft + curTime;

            // Check if can finish consumption before the shipment spoils
            if(timeOfFinish - front(Heap).spoilTime > ERR_TOL * rate) {
                // Rate is too slow!!!
                valid = -1;
                // Clean any dynamically allocated memory
                cleanArrayList(Heap);
                // Return based on the validity of the check
                return valid;
            }

            // Update the greatest spoil difference, if necessary
            if(front(Heap).spoilTime - timeOfFinish > greatestSpoilDifference) {
                greatestSpoilDifference = front(Heap).spoilTime - timeOfFinish;
            }

            // Check if we can finish the shipment before the end of the update
            if(curTime + timeLeft <= shipments[i].startTime) {
                // Update time and remove from heap
                // Process the front and add the incoming shipment to the heap so it can be processed
                // Case for if the incoming shipment starts before the current one finishes
                Heap->array[0].curNumMangoes = 0;
                curTime += timeLeft;    // update the current time

                // If it's not the most jammed-in shipment time, then don't need to consider the leftover time as extra idle time
                // IF IT IS the most jammed-in shipment time, then do need to consider the leftover time as extra idle time
                if(isTightestSpoilRange) {
                    tightestIdleTime = front(Heap).spoilTime - curTime;
                }

                heapRemove(Heap);
            }
            // Otherwise
            else {
                // Update the remaining size of the shipment and stop the simulation
                // The current front will not finish processing before the current shipment spoils. Therefore, update the current number of mangoes
                // for the current front of the array, add the current shipment to the Heap, and process the front. If the processing time for the
                // new front is greater than its spoil time, then we have an invalid solution and we return negative one. Otherwise, we process the front
                Heap->array[0].curNumMangoes -= rate * (shipments[i].startTime - curTime);
                curTime = (double)shipments[i].startTime;
                break;
            }

            // Update the current time, if necessary
            if((Heap->size > 0) && (front(Heap).startTime > curTime)) {
                curTime = front(Heap).startTime;
            }
        }

        // Add the newest shipment into the heap.
        heapAdd(Heap, shipments[i]);

        // Update the current time, if necessary
        if(front(Heap).startTime > curTime) {
            curTime = front(Heap).startTime;
        }
    }

    // Process any remaining elements in the heap
    while(Heap->size > 0) {
        // Determine if this is the shipment with the tightest spoil range
        currentSpoilRange = (double)(front(Heap).spoilTime - front(Heap).startTime)/front(Heap).originalNumMangoes;
        if(absDouble(currentSpoilRange - leastSpoilRange) < ERR_TOL) {
            isTightestSpoilRange = 1;
        }
        else {
            isTightestSpoilRange = 0;
        }

        // Update the current time if the front shipment has a greater start time than it.
        if(front(Heap).startTime >= curTime) {
            curTime = (double)front(Heap).startTime;
        }

        // Check that we can finish consuming the shipment before spoiling
        timeLeft = front(Heap).curNumMangoes/rate;
        currentSpoilTime = timeLeft + curTime;

        if(currentSpoilTime - front(Heap).spoilTime > ERR_TOL * rate) {
            // Rate is too slow!!!
            valid = -1;
            // Clean any dynamically allocated memory
            cleanArrayList(Heap);
            // Return based on the validity of the check (done outside of and after this WHILE block)
            return valid;
        }

        curTime += timeLeft;

        // Update the greatest spoil difference if necessary
        if(front(Heap).spoilTime - curTime > greatestSpoilDifference) {
            greatestSpoilDifference = front(Heap).spoilTime - curTime;
        }

        if(isTightestSpoilRange) {
            tightestIdleTime = front(Heap).spoilTime - curTime;
        }
        heapRemove(Heap);
    }

    // Check if the difference between the greatest spoil difference and the previous one is beyond tolerance levels
    if(absDouble(greatestSpoilDifference - prevGreatestSpoilDiff) > ERR_TOL) {
        if(greatestSpoilDifference < prevGreatestSpoilDiff) {
            // Rate is too fast!!!
            valid = 1;
        }
        else {
            // Rate is too slow!!!
            valid = -1;
        }
        // Update the previous greatest spoil difference
        prevGreatestSpoilDiff = greatestSpoilDifference;
    }

    // Then, the rate is just right.
    if(valid != 1 && valid != -1){
        valid = 0;
    }

    // Clean any remaining dynamically allocated memory!
    cleanArrayList(Heap);

    // Return based on the validity of the return (the guessed rate)
    return valid;
}

// Function to create an ArrayList
ArrayList * createArrayList(int initialSize)
{
    ArrayList * result = (ArrayList *)malloc(sizeof(ArrayList));
    if(result == NULL){
        return NULL;
    }
    result->cap = initialSize;    // initially able to hold 4 shipments
    result->size = 0;
    result->array = (Shipment *)malloc(initialSize * sizeof(Shipment));
    if(result->array == NULL){
        return NULL;
    }
    return result;
}

// Function to destroy an ArrayList
void cleanArrayList(ArrayList * arrList)
{
    free(arrList->array);
    arrList->array = NULL;
    free(arrList);
    arrList = NULL;
}

// Function to add to the end of an ArrayList
void append(ArrayList * arrList, Shipment toAppend)
{
    // If not within capacity, double the cap and reallocate enough space for the shipment going into the shipments array
    if (arrList->cap == arrList->size){
        arrList->cap *= 2;
        arrList->array = (Shipment *)realloc(arrList->array, sizeof(Shipment) * arrList->cap);
        if(arrList->array == NULL){
            return;
        }
    }

    // Add the element to the back of the ArrayList
    arrList->array[arrList->size] = toAppend;

    // Increment the size
    arrList->size++;

//    printf("Shipment with %d %d %d appended...\n", arrList->array[arrList->size-1].startTime, arrList->array[arrList->size-1].spoilTime, arrList->array[arrList->size-1].originalNumMangoes);
}

// Function to swap two indices of an ArrayList
void swap(ArrayList * heap, int index1, int index2)
{
    // Make sure the ArrayList has been created
    if (heap == NULL){
        return;
    }

    // Create the temporary variable to store the data for the index being swapped
    Shipment temp;
    temp = heap->array[index1];
    // Do the swap
    // index 1 -> index 2:
    heap->array[index1] = heap->array[index2];
    // index 2 -> index 1:
    heap->array[index2] = temp;
}

// A function to see what two spoil times are in relation to one another; if negative, then index1 has a lower spoil time than index2, and vice versa
int compare(ArrayList * heap, int index1, int index2)
{
    return heap->array[index1].spoilTime - heap->array[index2].spoilTime;
}

// A function to move a node up the heap, a.k.a. to "percolate" it up
void percolateUp(ArrayList * heap, int index)
{
    // Make sure the call is valid: make sure that the heap is created and the index lies within the heap
    if (heap == NULL || index > heap->size){
        return;
    }

    // Percolate node up the tree
    int parIndex = PARENT(index);
    // make sure the parent index lies within the heap
    if (parIndex < 0){
        return;
    }
    if (compare(heap, index, parIndex) < 0){
        swap(heap, index, parIndex);
        percolateUp(heap, parIndex);
    }
}

void percolateDown(ArrayList * heap, int index)
{
    // Make sure the call is valid: make sure that the heap is created and the index lies within the heap
    if (heap == NULL || index > heap->size){
        return;
    }

    int bestIndex = index;
    int rightIndex = RIGHT(index);
    int leftIndex = LEFT(index);

    // Check if right child is a better index than the "best"
    if (rightIndex < heap->size && compare(heap, bestIndex, rightIndex) > 0) {
        bestIndex = rightIndex;
    }

    // Check if left child is a better index than the "best" (which is now the right, if the initial "if" statement executed)
    if (leftIndex < heap->size && compare(heap, bestIndex, leftIndex) > 0) {
        bestIndex = leftIndex;
    }

    // If the "best" index is not the current index, then percolate down the heap
    if (index != bestIndex) {
        swap(heap, bestIndex, index);
        percolateDown(heap, bestIndex);
    }
}

// A function to add a shipment to the heap in the correct spot in the heap
void heapAdd(ArrayList * heap, Shipment shipment)
{
    // Make sure the heap has been created
    if (heap == NULL){
        return;
    }
    // Add the shipment to the heap
    append(heap, shipment);
    // Move it up into the right place
    percolateUp(heap, heap->size - 1);  // (heap size - 1) because arrays are 0-indexed but the size is made to be "1" with 1 element in it
}

// A function to remove the top of the heap and re arrange the heap to be of the correct arrangement
void heapRemove(ArrayList * heap)
{
    // Make sure the heap has been created and contains at least 1 element
    if (heap == NULL || heap->size == 0){
        return;
    }

    // Swap the value to be removed (root, top of the heap) with the left-most non-NULL value in the heap (bottom of heap)
    swap(heap, 0, heap->size - 1);

    /* Reallocate the heap to free the shipment from the end of it if the heap size is small enough
     Using realloc ended up being too dangerous and causing too many memory leaks despite many attempts to
     fix it, so instead I decided to simply change the way an index in the heap is denoted as empty (if it has
     already been used, that is). */
    Shipment emptyShip;
    emptyShip.curNumMangoes = EMPTY;
    emptyShip.startTime = EMPTY;
    emptyShip.spoilTime = EMPTY;
    emptyShip.originalNumMangoes = EMPTY;
    heap->array[heap->size - 1] = emptyShip;

    // Decrement the heap size
    heap->size--;

    // Move value originally at end of heap into correct position now that it has been swapped with former root and former root deleted
    percolateDown(heap, 0);
}

// A function to retrieve the top of the heap
Shipment front(ArrayList * heap)
{
    // verify that the heap has been created and contains at least 1 value
    if (heap == NULL || heap->size == 0 || heap->array == NULL){
        Shipment garbage;
        return garbage;
    }
    return heap->array[0];
}

// A generic merge sort function for the shipments array sorted by arrival time in ascending order
void mergeSort(Shipment * shipsArray, int len)
{
    // Base case for the recursion
    if (len <= 1){
        return;
    }

    // Create necessary variables
    int mid = len/2;
    int fptr = 0, bptr = mid, mergePtr = fptr;
    Shipment * mergedArray = (Shipment *)malloc(len * sizeof(Shipment));

    // Recursion call
    mergeSort(shipsArray, mid);
    mergeSort(shipsArray + mid, len - mid);

    // Merge the separate arrays back together in order
    while (mergePtr < len){
        // Case if front half fully merged into merged array (i.e. front half empty)
        if (fptr == mid){
            mergedArray[mergePtr] = shipsArray[bptr];
            mergePtr++;
            bptr++;
        }
        // Case if back half empty
        else if (bptr == len){
            mergedArray[mergePtr] = shipsArray[fptr];
            mergePtr++;
            fptr++;
        }
        // Case if neither half empty
        else{
            // Case if front pointer is pointing to greater value than back pointer; insert back value into merged array
            if (shipsArray[fptr].startTime > shipsArray[bptr].startTime){
                mergedArray[mergePtr] = shipsArray[bptr];
                mergePtr++;
                bptr++;
            }
            // Case if front pointer is pointing to lesser or equal value than back pointer; insert front value into merged array
            else {
                mergedArray[mergePtr] = shipsArray[fptr];
                mergePtr++;
                fptr++;
            }
        }
    }

    // Copy the contents of the merged array into the actual shipments array
    for (int i = 0; i < len; i++){
        shipsArray[i] = mergedArray[i];
    }

    // Free dynamically allocated memory
    free(mergedArray);
    mergedArray = NULL;
}

// Absolute value function for doubles
double absDouble(double val)
{
    if(val < 0){
        return val*-1;
    }
    return val;
}
