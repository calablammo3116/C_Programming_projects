//Caleb Gibson, COP3223H, Section 0203, Final Project, Submitted 12/8/20
//This program consists of two main parts. The first part consists of reading in, sorting, and printing to the screen a set of data from an input file
//containing the data for a number of stocks. The data regarding each stock includes and is printed out (in this order) the stock's ticker symbol (the
//official symbol for the name of the stock), the price of the stock in US dollars (USD $), and the percent growth of the stock (over no particular time
//period). The user specifies the input file they want the program to read in as well as the manner in which to sort the data from the input file.
//The second part of the program is optional, and allows the user to run a simulation of buying a single stock from the data given a randomly generated cash
//balance. The user is allowed to buy one or more shares of any stock they choose from the list, as long as the cost of their purchase does not exceed the
//randomly generated cash balance they are given at the start of the simulation. Throughout the simulation, they are given to stop the simulation and end the
//program. Regardless, before the program ends, at least one positive farewell message is printed to the screen, the final one always being a message wishing
//the user "Happy holidays!"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct stock	//This is the structure used to represent one stock
	{
	char ticker[10];//This field of stock refers to the ticker symbol for the stock. It can hold up to 10 characters, but US-listed stocks often have less
	double price;	//This field of stock refers to the price of the stock in USD
	double growth;	//This field of stock refers to the percent growth of the stock
	};

int read_data(struct stock array[]);
int user_sort_request();
void sort_data(struct stock list[], int num, int len);
void print_data(struct stock list[], int num);
double random_bal();
double buy(struct stock list[], double bal, int len);

