#include "pt4.h"

#include "mpi.h"

#include <vector>

void Solve()
{
    Task("MPI7Win18");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int A;

    int intSize;
    MPI_Type_size(MPI_INT, &intSize);

    MPI_Win win;
    MPI_Win_create(&A, intSize, intSize, MPI_INFO_NULL, MPI_COMM_WORLD, &win);

    MPI_Group all, split;
    MPI_Comm_group(MPI_COMM_WORLD, &all);

    std::vector<int> evenRanks(size / 2);
    for (int i = 0; i < size / 2; ++i)
    {
        evenRanks[i] = i * 2;
    }

    if (rank % 2)
    {
        MPI_Group_incl(all, evenRanks.size(), evenRanks.data(), &split);

        MPI_Win_post(split, 0, win);
        MPI_Win_wait(win);
        
        pt << A;
    }
    else
    {
        pt >> A;

        MPI_Group_excl(all, evenRanks.size(), evenRanks.data(), &split);

        MPI_Win_start(split, 0, win);
        MPI_Put(&A, 1, MPI_INT, rank + 1, 0, 1, MPI_INT, win);
        MPI_Win_complete(win);
    }

    MPI_Group_free(&split);
    MPI_Group_free(&all);
    MPI_Win_free(&win);
}
