/*
------------------------------------------------------------
 File: midpoint_omp.c
 Purpose: Numerical Integration using the Midpoint Rule (OpenMP)
 Domain: Numerical Computation & Scientific Computing
 Author: P W K W RUPASINGHE | Student ID: IT23283312
------------------------------------------------------------
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <omp.h>

// Function to get the current time in seconds
double now_seconds(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec * 1e-9;
}

// Function to integrate
double f(double x)
{
    return sin(x) * exp(-x / 10.0);
}

int main(int argc, char *argv[])
{
    if (argc < 4)
    {
        printf("Usage: %s <a> <b> <n>\n", argv[0]);
        return 1;
    }

    double a = atof(argv[1]);     // start
    double b = atof(argv[2]);     // end
    long long n = atoll(argv[3]); // number of intervals

    double dx = (b - a) / (double)n;
    double sum = 0.0;

    // Start timing
    double t0 = now_seconds();

    // Parallelize the main loop using OpenMP and combine with reduction
#pragma omp parallel for reduction(+ : sum) schedule(static)
    for (long long i = 0; i < n; i++)
    {
        double mid = a + (i + 0.5) * dx;
        sum += f(mid);
    }

    double integral = sum * dx;
    double t1 = now_seconds();

    // Output for performance evaluation
    printf("Approx integral = %.12f\n", integral);
    printf("time=%.9f threads=%d\n", (t1 - t0), omp_get_max_threads());

    return 0;
}
