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
std::vector<int> a, b, c;     // arrays to store matrix blocks in each process

MPI_Datatype MPI_BLOCK_A; // datatype for the block of the matrix A
MPI_Datatype MPI_BLOCK_B; // datatype for the block of the matrix B
MPI_Datatype MPI_BLOCK_C; // datatype for the block of the matrix C

MPI_Comm MPI_COMM_GRID = MPI_COMM_NULL;
             // communicator associated with a two-dimensional Cartesian grid

void Matr3CreateTypeBlock(int m0, int p0, int p, MPI_Datatype& t)
{
    MPI_Type_vector(m0, p0, p, MPI_INT, &t);
    MPI_Type_commit(&t);
}

void Solve()
{
    Task("MPI9Matr21");
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

    pt >> m >> p;
    m0 = m / k0;
    p0 = p / k0;

    Matr3CreateTypeBlock(m0, p0, p, MPI_BLOCK_A);

    a.assign(m0 * p0, 0);

    if (r)
    {
        MPI_Recv(a.data(), a.size(), MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
    else
    {
        a_.assign(ptin_iterator<int>(m * p), ptin_iterator<int>());

        MPI_Sendrecv(a_.data(), 1, MPI_BLOCK_A, 0, 0, a.data(), a.size(), MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        for (int i = 1; i < k; ++i)
        {
            MPI_Send(&a_[i / k0 * m0 * p + (i % k0) * p0], 1, MPI_BLOCK_A, i, 0, MPI_COMM_WORLD);
        }
    }

    std::copy(a.begin(), a.end(), ptout_iterator<int>());
}
