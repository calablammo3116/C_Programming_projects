/******************************************************************************

                            Online C Compiler.
                Code, Compile, Run and Debug C program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <time.h>   //this so we have access to the "clock()" function, so we can find how long it takes for the program to execute.

int main()
{
    int max_input_len;
    char * user_input;

    fflush(stdin);  //clear (a.k.a. flush) the input buffer

    printf("What is the max number of characters you think you need for your input?\nPlease hit enter once you're done:  ");
    scanf("%d", &max_input_len);

    fflush(stdin);  // clear (a.k.a. flush) the input buffer

    user_input = (char*)malloc(sizeof(char) * (max_input_len + 1));    /* allocate space in memory for the 
                                                                          "user_input" character array (a.k.a. the "user_input" string); 
                                                                          account for the NULL terminator '\0' using + 1 */
    if(user_input == NULL){ // Catch the error if memory allocation fails
      printf("Memory allocation failed, exiting program...\n");
      return 0;
    }

    printf("\nWhat input would you like to see if it's a palindrome?\nHit enter once you've finished typing it:\n");
    scanf("%[^\n]s", user_input);

    if(is_palindrome(user_input) == 1)
    {
        fputs("Your input is a palindrome!\n",stdout);
    }
    else
    {
        fputs("Your input is not a palindrome. :(\n",stdout);
    }

    fflush(stdin);  //clear (a.k.a. flush) the input buffer

    free(user_input);   //free the memory space taken up by the character pointer "user_input"
    user_input = NULL;

    return 0;
}


int is_palindrome(char * input)
{
    int i = 0, j = 0, input_length = 0, ans = 0;  // ans = answer to whether is palindrome or not - 0 means "no," 1 means "yes"
    double time_taken;  //double to store the time it takes
    clock_t begin, end;

    begin = clock();

    ans = 0; //default value for "ans," which is the answer to the question, "Is the input a palindrome?" is true (which, since C does NOT have boolean data types, is represented as the number 1)
    for(input_length = 0; input[input_length] != '\0'; input_length++)
   {
        printf("Character in input[%d]: %c\n", input_length, input[input_length]);
    }
  
    //word_length should now be equal to the length of the input
    // could have simply done strlen(input), but this way works too.
  
    printf("\nInput length: %d\n", input_length);  //test of input_length counter
                                                
    j = input_length;

    for(i=0;i<input_length;i++)
    {
//        printf("Character i in input[i=%d]: %c\nCharacter j in input[j=%d]: %c\n",i,input[i],j,input[j]);   //This line here for testing purposes

        if(i == j)
        {
            break;
        }
        else if(input[i] == input[j])
        {
            j--;
            continue;
        }
        else
        {
            ans = 0;
            break;
        }
    }

    end = clock();

    time_taken = (end - begin)/CLOCKS_PER_SEC;  //calculate the total time taken to run the function by taking the difference of the end and the beginning, then dividing it by "CLOCKS_PER_SEC" to convert that time difference to seconds (since "end" and "begin" are given in terms of the amount of ticks on the computer's CPU's clock)

    printf("The is_palindrome() function took %3.7lf seconds to excute.\n", time_taken);

   return ans;
}
