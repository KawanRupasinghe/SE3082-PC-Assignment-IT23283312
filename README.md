# SE3082 – Parallel Computing Assignment
Author: P. W. K. W. Rupasinghe  
Student ID: IT23283312  
Algorithm: Midpoint Numerical Integration

---

## 1. Compilation Instructions

### Serial (macOS)
cd Serial  
gcc -O3 -std=c11 midpoint_serial.c -lm -o midpoint_serial

### OpenMP (macOS)
cd OpenMP  
gcc -O3 -std=c11 -fopenmp midpoint_omp.c -lm -o midpoint_omp

### MPI (macOS)
cd MPI  
mpicc -O3 -std=c11 midpoint_mpi.c -lm -o midpoint_mpi

### CUDA (Google Colab)
cd CUDA  
nvcc -O3 midpoint_cuda.cu -o midpoint_cuda

---

## 2. Execution Instructions

### Serial
./midpoint_serial 0 1000 10000000

### OpenMP
export OMP_NUM_THREADS=<threads>  
./midpoint_omp 0 1000 10000000

### MPI
mpirun -np <processes> ./midpoint_mpi 0 1000 10000000

### CUDA (Colab)
./midpoint_cuda 0 1000 10000000 <threads_per_block>

---

## 4. Tested Configurations

### OpenMP:
Threads tested: 1, 2, 4, 8, 16

### MPI:
Processes tested: 1, 2, 4, 8

### CUDA:
Threads per block tested: 8, 16, 32, 64, 128, 256

---

## 5. Notes
- Serial execution time used as baseline for speedup comparison.
- All implementations produce the same numerical result.
- CUDA executed in Google Colab using an NVIDIA T4 GPU.


