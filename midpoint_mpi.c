/*
------------------------------------------------------------
 File: midpoint_mpi.c
 Purpose: Numerical Integration using the Midpoint Rule (MPI)
 Domain: Numerical Computation & Scientific Computing
 Author: P W K W RUPASINGHE | Student ID: IT23283312
------------------------------------------------------------
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

// Function to integrate
double f(double x)
{
    return sin(x) * exp(-x / 10.0);
}

int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // current process ID
    MPI_Comm_size(MPI_COMM_WORLD, &size); // number of processes

    if (argc < 4)
    {
        if (rank == 0)
            printf("Usage: %s <a> <b> <n>\n", argv[0]);
        MPI_Finalize();
        return 1;
    }

    double a = atof(argv[1]);
    double b = atof(argv[2]);
    long long n = atoll(argv[3]);
    double dx = (b - a) / (double)n;

    // Divide work evenly across processes
    long long local_n = n / size;
    long long start = rank * local_n;
    long long end = (rank == size - 1) ? n : start + local_n;

    double local_sum = 0.0;

    // Ensure all processes start timing together
    MPI_Barrier(MPI_COMM_WORLD);
    double t0 = MPI_Wtime();

    // Each process computes its own partial sum
    for (long long i = start; i < end; i++)
    {
        double mid = a + (i + 0.5) * dx;
        local_sum += f(mid);
    }

    // Measure local execution time
    MPI_Barrier(MPI_COMM_WORLD);
    double local_time = MPI_Wtime() - t0;

    // Combine sums from all processes
    double total_sum = 0.0;
    MPI_Reduce(&local_sum, &total_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    // Find maximum time across processes
    double max_time;
    MPI_Reduce(&local_time, &max_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    // Print the final result by rank 0
    if (rank == 0)
    {
        double integral = total_sum * dx;
        printf("Approx integral = %.12f\n", integral);
        printf("time=%.9f processes=%d\n", max_time, size);
    }

    MPI_Finalize();
    return 0;
}
