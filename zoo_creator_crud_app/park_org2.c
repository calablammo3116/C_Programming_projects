/* Caleb Gibson
    COP 3502C
    Section 0004
    Assignment 1 -- Organization
    due 9/12/2022 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Cage
{
    char* name;
};

struct Section
{
struct Cage* cages;
int num_cages;
};

struct Cage;
struct Section;

int main(int argc, char **argv)
{
    struct Section* all_secs;
    char name_input[1000 + 1]; // + 1 for the newline character '\n' that may be added when the user input is read in by scanf
    int tot_secs, cmd, sec_num, cage_num, name_length, i, j;

    cmd = 0;    // default value for command, to ensure it has no impact on rest of program

    // read in the number of sections
    scanf("%d", &tot_secs);

    // allocate enough space for all the sections
    all_secs = (struct Section*)malloc(sizeof(struct Section) * tot_secs);
    for(i = 0; i < tot_secs; i++)
    {
        all_secs[i].num_cages = 0;
    }

    // handle commands
    while (cmd != 4)
    {
            // read in command
            scanf("%d", &cmd);

            // exit case
            if (cmd == 4)
            {
                break;
            }

            // read in section and cage
            scanf("%d%d", &sec_num, &cage_num);

            // change number of cages in section case
            if (cmd == 1)
            {
                if (all_secs[sec_num-1].num_cages == 0)
                {
                    all_secs[sec_num-1].cages = (struct Cage *)malloc(sizeof(struct Cage) * cage_num);
                    for (i=0; i<cage_num; i++)
                    {
                        all_secs[sec_num-1].cages[i].name = NULL;
                    }
                }
                else
                {
                    if (all_secs[sec_num-1].num_cages > cage_num)
                    {
                        for (i = all_secs[sec_num-1].num_cages - 1; i > (cage_num - 1); i--)
                        {
                            free(all_secs[sec_num-1].cages[i].name);
                            all_secs[sec_num-1].cages[i].name = NULL;
                        }
                    }

                    all_secs[sec_num-1].cages = (struct Cage*)realloc(all_secs[sec_num-1].cages, sizeof(struct Cage) * cage_num);

                    if (all_secs[sec_num-1].num_cages < cage_num)
                    {
                        for (i=all_secs[sec_num-1].num_cages; i<cage_num; i++)
                        {
                            all_secs[sec_num-1].cages[i].name = NULL;
                        }
                    }
                }
                all_secs[sec_num-1].num_cages = cage_num;
            }

            // put animal in cage in section case
            else if (cmd == 2)
            {
                // read in animal's name
                scanf("%s", name_input);

                if ( sec_num > tot_secs  ||  all_secs[sec_num-1].num_cages < cage_num )
                {
                    continue;
                }
                else if (all_secs[sec_num-1].cages[cage_num-1].name != NULL)
                {
                    continue;
                }
                else
                {
                    all_secs[sec_num-1].cages[cage_num-1].name = (char *)malloc(sizeof(char) * (strlen(name_input) + 1));
                    strcpy(all_secs[sec_num-1].cages[cage_num-1].name, name_input);
                }
            }

            // "does cage in section have animal, and if so, what animal?" case
            else if (cmd == 3)
            {
                if ( sec_num > tot_secs  ||  all_secs[sec_num-1].num_cages < cage_num )
                {
                    printf("No animal found.\n");
                }
                else if (all_secs[sec_num-1].cages[cage_num-1].name == NULL)
                {
                    printf("No animal found.\n");
                }
                else
                {
                    printf("%s\n", all_secs[sec_num-1].cages[cage_num-1].name);
                }
            }
    }

    // free remaining memory
    for (i = 0; i < tot_secs; i++)
    {
        if (all_secs[i].num_cages != 0)
        {
            for (j = 0; j < all_secs[i].num_cages; j++)
            {
                free(all_secs[i].cages[j].name);
            }
            free(all_secs[i].cages);
        }


    }
    free(all_secs);

    return 0;
}
