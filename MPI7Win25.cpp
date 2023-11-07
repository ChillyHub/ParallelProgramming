#include "pt4.h"

#include "mpi.h"

void Solve()
{
    Task("MPI7Win25");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int M;
    double B;
    double A[5];

    int doubleSize;
    MPI_Type_size(MPI_DOUBLE, &doubleSize);

    MPI_Win win;
    MPI_Win_create(A, 5 * doubleSize, doubleSize, MPI_INFO_NULL, MPI_COMM_WORLD, &win);

    if (rank % 3 == 0)
    {
        pt >> A[0] >> A[1] >> A[2] >> A[3] >> A[4];

        MPI_Barrier(MPI_COMM_WORLD);
    }
	else if (rank % 3 == 1)
    {
        MPI_Win_lock(MPI_LOCK_EXCLUSIVE, rank - 1, 0, win);

        pt >> M >> B;
        MPI_Accumulate(&B, 1, MPI_DOUBLE, rank - 1, M, 1, MPI_DOUBLE, MPI_MIN, win);

        MPI_Win_unlock(rank - 1, win);

        MPI_Barrier(MPI_COMM_WORLD);
    }
    else if (rank % 3 == 2)
    {
        MPI_Barrier(MPI_COMM_WORLD);

        MPI_Win_lock(MPI_LOCK_EXCLUSIVE, rank - 2, 0, win);

        MPI_Get(A, 5, MPI_DOUBLE, rank - 2, 0, 5, MPI_DOUBLE, win);

        MPI_Win_unlock(rank - 2, win);

        pt << A[0] << A[1] << A[2] << A[3] << A[4];
    }

    MPI_Win_free(&win);
}