int main ()
	{
	struct stock stock_list[100];	//This stock structure array stores one stock structure (one stock from the data in the input file) at each index
	char ans1, ans2;		//These character variables store yes-or-no answers (y or n) to questions in the main program.
	int sort_choice, num_stocks;	//sort_choice stores an integer representing the main way the user wants the data to be sorted; num_stocks stores the
					//integer value indicating the number of stocks in the data.
	double cur_bal, new_bal;	//cur_bal stores the user's randomly-generated initial cash balance in $ at the start of the simulation. new_bal
					//stores the user's final cash $ balance at the end of the simulation, after they have purchased however many shares
					//of the stock from the data that they chose.

	//Part 1: Reading in the Data
	//First, read in the data. Since the function read_data returns an integer indicating the number of stocks in the data, set num_stocks equal to it.
	num_stocks = read_data(stock_list);

	//Next, get the user's input on how to sort the data. As the function user_sort_request returns an integer showing how the user wants the data sorted
	//(1 for sorting by ticker, 2 for sorting by price, 3 for sorting by growth), set sort_choice equal to that function.
	sort_choice = user_sort_request();

	//Now, sort the data using the function sort_data. This function will also get more input from the user on how they want to sort the data before doing
	//so (for example, asking if they want the data sorted A to Z or Z to A if they requested the data be sorted by ticker, as indicated by sort_choice,
	//which is one of the parameters for sort_data here in main).
	sort_data(stock_list, sort_choice, num_stocks);

	//Finally, for the end of Part 1 of the program, call the print_data function to print the data, using the now-filled-and-sorted stock structure
	//array stock_list[] and num_stocks (the number of stocks in the data from the input file) as the paramaters.
	print_data(stock_list, num_stocks);


	//Part 2: Stock-Buying Simulation - if the user wants, simulate buying one or more shares of a single stock from the data, using a randomly generated
	//cash balance
	//In this part of the program, the user is often asked polar (yes-or-no) questions. Each time, a positive response is indicated by entering the letter
	//'y' (as in "Yes") and a negative response is indicated by entering the letter 'n' (as in "No"). Any time the user gives a negative response, the
	//simulation, and consequently the program end, with an acknowledgement of the user's decision as well as a positive farewell message. Any time the
	//user gives a positive response, they are allowed to continue with the simulation. Each time the user is asked to give a response to a question,
	//a combination of an if statement and a while loop designed to make sure that one, the user gives a valid response and two, to give them a chance to
	//re-enter their response until it is valid, is implemented in the program.

	//The simulation starts by asking thm if they would like to know their current cash balance. Store the answer's reply in the character variable 'ans1'
	printf("Would you like to know your current balance?\n");
	scanf("%c", &ans1);
	//Use the combination if-statement-and-while-loop described above to ensure the user's response is valid.
	if (ans1 != 121 && ans1 != 110)	//The decimal ASCII values for y and n are 121 and 110 respectively, so if ans1 doesn't equal y and it doesn't equal n
		{
		while (ans1 != 121 && ans1 != 110)	//While ans1 isn't y or n, ask the user to give a valid response and allow them to enter a response
			{
			printf("Please enter a valid response. Enter y for 'Yes' or n for 'No'\n");
			scanf("%c", &ans1);
			}
		}

	//Next, the second part of the simulation. If the user's reply to the cash balance question was 'y', continue with running the simulation by randomly
	//generating their starting cash balance by assigning the cur_bal variable the value returned by the random_bal function (which as the name describes,
	//generates a random balance, in this case for use in this simulation in the main program). However, if the user's reply was 'n', end the simulation
	//right here (essentially before it even starts).
	if (ans1 == 121)	//if the user's reply was y
		{
		cur_bal = random_bal();	//starting cash balance is a random number between 0 and 10000
		printf("Your current cash balance is $%6.2lf\n", cur_bal);	//Print the starting cash balance in $ to the screen
	//Next, the user is asked if they would like to buy any shares of a stock. Their answer to this question is stored in 'ans2'.
	//If they answer n, the simulation and thus the program end right here. If they answer y, the simulation continues by setting the variable new_bal
	//(their final cash balance after the transactopm) equal to what the buy function returns after it has been called and ran.
		printf("Would you like to purchase any shares of a stock?\n");
		scanf("%c", &ans2);
	//Implement the same combination if-statement-and-while-loop combination as described above to ensure the user's response is valid.
		if (ans2 != 121 && ans2 != 110)
			{
			while (ans2 != 121 && ans2 != 110)
				{
				printf("Please enter a valid response. Enter y for 'Yes' or n for 'No'\n");
				scanf("%c", &ans2);
				}
			}

		if (ans2 == 121)	//If ans2 is y
			{
			//Call and run the buy function, taking the stock_list[] array, the starting cash balance, and the number of stocks as parameters.
			//Set new_bal equal to it, since it returns the user's final cash balance after they have carried out their stock purchase transaction
			//(that is, if they decided to carry one out); the buy function allows the user to pick a stock from the list and specify how many
			//shares of it they want to buy, but if the cost of the transaction exceeds their starting cash balance, they are given the chance to
			//either redo the process of picking a stock and specifying how many shares of it they want to buy or decline to do so and essentially
			//end the simulation right there, leaving their starting cash balance unchanged and returning to the main program.
			new_bal = buy(stock_list, cur_bal, num_stocks);
			printf("Your final cash balance is $%6.2lf\nHave a nice day!\n", new_bal);	//Report the user's ending balance in USD.
			}
		else			//If ans2 is n
			{
			printf("Okay. Have a nice day!\n");	//Print to the screen a positive farewell message acknowledging the user's decision.
			}
		}

	else if (ans1 == 110)		//If ans1 (the user's reply to the question about knowing their starting cash balance) is n
		{
		printf("Okay. Have a nice day! ");	//Print to the screen a positive farewell message acknowledging the user's decision.
		}

	//This is the end of Part 2 of the main program.

	//Finally, print a farewell message to the screen wishing the user happy holidays.
	printf("Happy Holidays!\n");

	return 0;
	}


//Function 1
//Function Name: read_data
//Preconditions: a structure array of structure type 'stock'.
//Postconditions: this function returns an integer indicating the number of stocks in the data.
//Actions: This function asks the user which stock data file they want read in, then reads it in. It stores the first line of the input file, which contains
//an integer indicating the number of stocks in the data, into the integer variable numstocks. It stores the contents of each subsequent line into an index
//of the stock array, starting at index 0 and storing the contents of each next line of the data in the appropriate structure field at the next index of the
//array until it reaches the end of the input file.
int read_data(struct stock array[])
	{
	int i, numstocks;	//the number of stocks and an integer loop counter i
	FILE *fp;		//a file pointer
	char filename[50];	//a character array for storing the name of the input file, with space for up to 50 characters (excluding the null character)

	//Ask the user for the name of the data file they would like to read in.
	printf("Which data file would you like to read in?\n");
	scanf("%s", filename);	//Store the desired file's name in the character array filename

	//Initialize the file pointer; set it to read in the specified file
	fp = fopen(filename, "r");
	fscanf(fp, "%d", &numstocks);	//Read in the first line of the input file (the number of stocks in the data) and store it in numstocks

	//Loop through each index of the struct stock array from the preconditions and fill the appropriate fields of the stock structure at each one with
	//the corresponding information from the input file.
	for (i=0; i<numstocks; i++)
		{
		fscanf(fp, "%s %lf %lf", array[i].ticker, &array[i].price, &array[i].growth);
		}

	return numstocks;	//Return the number of stocks in the input file.
	}



