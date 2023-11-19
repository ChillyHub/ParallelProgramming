#include "pt4.h"

#include "mpi.h"

#include <cmath>

#include <vector>

int k;              // number of processes
int r;              // rank of the current process

int m, p, q;        // sizes of the given matrices
int na, nb;         // sizes of the matrix bands

std::vector<int> a_, b_, c_;  // arrays to store matrices in the master process
std::vector<int> a, b, c;     // arrays to store matrix bands in each process

void Matr1Calc(int I)
{
    for (int i = 0; i < na; ++i)
    {
        for (int j = 0; j < q; ++j)
        {
            for (int n = 0; n < nb; ++n)
            {
                const int offset = ((r + I) % k) * nb + n;
                if (offset < p)
                {
                    c[i * q + j] += a[i * p + offset] * b[n * q + j];
                }
            }
        }
    }

    MPI_Sendrecv_replace(b.data(), nb * q, MPI_INT, (k + r - 1) % k, 0, (r + 1) % k, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
}

void Solve()
{
    Task("MPI9Matr5");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    k = size;
    r = rank;

    pt >> na >> p >> nb >> q;
    a.assign(ptin_iterator<int>(na * p), ptin_iterator<int>());
    b.assign(ptin_iterator<int>(nb * q), ptin_iterator<int>());
    c.assign(ptin_iterator<int>(na * q), ptin_iterator<int>());

    int L;
    pt >> L;

    for (int i = 0; i < L; ++i)
    {
        Matr1Calc(i);
    }

    std::copy(c.begin(), c.end(), ptout_iterator<int>());
    std::copy(b.begin(), b.end(), ptout_iterator<int>());
}
