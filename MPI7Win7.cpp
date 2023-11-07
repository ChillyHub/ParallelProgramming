#include "pt4.h"

#include "mpi.h"

void Solve()
{
    Task("MPI7Win7");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int K = size - 1;

    int A[2];

    int intSize;
    MPI_Type_size(MPI_INT, &intSize);

    MPI_Win win;
    MPI_Win_create(A, 2 * intSize, intSize, MPI_INFO_NULL, MPI_COMM_WORLD, &win);

    for (int i = 0; i < K; ++i)
    {
        MPI_Win_fence(0, win);

        if (!rank)
        {
            pt >> A[0] >> A[1];

            MPI_Put(A, 2, MPI_INT, i + 1, 0, 2, MPI_INT, win);
        }

        MPI_Win_fence(0, win);

        if (rank == i + 1)
        {
            pt << A[0] << A[1];
        }
    }

    MPI_Win_free(&win);
}
