#include "pt4.h"

#include "mpi.h"

#include <vector>

void Solve()
{
    Task("MPI7Win16");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<double> A(ptin_iterator<double>(size * 1), ptin_iterator<double>());

    double B;
    pt >> B;

    std::vector<double> tmp(size, B);

    int doubleSize;
    MPI_Type_size(MPI_DOUBLE, &doubleSize);

    MPI_Win win;
    MPI_Win_create(A.data(), A.size() * doubleSize, doubleSize, MPI_INFO_NULL, MPI_COMM_WORLD, &win);

    MPI_Win_fence(0, win);

    MPI_Accumulate(tmp.data(), tmp.size(), MPI_DOUBLE, (rank + 1) % size, 0, A.size(), MPI_DOUBLE, MPI_MAX, win);

    MPI_Win_fence(0, win);

    for (int i = 0; i < size; ++i)
    {
        MPI_Get(&tmp[i], 1, MPI_DOUBLE, i, rank, 1, MPI_DOUBLE, win);
    }
    
    MPI_Win_fence(0, win);

    std::copy(tmp.begin(), tmp.end(), ptout_iterator<double>());

    MPI_Win_free(&win);
}
