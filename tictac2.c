#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <libgen.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <errno.h>
#include <getopt.h>
#include <sys/types.h>

//Declare functions
int set_up_board(int rows_or_cols, char* board)
{
int i;

for(i=0;i<rows_or_cols*rows_or_cols;i++)
{
    board[i] = 0x20;
}

}

char choose_character(void)
{
    char* char_choice;

    puts("Hello! Welcome to Tic-Tac-Toe. Here, you will play as either Xs or Os, and the computer will play as the opposite as you.\n");
    puts("Would you like to play as Xs or Os?\nType the letter you would like to play as. Then hit enter: ");
    scanf("%c", &char_choice);
    //fgets(char_choice,1,stdin);

    //&char_choice

/*     while((char_choice != 1) && (char_choice != 2))
 *     {
 *         puts("Whoops, that was an invalid choice! Please type 1 for playing as Xs or 2 for playing as Os.\n");
 *         fgets(char_choice,1,stdin);
 *     }
 */

/*     if((char_choice == 0x78) || (char_choice == 0x58)
 *     {
 *         fputs("You will play as Xs!\n",stdout);
 *     }
 *     else if((char_choice==0x6F)||(char_choice==0x4F)
 *     {
 *         fputs("You will play as Os!\n",stdout);
 *     }
 *     else
 *     {
 */
    printf("You will play as %c!\n",char_choice);
//    }

    return char_choice;
}


void gdisplay(char* board, int num_rows_or_cols)
{
int i,j;
for(i=0;i<num_rows_or_cols;i++)
{
    //First, print the rows containing actual values from the table indices (e.g. X, O, etc.);
    // be sure to include the little vertical line separations in between each index
    for(j=0;j<num_rows_or_cols;j++)
    {
        //If you're at any index before the last index in the row, be sure to print the character filling the index,
        // and then followed by the vertical line separator to separate it between it and the other indices next to it
        if(j<num_rows_or_cols-1)
        {
            printf("%c|",board[i*num_rows_or_cols+j]);
        }
        //If you're at the last index, be sure to not print the vertical line separator after the character stored on the board,
        // and instead print a newline character to go to the next line, so you can start printing hyphens (horizontal lines)
        // as separators between that row and the next
        else
        {
            printf("%c\n",board[i*num_rows_or_cols+j]);
        }
    }
    //If you're at a row before the very last one, make a horizontal line separating it from the next one
    if(i<num_rows_or_cols-1)
    {
        for(j=0;j<num_rows_or_cols;j++)
        {
            if(j<num_rows_or_cols-1)
            {
                fputs("--",stdout);
            }
            else
            {
                fputs("-",stdout);
            }
        }
        fputs("\n",stdout);
    }
    //If you're at the last row, then there's no need to print another horizontal line below it separating it from the next row
    // (and frankly, you're not supposed to do that on an actual Tic - Tac - Toe board). So, just print a newline character
    // to go to the next line easy-peasy
    else
    {
        printf("\n",board[num_rows_or_cols*(i+1)-1]);
    }
}
return;
}

void place_marker(int player_choice_row, int player_choice_col, int num_rows_or_cols, char char_choice, char* board)
{
    int desired_index_to_alter;

    //Reference the appropriate index on the 1D tic-tac-toe board. This was tough to figure out, but after doing the math in my head I *think* it should work.
    // This method of index-referencing is designed to work iff the player uses the base-1 index numbering sys., instead of base-0 (like what the computer uses for index-referencing).
    desired_index_to_alter = (player_choice_row-1)*num_rows_or_cols+player_choice_col-1;

    board[desired_index_to_alter] = char_choice;

    return;
}

