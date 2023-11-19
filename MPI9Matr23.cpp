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

void Matr3ScatterData()
{
    int cast[6];

	if (!r)
	{
        pt >> m >> p >> q;

        cast[0] = static_cast<int>(ceil(static_cast<double>(m) / k0));
        cast[1] = static_cast<int>(ceil(static_cast<double>(p) / k0));
        cast[2] = static_cast<int>(ceil(static_cast<double>(q) / k0));
        cast[3] = cast[0] * k0;
        cast[4] = cast[1] * k0;
        cast[5] = cast[2] * k0;

        const int M = cast[3];
        const int P = cast[4];
        const int Q = cast[5];

        a_.assign(M * P, 0);
        b_.assign(P * Q, 0);

        for (int i = 0; i < m; ++i)
        {
	        for (int j = 0; j < p; ++j)
	        {
                pt >> a_[i * P + j];
	        }
        }

        for (int i = 0; i < p; ++i)
        {
	        for (int j = 0; j < q; ++j)
	        {
                pt >> b_[i * Q + j];
	        }
        }
	}

    MPI_Bcast(cast, 6, MPI_INT, 0, MPI_COMM_WORLD);

    m0 = cast[0];
    p0 = cast[1];
    q0 = cast[2];
    m = cast[3];
    p = cast[4];
    q = cast[5];

    a.assign(m0 * p0, 0);
    b.assign(p0 * q0, 0);
    c.assign(m0 * q0, 0);

    Matr3CreateTypeBlock(m0, p0, p, MPI_BLOCK_A);
    Matr3CreateTypeBlock(p0, q0, q, MPI_BLOCK_B);

    if (r)
    {
        MPI_Recv(a.data(), a.size(), MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(b.data(), b.size(), MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
    else
    {
        MPI_Sendrecv(a_.data(), 1, MPI_BLOCK_A, 0, 0, a.data(), a.size(), MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Sendrecv(b_.data(), 1, MPI_BLOCK_B, 0, 0, b.data(), b.size(), MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        for (int i = 1; i < k; ++i)
        {
            MPI_Send(&a_[i / k0 * m0 * p + (i % k0) * p0], 1, MPI_BLOCK_A, i, 0, MPI_COMM_WORLD);
            MPI_Send(&b_[i / k0 * p0 * q + (i % k0) * q0], 1, MPI_BLOCK_B, i, 0, MPI_COMM_WORLD);
        }
    }
}

void Solve()
{
    Task("MPI9Matr23");
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

    Matr3ScatterData();

    pt << m0 << p0 << q0;
    std::copy(a.begin(), a.end(), ptout_iterator<int>());
    std::copy(b.begin(), b.end(), ptout_iterator<int>());
    std::copy(c.begin(), c.end(), ptout_iterator<int>());
}
