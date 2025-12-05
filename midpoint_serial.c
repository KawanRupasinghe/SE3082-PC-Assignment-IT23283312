/*
------------------------------------------------------------
 File: midpoint_serial.c
 Purpose: Numerical Integration using the Midpoint Rule
 Domain: Numerical Computation & Scientific Computing
 Author: P W K W RUPASINGHE | Student ID: IT23283312
------------------------------------------------------------
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

// Function to get the current time in seconds
double now_seconds(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec * 1e-9;
}

// Function to integrate: f(x) = sin(x) * e^(-x/10)
double f(double x)
{
    return sin(x) * exp(-x / 10.0);
}

int main(int argc, char *argv[])
{
    if (argc < 4)
    {
        printf("Usage: %s <a> <b> <n> [--print]\n", argv[0]);
        return 1;
    }

    // Read input parameters
    double a = atof(argv[1]);     // Start of interval
    double b = atof(argv[2]);     // End of interval
    long long n = atoll(argv[3]); // Number of subintervals
    int do_print = (argc > 4 && strcmp(argv[4], "--print") == 0);

    if (n <= 0 || b <= a)
    {
        printf("Invalid input parameters.\n");
        return 1;
    }

    // Width of each subinterval
    double dx = (b - a) / (double)n;
    double sum = 0.0;

    double t0 = now_seconds(); // Start timing

    // Main loop: compute sum of f for each subinterval
    for (long long i = 0; i < n; i++)
    {
        double mid = a + (i + 0.5) * dx; // Midpoint of current subinterval
        sum += f(mid);
    }

    // Final result
    double integral = sum * dx;

    double t1 = now_seconds(); // End timing

    if (do_print)
        printf("a=%.3f, b=%.3f, n=%lld, dx=%.6e\n", a, b, n, dx);

    printf("Approx integral = %.9f\n", integral);
    printf("Elapsed time = %.6f seconds\n", t1 - t0);

    return 0;
}
