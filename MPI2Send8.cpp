#include "pt4.h"

#include "mpi.h"

void Solve()
{
    Task("MPI2Send8");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Status s;

    if (rank)
    {
        int n;
        pt >> n;

        if (n)
        {
            int data[2] = { n, rank };
            MPI_Send(data, 2, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
    }
    else
    {
        int data[2];
        MPI_Recv(data, 2, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &s);

        pt << data[0] << data[1];
    }
}