//Function 2
//Function Name: user_sort_request
//Preconditions: none
//Postconditions: an integer indicating the user's choice regarding how they want the data sorted (1 = ticker, 2 = price, 3 = growth)
//Actions: this function gets a user's input regarding how they want the stock data to be sorted.
int user_sort_request()
	{
	int sort_number;	//the number representing the user's choice for they want to sort the data from the input file

	//Ask the user how they want the data sorted and specify what each number response represents; 1 for by ticker, 2 for by price, 3 for by growth
	printf("How would you like the data to be sorted? Enter 1 for ticker, 2 for the current price, or 3 for the percent growth\n");
	scanf("%d", &sort_number);

	//Implement a while loop that runs while the user's response isn't stored as one of these three number choices to ensure their response is valid
	while (sort_number > 3 || sort_number < 1)
		{
		printf("Please enter a valid number choice.\n");
		scanf("%d", &sort_number);
		}

	return sort_number;	//Return the number indicating how the user wants the data sorted
	}


//Function 3
//Function Name: sort_data
//Preconditions: a structure array of structure type 'stock', an integer indicating the user's choice in terms of sorting the data, and an integer indicating
//		 the total number of stocks in the 'stock' structure array.
//Postconditions: none - this function doesn't return anything.
//Actions: This function sorts the stock list data according to the user's request: 1 for ticker, 2 for price, 3 for percent growth. It first asks what order
//	   user wants the data sorted, such as standard or reverse alphabetical order, or greatest to least or least to greatest, then sorts the data
//	   accordingly, swapping the stock structures in the now-filled array until it is sorted according to the user's specifications.
void sort_data(struct stock list[], int num, int len)
	{
	int i, j, order;
	struct stock temp;

	//Case 1: sorting the data by ticker symbol
	if (num == 1)
		{
		printf("Would you like the tickers sorted A-Z, or Z-A? Enter 1 for 'A-Z' or 2 for 'Z-A'\n");
		scanf("%d", &order);
		while (order < 1 || order > 2)
			{
			printf("Please enter a valid choice.\n");
			scanf("%d", &order);
			}
		//Case 1A: sorting the data by ticker symbol alphabetically (A to Z)
		if (order == 1)
			{
			//Start at the start of the stock array (index 0), then work up to the last index containing any relevant data.
			for (i=0; i<len; i++)
				{
				for (j=i; j<len; j++)
					{
					if ( strcmp(list[i].ticker, list[j].ticker) > 0)
						{
						temp.growth = list[i].growth;
						temp.price = list[i].price;
						strcpy(temp.ticker, list[i].ticker);

						list[i].growth = list[j].growth;
						list[i].price = list[j].price;
						strcpy(list[i].ticker, list[j].ticker);

						list[j].growth = temp.growth;
						list[j].price = temp.price;
						strcpy(list[j].ticker, temp.ticker);
						}
					}
				}
			}
		//Case 1B: sorting the data by ticker symbol in reverse alphabetical order (Z to A)
		else if (order == 2)
			{
			//Start at the end of the stock array (the last index containing any relevant data), then work down back to the start (index 0)
			for (i=len-1; i>=0; i--)
				{
				for (j=i; j>=0; j--)
					{
                                        if ( strcmp(list[i].ticker, list[j].ticker) > 0 )
                                                {
                                                temp.growth = list[i].growth;
                                                temp.price = list[i].price;
                                                strcpy(temp.ticker, list[i].ticker);

                                                list[i].growth = list[j].growth;
                                                list[i].price = list[j].price;
                                                strcpy(list[i].ticker, list[j].ticker);

                                                list[j].growth = temp.growth;
                                                list[j].price = temp.price;
                                                strcpy(list[j].ticker, temp.ticker);
						}
					}
				}
			}
		}

	//Case 2: sorting the data by price
	else if (num == 2)
		{
		printf("Would you like the prices sorted from greatest to least, or least to greatest? Enter 1 for 'greatest to least' or 2 for 'least to greatest'\n");
		scanf("%d", &order);
		while (order < 1 || order > 2)
			{
			printf("Please enter a valid choice.\n");
			scanf("%d", &order);
			}
		//Case 2A: sorting the data by price from greatest to least
		if (order == 1)
			{
			//Start at index 0, work up until the last index containing stock data from the input file
                        for (i=0; i<len; i++)
                                {
                                for (j=i; j<len; j++)
                                        {
					if (list[j].price > list[i].price)
						{
                                                temp.growth = list[i].growth;
                                                temp.price = list[i].price;
                                                strcpy(temp.ticker, list[i].ticker);

                                                list[i].growth = list[j].growth;
                                                list[i].price = list[j].price;
                                                strcpy(list[i].ticker, list[j].ticker);

                                                list[j].growth = temp.growth;
                                                list[j].price = temp.price;
                                                strcpy(list[j].ticker, temp.ticker);
						}
                                        }
                                }
			}
		//Case 2B: sorting the data by price from least to greatest
		else if (order == 2)
			{
			//Start at the last index of the stock array containing any relevant data, work down to the start (index 0).
                        for (i=len-1; i>=0; i--)
                                {
                                for (j=i; j>=0; j--)
                                        {
                                        if (list[j].price > list[i].price)
                                                {
                                                temp.growth = list[i].growth;
                                                temp.price = list[i].price;
                                                strcpy(temp.ticker, list[i].ticker);

                                                list[i].growth = list[j].growth;
                                                list[i].price = list[j].price;
                                                strcpy(list[i].ticker, list[j].ticker);

                                                list[j].growth = temp.growth;
                                                list[j].price = temp.price;
                                                strcpy(list[j].ticker, temp.ticker);
						}
                                        }
                                }
			}
		}

	//Case 3: sorting the data by growth
	else if (num == 3)
		{
		printf("Would you like the percent growth sorted from greatest to least, or least to greatest? Enter 1 for 'greatest to least' or 2 for 'least to greatest'\n");
                scanf("%d", &order);
                while (order < 1 || order > 2)
                        {
                        printf("Please enter a valid choice.\n");
                        scanf("%d", &order);
                        }
		//Case 3A: sorting the data by growth from greatest to least
		if (order == 1)
			{
			//Start at index 0 of the stock array, work up to the last index containing stock data from the input file.
                        for (i=0; i<len; i++)
                                {
                                for (j=i; j<len; j++)
                                        {
					if (list[j].growth > list[i].growth)
						{
                                                temp.growth = list[i].growth;
                                                temp.price = list[i].price;
                                                strcpy(temp.ticker, list[i].ticker);

                                                list[i].growth = list[j].growth;
                                                list[i].price = list[j].price;
                                                strcpy(list[i].ticker, list[j].ticker);

                                                list[j].growth = temp.growth;
                                                list[j].price = temp.price;
                                                strcpy(list[j].ticker, temp.ticker);
						}
                                        }
                                }
			}
		//Case 3B: sorting the data by growth from least to greatest
		else if (order == 2)
			{
			//Start at the last index of the stock array containing relevant stock data from the input file, work down to the start (index 0).
                        for (i=len-1; i>=0; i--)
                                {
                                for (j=i; j>=0; j--)
                                        {
					if (list[j].growth > list[i].growth)
						{
						temp.growth = list[i].growth;
						temp.price = list[i].price;
						strcpy(temp.ticker, list[i].ticker);
						list[i].growth = list[j].growth;
						list[i].price = list[j].price;
						strcpy(list[i].ticker, list[j].ticker);
						list[j].growth = temp.growth;
						list[j].price = temp.price;
						strcpy(list[j].ticker, temp.ticker);
						}
                                        }
                                }
			}
		}

	return;
	}


