#include "pt4.h"

#include "mpi.h"

#include <vector>

void Solve()
{
    Task("MPI7Win21");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<double> A(size / 2);
    std::vector<int> N;

    int doubleSize;
    MPI_Type_size(MPI_DOUBLE, &doubleSize);

    MPI_Win win;
    MPI_Win_create(A.data(), A.size() * doubleSize, doubleSize, MPI_INFO_NULL, MPI_COMM_WORLD, &win);

    MPI_Group all, main;
    MPI_Comm_group(MPI_COMM_WORLD, &all);

    int mainRank = 0;

    if (rank)
    {
        MPI_Group_incl(all, 1, &mainRank, &main);

        MPI_Win_post(main, 0, win);
        MPI_Win_wait(win);

        pt << A[0];
    }
    else
    {
        A = std::vector<double>(ptin_iterator<double>(size / 2), ptin_iterator<double>());
        N = std::vector<int>(ptin_iterator<int>(size / 2), ptin_iterator<int>());

        MPI_Group_excl(all, 1, &mainRank, &main);

        MPI_Win_start(main, 0, win);
        for (int i = 0; i < N.size(); ++i)
        {
            MPI_Put(&A[i], 1, MPI_DOUBLE, N[i], 0, 1, MPI_DOUBLE, win);
        }
        MPI_Win_complete(win);
    }

    MPI_Group_free(&main);
    MPI_Group_free(&all);
    MPI_Win_free(&win);
}
