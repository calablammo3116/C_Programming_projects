// Caleb Gibson
// Assignment 2 -- Shady Park
// COP 3502
// Section 0004
// due 9/23/2022

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// notes: tan(x) returns the tangent of x in RADIANS, NOT degrees, so must convert result of tan(x) to radians before plugging in value into
// formula for calculating coverage. According to https://www.programiz.com/c-programming/library-function/math.h/tan
// to convert radians to degrees, do this: x rads = x*180/pi;
// apparently, pi does not exist in the C standard library, according to https://www.gnu.org/software/libc/manual/html_node/Mathematical-Constants.html,
// and while it is in math.h, it may also not exist on the local system, so define our own pi
// THE HIGHER THE COVERAGE ANGLE, THE LOWER THE COVERAGE
// SINCE TANGENT IS NEGATIVE IN THE SECOND QUADRANT (IE FROM ANGLES 90 TO 180 DEGREES), NEED TO TAKE THE ABSOLUTE VALUE OF THE COVERAGE TO GET
// THE ACTUAL VALUE/MAGNITUDE OF THE COVERAGE, SINCE IT WILL BE NEGATIVE WHEN ADDING IT
// OR, CAN JUST SUBTRACT COVERAGE VALUE FOR EACH TOWER FROM TOTAL COVERAGE FOR PARK, EFFECTIVELY MAKING THE TOTAL COVERAGE FOR THE PARK
// POSITIVE!

struct Tower    {
int height;
int location;
double covg;
};

struct Tower;
double absDouble(double);