int win_check(char* board, char player_char_choice, char computer_char_choice, int num_rows_or_cols)
{
    int win_value=0,i,j;

    //Check to see if there's a horizontal win case
    for(i=0;i<num_rows_or_cols;i++)
    {
        for(j=0;j<num_rows_or_cols;j++)
        {
            if(j<num_rows_or_cols-1)
            {
                //If you're still not at the last index of the row and the next index is not equal to the current index,
                // then just break out of the loop, since it's clear that no one won with the horizontal row method
                if(board[i*num_rows_or_cols+j]!=board[i*num_rows_or_cols+j+1])
                {
                    break;
                }
                //If every index in the row on the board (not the display board) IS equal to each other,
                // then check to see whose choice it is: the player's, or the computer's
                else
                {
                    //Check to see if the player won
                    if(board[i*num_rows_or_cols+j]==player_char_choice)
                    {
                        //If the player did win, then set the win_value to "1" and return the win_value
                        win_value = 1;
                        return win_value;
                    }
                    else if(board[i*num_rows_or_cols+j]==computer_char_choice)
                    {
                        //Otherwise, if the player didn't win, then since it's a binomial game then the computer must've won.
                        // So, set the win_value to "2" and return the win_value
                        win_value = 2;
                        return win_value;
                    }
                    else
                    {
                        continue;
                    }
                }
            }
        }
        //Check to see if there's a vertical win case
        for(i=0;i<num_rows_or_cols;i++)
        {
            for(j=0;j<num_rows_or_cols-1;j++)
            {
                //If you're at any index before the last one of a vertical column,
                // and the next index in the vertical column doesn't equal that one's,
                // then just break out of the loop, since it's clear that no one won
                // with that vertical column
                if(board[i+j*num_rows_or_cols]!=board[i+(j+1)*num_rows_or_cols])
                    {
                        break;
                    }
                //If you are at the index before the last one AND that index EQUALS the next index
                // (i.e. the last one), then we have a winner! Check to see who it is,
                // and then return their win_value (2 if it is the computer who won,
                // and 1 if it is the player who won)
                else if((j+1)==(num_rows_or_cols-1))
                {
                    if(board[i+j*num_rows_or_cols]==player_char_choice)
                    {
                        win_value=1;
                        return win_value;
                    }
                    else if(board[i+j*num_rows_or_cols]==computer_char_choice)
                    {
                        win_value=2;
                        return win_value;
                    }
                    else
                    {
                        continue;
                    }
                }
                else
                {
                    continue;
                }
            }
        }
        //Check to see if there's a diagonal win case (top left to bottom right here first)
        for(i=0;i<num_rows_or_cols-1;i++)
        {
            //If you're not at the last index (which you shouldn't be, since "i" is set to be only less than
            // the number of rows or columns minus 1, i.e. it can only go up to the 2nd to last index),
            // then check to see if the next index on the board diagonal down right from the current one is equal to the current one,
            // and if it isn't then just break, since you definitely don't have a winner here then
            if(board[i*num_rows_or_cols+i]!=board[(i+1)*num_rows_or_cols+i+1])
            {
                break;
            }
            //If you're at the 2nd to last index and the next index diagonally down and to the right
            // of your current one is EQUAL to your current one, then since you've already passed the check
            // for all the other previous indice pairs, then we have a winner! Check to see whether it is the player
            // or the computer, and then return their win_value.
            // ACKSHYUALLY, there could be case where the whole row is just empty, so check to make sure that if the win
            // condition is confirmed, that it is either 1) the player, or 2) the computer that won
            else if((i==(num_rows_or_cols-2))&&(board[i*(num_rows_or_cols+1)]==board[(i+1)*num_rows_or_cols+i+1]))
            {
                if(board[i*(num_rows_or_cols+1)]==player_char_choice)
                {
                    win_value=1;
                    return win_value;
                }
                else if(board[i*(num_rows_or_cols+1)]==computer_char_choice)
                {
                    win_value=2;
                    return win_value;
                }
                else
                {
                    continue;
                }
            }
            //In the case that you're not at the last index, but the next index in the vertical column still matches yours (the current one),
            // simply continue the loop
            else
            {
                continue;
            }
        }
        //Check to see if there's a diagonal win case (bottom left to top right this time around)
        for(i=num_rows_or_cols;i>0;i--)
        {
            if(board[i*num_rows_or_cols+i]!=board[(i-1)*num_rows_or_cols+i-1])
            {
                break;
            }
            else if((i==1)&&(board[i*num_rows_or_cols+i]==board[(i-1)*num_rows_or_cols+i-1]))
            {
                if(board[i*num_rows_or_cols+i]==player_char_choice)
                {
                    win_value=1;
                    return win_value;
                }
                else if(board[i*num_rows_or_cols+i]==computer_char_choice)
                {
                    win_value=2;
                    return win_value;
                }
                else
                {
                    continue;
                }
            }
            //Otherwise, simply continue the loop
            else
            {
                continue;
            }
        }
    }


    return win_value;
}


//Now, enter the main function
int main(int* argc, char** argv)
{
//system("chcp 65001");
int player_marker_placement_choice, winner, num_spaces, rows, cols, player_choice_row, player_choice_col, computer_index_choice, i, j;
char char_choice, computer_choice;
char* tic_tac_toe_board;


printf("How many rows would you like to have? The amount of columns will be the same as the amount of rows.\n");
scanf("%d", &rows);
cols = rows;    // even though cols is the same value as rows, it helps with readability to have two separate variables for them

num_spaces= rows*cols;

tic_tac_toe_board = (char *)malloc( sizeof(char) * num_spaces);

//seed the random number generator with the time function
srand(time(0));

//the octal ASCII value for x is 170
//the octal ASCII value for o is 157
//the octal ASCII value for X is 130
//the octal ASCII value for O is 117

char_choice=choose_character();

if((char_choice==0x58)||(char_choice==0x78))
{
    computer_choice = 0x4F;
    fputs("Since you picked Xs, you will go first.\n",stdout);
}
else if((char_choice==0x4F)||(char_choice==0x6F))
{
    computer_choice = 0x58;
    fputs("Since you picked Os, you will go second.\n",stdout);
}
else{fputs("Since you picked neither x's nor o's, you will go second and the computer will play as x's\n",stdout);computer_choice=0x58;}

set_up_board(rows, tic_tac_toe_board);

while(win_check(tic_tac_toe_board, char_choice, computer_choice, rows) == 0)
{
    gdisplay(tic_tac_toe_board, rows);

//    getchar();
    fputs("What index of the board would you like to pick? Specify 1st) the row, then press enter, and 2nd) the column, then press enter \n(use base 1).\n",stdout);
//    getchar();
    scanf("%d",&player_choice_row);
//    getchar();
    scanf("%d",&player_choice_col);
    place_marker(player_choice_row,player_choice_col, rows, char_choice, tic_tac_toe_board);
    computer_index_choice=rand()%num_spaces; //Make the computer's choice of where to place it's marker simply a random index in the range
                                             // [0,num_spaces) by using the mod operator
    while(tic_tac_toe_board[computer_index_choice]!=0x20)
    {
        computer_index_choice=rand()%num_spaces;
    }
    tic_tac_toe_board[computer_index_choice]=computer_choice;
    fputs("\n",stdout);
}

if(win_check(tic_tac_toe_board, char_choice, computer_choice, rows) == 1)
{
    gdisplay(tic_tac_toe_board, rows);
    fputs("You are the winner!!!\n",stdout);
}
else if(win_check(tic_tac_toe_board, char_choice, computer_choice, rows) == 2)
{
    gdisplay(tic_tac_toe_board, rows);
    fputs("The computer is the winner. Better luck next time!\n",stdout);
}

free(tic_tac_toe_board);

return 0;
}
