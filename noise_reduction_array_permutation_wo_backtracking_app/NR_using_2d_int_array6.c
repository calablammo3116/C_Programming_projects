/* COP 3502 - 0004
   Assignment 3 -- Noise Reduction
   Professor Travis Meade
   Caleb Gibson
   due Friday, 10/14/2022.
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void permute(int*, int*, int, int);
int sum(int, int*);

int ct = 0;
int min_sum = 2147483647; // maximum value for an integer in C according to https://learn.microsoft.com/en-us/cpp/c-language/cpp-integer-limits?view=msvc-170
int exhibits[11][11];   // create an integer "exhibits_array" array of arrays for storing all the Exhibits that will be read in from the input.

int main(void)
{
    int number_of_exhibits, i, j;

    // read in the number of exhibits
    scanf("%d", &number_of_exhibits);

    // create variables whose size depends on the number of exhibits
    int arr[number_of_exhibits];    // a temporary integer array for storing the current permutation of the Exhibits' initial positions,
                                    // which are stored as integers from 1 to "N," where "N" represents the total number of exhibits being
                                    // read in as input
    int used[number_of_exhibits];   // a temporary array for marking which indexes of the temporary integer "arr" array have already been used
                                    // in the current permutation. It is necessary only for the void "permute()" function, which finds and
                                    // stores all the possible permutations of Exhibits' initial positions (i.e. row) arrangements and then,
                                    // at the end of each permutation, sums all the noises for that permutation

    for(i=0; i<number_of_exhibits; i++) {
        // set the initial value of the i-th index of integer "used" array to denote an "empty" index, by making it negative 1.
        used[i] = -1;

        // set the initial value of the i-th index of the integer "arr" array to denote an "empty" index too, by making it negative 1.
        arr[i] = -1;

        // read in the noise values for the i-th index of the 2D integer "exhibits" array of arrays.
        for(j=0; j<number_of_exhibits; j++)
        {
            scanf("%d", &exhibits[i][j]);
        }
    }

    // find all possible permutations of the exhibits' initial positions in an integer array "arr" of size integer "number_of_exhibits."
    // Each permutation of the integer array "arr" representing one possible ordered arrangement (i.e. one possible permutation) of the
    // exhibits' initial positions. The integer array "used" is used to keep track of the indexes in the integer array "arr" that are already
    // used during the "permute()" function's finding of each current permutation.
    permute(arr, used, number_of_exhibits, number_of_exhibits);

    // now that the least sum has been found, print it to the screen, as required.
    printf("%d\n", min_sum);

    return 0;
}

void permute(int * array, int * used, int number_of_perm_elements_to_do, int num_elements)
{
    // base case. Integer "number_of_perm_elements_to_do" equaling 0 is a lie. If this is the case, then we must be at the last branch of the
    // permutation function tree. This will mean that a new permutation has been found. Add the numbers of the permutation into the 2nd-level indexes
    // index of the integer "array_of_options" array of arrays for the integer "ct"-th (integer "ct" means "count," referring to the total count
    // count of the total number of permutations) index in the first-level array of the integer "array" array for the current permutation.
    if (number_of_perm_elements_to_do == 0)
    {
        int current_sum = 0;
        current_sum = sum(num_elements, array);

        if(min_sum > current_sum) {
            min_sum = current_sum;
        }

        return;
    }

    // create an integer variable "position_in_permutation" to keep track of where we are in each permutation. For example, the 0-th
    // character, the first character, the second character, and so on and so forth. This should be the difference between the integers
    // "num_elements" representing the total number of rows (i.e. exhibits) in the initial 2D integer "exhibits" array of arrays
    // and the "number_of_perm_elements_to_do" integer, representing how many elements we have left to do in the current calculation of
    // the current permutation.
    int position_in_permutation = num_elements - number_of_perm_elements_to_do;

    for(int i = 0; i < num_elements; i++)
    {
        // see if current index in integer "array" array for current permutation is already used by seeing if the value in
        // the integer "used" array does not mean an empty index. In this case, the value used to mean an empty index is zero, so check to see
        // if the i-th index in the integer "used" array is equaling -1. If it is, then we're good. If it is not, then continue on to the
        // next iteration in the "number in the i-th index of the second-level array of the integer 'array' array for current permutation"
        // generating loop.
        if(used[i] != -1) {
            continue;
        }
        // if the loop makes it here, then the next index of array in the integer "array" array for the current permutation of rows is open for
        // number generation

        // make the index used now
        used[i] = 1;

        // make another number here in the temporary integer "array" array storing the Exhibits' initial positions in the current permutation
        array[position_in_permutation] = i;

        permute(array, used, number_of_perm_elements_to_do - 1, num_elements);

        // un-make element of array being "used," that way it can be used in the next iteration of integer "i." Also empty out the temporary
        // integer "array" array so that it can be used for another permutation.
        array[position_in_permutation] = -1;
        used[i] = -1;
    }
}

// TO-DO fix the integer function "sum()"'s output; it is outputting the wrong value
int sum(int dimensions, int * array_perm)
{
    int n_sum = 0;

    // collect the noise sums for each row (i.e. each animal's noise output when it is antagonized) in the 2D integer "exhibits" array of
    // arrays representing the exhibits' initial positions and the noise outputs in each exhibit
    for (int i=0; i<dimensions; i++) {
        // Check left of the current exhibit in the array permutation represented by "array_perm"
        // Rows potentially below the current "i"-th row in the integer "array_perm" array, which represents the order of the rows in the
        // current permutation that is being analyzed for that permutation's total noise sum output.
        // do the case for a row that is two rows above the "i"-th one (if the noises are visualized as a table of noise outputs, with
        // "animals" representing rows and their "noises" in the columns) in the current permutation.
        if(i > 1) {
                n_sum += exhibits[array_perm[i]][array_perm[i-2]];
        }
        // do the case for a row that is one row above the "i"-th one (if the noises are visualized as a table of noise outputs, with
        // "animals" representing rows and their "noises" in the columns) in the current permutation.
        if(i > 0) {
                n_sum += exhibits[array_perm[i]][array_perm[i-1]];
        }

        // Check right of the current exhibit in the array permutation represented by "array_perm"
        // Rows potentially above the current "i"-th row in the integer "array_perm" array, which represents the order of the rows in the
        // current permutation that is being analyzed for that permutation's total noise sum output.
        // do the case for a row that is two rows below the "i"-th one in the current permutation
        if((i+2) < dimensions) {
                n_sum += exhibits[array_perm[i]][array_perm[i+2]];
        }

        // do the case for a row that is one row below the "i"-th one in the current permutation
        if((i+1) < dimensions) {
                n_sum += exhibits[array_perm[i]][array_perm[i+1]];
        }

        if(n_sum > min_sum) {
            break;
        }
    }

    return n_sum;
}