int main(int argc, char **argv)
{
struct Tower* towers;
int num_towers, park_length, percent_coverage, i;   // note that percent coverage is an integer, as specified by the assignment
double actual_coverage, err_tol, pi, lower_angle_bound_r, upper_angle_bound_r, theta_r, coverage_r, lower_angle_bound_l, upper_angle_bound_l, theta_l, coverage_l;

pi = 3.14159265358979;   // first 14 digits of pi, according to http://www.geom.uiuc.edu/~huberty/math5337/groupe/digits.html and Professor Meade

err_tol = 0.000000001; // error tolerance = 10^-9

scanf("%d%d%d", &num_towers, &park_length, &percent_coverage);

// set actual coverage to equal the length of the park desired to be covered with shade
actual_coverage = park_length*((double)percent_coverage/100);   //  need cast percent average as a double, otherwise division rounds down to 0

// allocate memory space for the towers array
towers = (struct Tower *)malloc(sizeof(struct Tower) * num_towers);

for(i=0; i<num_towers; i++)
{
    scanf("%d%d", &towers[i].location, &towers[i].height);
    towers[i].covg = 0;
}

// right-side angle bisection
lower_angle_bound_r = 0;
upper_angle_bound_r = pi/2; // 90 degrees in radians is pi/2 -- we need to convert to rads (radians) because the tan() function in C only accepts an angle given in rads
coverage_r = 0;

// covg(theta) = tower.height / tan(theta) => mid = (hi_r + lo_r)/2, ymid = covg(mid) = tower.height / tan(mid); if(abs(ymid/parkLen - percent_coverage) < err_tol) { resr = mid; } else { if (ymid/parkLen - percent_coverage > 0) { hi_r = mid; } else { lo_r = mid; }
while(absDouble(coverage_r - actual_coverage) > err_tol)
{
    coverage_r = 0;

    theta_r = (lower_angle_bound_r + upper_angle_bound_r) / 2;    // midpoint of the two angles

    for(i=1; i < num_towers; i++)
    {
       towers[i].covg = towers[i].height / tan(theta_r);
    }

    for(i=1; i < num_towers; i++)
    {
        // if the tower's coverage exceeds the distance to the next tower, then ...
        if (towers[i].covg > towers[i].location - towers[i-1].location)
        {
            // if the first tower is so tall that it blocks out completely the coverage from the previous tower, nullify the coverage from the previous tower
            if (towers[i].location - towers[i].covg < towers[i-1].location - towers[i-1].covg)
            {
                coverage_r -= towers[i-1].covg;
                towers[i-1].covg = 0;
            }

            // otherwise, the first tower's coverage overlaps with some, BUT NOT ALL, of the previous tower's coverage. So, set this current
            // tower's coverage equal to the distance between the 2 towers.
            else
            {
                towers[i].covg = towers[i].location - towers[i-1].location; // nullify the overlapped coverage provided by the first tower
            }
        }

        // if the tower coverage exceeds the distance to the respective end of the park, nullify the coverage beyond it
        if (towers[i].location - towers[i].covg < 0)
        {
            towers[i].covg = (double)towers[i].location;
        }

        coverage_r += towers[i].covg;
    }

    // lower coverage means that our angle is too high; therefore, we NEED TO DECREASE our angle, if this is the case
    if (coverage_r < actual_coverage)
    {
        upper_angle_bound_r = theta_r;
    }

    // otherwise, we have higher-than-optimal coverage; therefore, we NEED TO INCREASE our lower-bound angle, if this is the case.
    else
    {
        lower_angle_bound_r = theta_r;
    }
}


// left-side angle bisection
lower_angle_bound_l = pi; // 90 degrees = pi/2 rads
upper_angle_bound_l = pi/2;   // 180 degrees = pi rads
coverage_l = 0;

// covg(theta) = tower.height / tan(theta) => mid = (hi_r + lo_r)/2, ymid = covg(mid) = tower.height / tan(mid); if(abs(ymid/parkLen - percent_coverage) < err_tol) { resr = mid; } else { if (ymid/parkLen - percent_coverage > 0) { hi_r = mid; } else { lo_r = mid; }
while(absDouble(coverage_l - actual_coverage) > err_tol)
{
    coverage_l = 0;

    theta_l = (lower_angle_bound_l + upper_angle_bound_l) / 2;

    for(i=num_towers-2; i >= 0; i--)
    {
       towers[i].covg = absDouble(towers[i].height / tan(theta_l));
    }

    for (i=num_towers-2; i >= 0; i--)
    {
        // check if the coverage overlaps with that of another tower
        if (towers[i].covg > towers[i+1].location - towers[i].location)
        {
            // if the first tower is so tall that it completely overlaps the coverage of the next tower, then nullify the next tower's coverage
            if(towers[i].location + towers[i].covg > towers[i+1].location + towers[i+1].covg)
            {
                coverage_l -= towers[i+1].covg;
                towers[i+1].covg = 0;   // nullify the coverage of the completely overlapped tower
            }
            // otherwise, the first tower overlaps only some of the next tower's coverage. So, set this first tower's coverage equal to the
            // distance between the 2 towers.
            else
            {
                towers[i].covg = towers[i+1].location - towers[i].location; // nullify the overlapped coverage provided by the previous tower
            }
        }

        // if a tower is so tall that its coverage goes beyond the distance to the respective end of the park, nullify the coverage beyond it
        if (towers[i].location + towers[i].covg > park_length)
        {
            towers[i].covg = (double)park_length - towers[i].location;
        }

        coverage_l += towers[i].covg;
    }

    // IF THE ACTUAL COVERAGE IS TOO LOW, THEN THE ANGLE IS TOO HIGH; SOOO, LOWER THE BOUND FOR THE UPPER ANGLE, IF THIS IS THE CASE
    if (coverage_l < actual_coverage)
    {
        upper_angle_bound_l = theta_l;
    }

    // otherwise, the actual coverage is higher-than-optimal, so the angle is too low; therefore, increase the bound for the lower angle, IF
    // THIS IS THE CASE.
    else
    {
        lower_angle_bound_l = theta_l;
    }
}

// Convert mid_r and mid_l back into degrees
theta_r = theta_r * 180 / pi;
theta_l = theta_l * 180 / pi;

// print the results to the screen
printf("%.5lf   %.5lf\n", theta_r, theta_l);

// free any and all dynamically allocated memory
free(towers);

return 0;
}

double absDouble(double num)
{
    if(num < 0)
    {
        return num*-1;
    }
    else
    {
        return num;
    }
}
