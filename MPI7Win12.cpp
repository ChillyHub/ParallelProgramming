#include "pt4.h"

#include "mpi.h"

#include <vector>

void Solve()
{
    Task("MPI7Win12");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<int> A(size / 2);

    int intSize, num;
    MPI_Type_size(MPI_INT, &intSize);

    MPI_Win win;
    MPI_Win_create(A.data(), A.size() * intSize, intSize, MPI_INFO_NULL, MPI_COMM_WORLD, &win);

    MPI_Win_fence(0, win);

    if (rank % 2 == 0)
    {
        for (int i = 0; i < size; i += 2)
        {
            pt >> A[i / 2];

            MPI_Put(&A[i / 2], 1, MPI_INT, i + 1, rank / 2, 1, MPI_INT, win);
        }
    }

    MPI_Win_fence(0, win);

    if (rank % 2)
    {
        for (int j = 0; j < size / 2; ++j)
        {
            pt >> num;
            pt << num + A[j];
        }
    }

    MPI_Win_free(&win);
}
