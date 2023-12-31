#include "pt4.h"

#include "mpi.h"

#include <vector>

void Solve()
{
    Task("MPI1Proc8");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int N;
    pt >> N;

    double sum = 0;
    for (int i = 0; i < N; i++)
    {
        double num;
        pt >> num;

        sum += num;
    }

    if (rank % 2)
    {
        pt << sum / N;
    }
    else
    {
        pt << sum;
    }
}
