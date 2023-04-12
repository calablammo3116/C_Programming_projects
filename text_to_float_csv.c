#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char ** argv)
{
    FILE * fp_in = fopen(argv[1], "r");
    FILE * fp_out = fopen(argv[2], "w");

    char input_string[256];
    char input_string2[256];
    double input_num = 0;

    for(int i = 0; i < 256; i++) {
        input_string[i] = 0;
        input_string2[i] = 0;
    }

    // Read in the input, convert it to a float, and write it to the output file
    while(fscanf(fp_in, "%s%s", input_string, input_string2) == 2)   // 2 because for 2 columns of data
    {
        input_num = atof(input_string);
        fprintf(fp_out, "%lf, ", input_num);
        input_num = atof(input_string2);
        fflush(stdout);
        fprintf(fp_out, "%lf\n", input_num);
        fflush(stdin);
        fflush(stdout);
    }


    // Close the file pointers
    fclose(fp_in);
    fclose(fp_out)

    return 0;
}