//Function 4
//Function Name: print_data
//Preconditions: a structure array of structure type 'stock' and an integer representing the number of stocks.
//Postconditions: none - this function doesn't return anything
//Actions: This function organizes the data as a table by printing a heading for each of the three types of data for each stock, then separating them from the
//	   data by printing '=' signs under them. It then loops through every index of the "stock" structure array, numbers each stock by its index location
//	   but counting up from 1 instead of 0 - so index 0 is numbered as "1.", index 1 as numbered as "2.", and so on), and prints its values underneath the
//	   appropriate heading.
void print_data(struct stock list[], int num)
	{
	int i;	//integer loop counter i

	//print the headings for the data, and below them, print the equal '=' signs helping to separate them from the data
	printf("      Tickers    Share Price    Growth\n      ========   ============   =========\n");

	//print the data, starting at the first index of the stock array containing any stock data and going up to the last one. Number each stock structure
	//to the left of its data, starting from 1 instead of 0 to try to make it more natural for the user (if, for example, they are not computer-savvy)
	for (i=0; i<num; i++)
		{
		printf("%3d.  %8s   %12.3lf   %6.2lf Percent \n", i+1, list[i].ticker, list[i].price, list[i].growth);
		}

	return;
	}


//Function 5
//Function Name: random_bal
//Preconditions: none
//Postconditions: a double indicating the user's current balance.
//Actions: This function seeds the random number generator to generate a random double value between 0 and 10000 representing the user's current balance.
double random_bal()
	{
	double balance;	//stores the randomly generated balance

	//Seed the random number generator with the time function
	srand(time(0));

	//Set the balance equal to a random number between 0 and 10000
	balance = rand()%10000;

	return balance;	//return the randomly generated number stored in balance
	}


