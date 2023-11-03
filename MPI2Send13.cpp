#include "pt4.h"

#include "mpi.h"

void Solve()
{
    Task("MPI2Send13");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Status s;

    int num;
    int source = (size + rank + 1) % size;
    int dest = (size + rank - 1) % size;

    pt >> num;

    if (rank)
    {
        MPI_Ssend(&num, 1, MPI_INT, dest, 0, MPI_COMM_WORLD);
        MPI_Recv(&num, 1, MPI_INT, source, 0, MPI_COMM_WORLD, &s);
    }
    else
    {
        int tmp = num;
        MPI_Recv(&num, 1, MPI_INT, source, 0, MPI_COMM_WORLD, &s);
        MPI_Ssend(&tmp, 1, MPI_INT, dest, 0, MPI_COMM_WORLD);
    }

    pt << num;
}
