#include "pt4.h"

#include "mpi.h"

#include <cmath>

#include <vector>

int k;              // number of processes
int r;              // rank of the current process

int m, p, q;        // sizes of the given matrices
int m0, p0, q0;     // sizes of the matrix blocks
int k0;             // order of the Cartesian grid (equal to sqrt(k))

std::vector<int> a_, b_, c_;  // arrays to store matrices in the master process
std::vector<int> a, b, c, t;  // arrays to store matrix blocks in each process

MPI_Datatype MPI_BLOCK_A; // datatype for the block of the matrix A
MPI_Datatype MPI_BLOCK_B; // datatype for the block of the matrix B
MPI_Datatype MPI_BLOCK_C; // datatype for the block of the matrix C

MPI_Comm MPI_COMM_GRID = MPI_COMM_NULL;
             // communicator associated with a two-dimensional Cartesian grid
MPI_Comm MPI_COMM_ROW = MPI_COMM_NULL;
             // communicator associated with rows of a two-dimensional grid
MPI_Comm MPI_COMM_COL = MPI_COMM_NULL;
             // communicator associated with columns of a two-dimensional grid

void Matr4CreateCommGrid(MPI_Comm& comm)
{
    const int dims[] = { k0, k0 };
    constexpr int periods[] = { true, true };
    MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, false, &comm);
}

void Matr4CreateCommRow(MPI_Comm grid, MPI_Comm& row)
{
    constexpr int remain_dims[] = { false, true };
    MPI_Cart_sub(grid, remain_dims, &row);
}

void Solve()
{
    Task("MPI9Matr33");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    k = size;
    r = rank;
    k0 = (int)floor(sqrt((double)k) + 0.1);

    pt >> m0 >> p0;
    a.assign(ptin_iterator<int>(m0 * p0), ptin_iterator<int>());

    Matr4CreateCommGrid(MPI_COMM_GRID);
    Matr4CreateCommRow(MPI_COMM_GRID, MPI_COMM_ROW);

    int coords[2];
    MPI_Cart_coords(MPI_COMM_GRID, r, 2, coords);

    MPI_Bcast(a.data(), a.size(), MPI_INT, coords[0], MPI_COMM_ROW);

    pt << coords[0] << coords[1] << coords[1];
    std::copy(a.begin(), a.end(), ptout_iterator<int>());
}
