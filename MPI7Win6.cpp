#include "pt4.h"

#include "mpi.h"

#include <vector>

void Solve()
{
    Task("MPI7Win6");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int K = size - 1;
    std::vector<double> A(2 * K - 1);
    std::vector<double> B;

    if (rank)
    {
        B = std::vector<double>(ptin_iterator<double>(rank), ptin_iterator<double>());
    }
    else
    {
        A = std::vector<double>(ptin_iterator<double>(2 * K - 1), ptin_iterator<double>());
    }

    int doubleSize;
    MPI_Type_size(MPI_DOUBLE, &doubleSize);

    MPI_Win win;
    MPI_Win_create(A.data(), A.size() * doubleSize, doubleSize, MPI_INFO_NULL, MPI_COMM_WORLD, &win);

    MPI_Win_fence(0, win);

    if (rank)
    {
        MPI_Accumulate(B.data(), rank, MPI_DOUBLE, 0, rank - 1, rank, MPI_DOUBLE, MPI_SUM, win);
    }

    MPI_Win_fence(0, win);

    if (!rank)
    {
        std::copy(A.begin(), A.end(), ptout_iterator<double>());
    }

    MPI_Win_free(&win);
}
