#include "pt4.h"

#include "mpi.h"

#include <vector>

void Solve()
{
    Task("MPI7Win3");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int K = size - 1;
    std::vector<int> A(K);

    if (!rank)
    {
	    A = std::vector<int>(ptin_iterator<int>(K), ptin_iterator<int>());
    }

    int intSize;
    MPI_Type_size(MPI_INT, &intSize);

    MPI_Win win;
    MPI_Win_create(A.data(), A.size() * intSize, intSize, MPI_INFO_NULL, MPI_COMM_WORLD, &win);

    MPI_Win_fence(0, win);

    if (rank)
    {
        MPI_Get(&A[rank - 1], 1, MPI_INT, 0, K - rank, 1, MPI_INT, win);
    }

    MPI_Win_fence(0, win);

    if (rank)
    {
        pt << A[rank - 1];
    }

    MPI_Win_free(&win);
}