//Function 6
//Function Name: buy
//Preconditions: a structure array of structure type "stock", a double indicating the user's current balance, and an integer indicating the number of stocks
//		 in the list.
//Postconditions: a double indicating the user's new balance.
//Actions: This function asks the user which stock they would like to buy and how many shares of it they would like to buy. If the cost of their transaction
//	   is greater than their current balance, it asks them if they would like to make another selection or not. If they don't, it returns their balance,
//	   unchanged. If they do make a purchase, it prints a report of their purchase to the screen, and subtracts the total cost of their transaction in USD
//	   from their starting balance, indicated by the double variable num in the preconditions. It then returns this new balance.
double buy(struct stock list[], double num, int len)
	{
	double new_bal;	//indicates the user's new balance at the end of the function
	char user_response;	//indicates the user's yes-or-no response to the question of trying to buy another stock if their first attempt does not work
	int stock_choice, num_shares;	//stock_choice indicates the index in the stock array of the stock the user wants to purchase; num_shares indicates
					//the number of shares the user wants to buy

	//Ask the user for the position in the list (or table, if one would rather call it that) of the stock they want to buy
	printf("Please enter the number indicating the position in the list of the stock you would like to buy.\n");
	scanf("%d", &stock_choice);
	//Since the stock's number in the list (or table) is actually 1 greater than its index in the stock array, decrease the user's entry by 1.
	stock_choice--;
	//Ensure the user's response is valid; if it isn't, have them re-enter their response until it is valid.
	while (stock_choice < 0 || stock_choice >= len)
		{
		printf("Please enter a valid number.\n");
		scanf("%d", &stock_choice);
		stock_choice--;
		}

	//Ask the user how many shares they would like to buy of the selected stock.
	printf("How many shares of %s would you like to buy?\n", list[stock_choice].ticker);
	scanf("%d", &num_shares);
	//Check if the total cost of the number of shares they want to buy exceeds their current balance. While it does, ask the user if they want to try to
	//make another purchase. If they don't return their unchanged balance. If they do, let them repeat the process of selecting a stock from the list to
	//buy and then specifying how many shares of it to buy.
	while (num_shares * list[stock_choice].price > num)
		{
		printf("You do not have enough cash to make this purchase. Would you like to try to make a different purchase?\n");
		scanf("%c", &user_response);
		//Check to make sure the user's response to this question is valid (y or n).
		if (user_response != 121 && user_response != 110)
			{
			while (user_response != 121 && user_response != 110)
				{
				printf("Please give a valid response. y for 'Yes' or n for 'No'\n");
				scanf("%c", &user_response);
				}
			}

		//If the user wants to try to make another purchase:
		if (user_response == 121)
			{
			printf("Please enter the number indicating the position in the list of the stock you would like to purchase.\n");
			scanf("%d", &stock_choice);
			stock_choice--;
			printf("How many shares of %s would you like to buy?\n", list[stock_choice].ticker);
			scanf("%d", &num_shares);
			//If the total cost still exceeds the user's current cash balance, have them redo the loop (redo the whole process of answering the
			//question of whether they want to try to make another purchase, then specifying the stock and how many shares of it they want to buy)
			if (num_shares * list[stock_choice].price > num)
				{
				continue;
				}
			//If the total cost does not exceed the user's current cash balance, go ahead and exit the loop
			else
				{
				break;
				}
			}
		//If the user does not want to try to make another purchase:
		else
			{
			printf("Okay.\n");	//Print a message to the screen acknowledging their decision
			new_bal = num*1.00;	//Make sure new_bal has a value, even though the new balance is unchanged from the starting one
			return new_bal;		//Return the new balance, which remains the same as the starting balance indicated by the parameter double num
			}
		}

	//If the user did make a stock purchase, print a message to the screen reporting their purchase, showing the number of shares they bought, the ticker
	//symbol for the stock they bought, and the total cost in $ of their transaction.
	printf("Congratulations. You bought %d shares of %s for a total of $%6.2lf.\n", num_shares, list[stock_choice].ticker,
												num_shares*list[stock_choice].price);

	//Calculate the user's new balance by subtracting the cost of their purchase from the starting balance indicated by the precondition num
	new_bal = num - (num_shares * list[stock_choice].price);

	return new_bal;
	}
