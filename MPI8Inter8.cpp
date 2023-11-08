#include "pt4.h"

#include "mpi.h"

void Solve()
{
    Task("MPI8Inter8");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Comm peer, local, inter1, inter2;
    MPI_Comm_dup(MPI_COMM_WORLD, &peer);
    MPI_Comm_split(MPI_COMM_WORLD, rank % 2, rank, &local);

    MPI_Intercomm_create(local, 0, peer, rank % 2 ? 0 : 1, 100, &inter1);

    MPI_Comm_rank(inter1, &rank);

    pt << rank;

    int C;
    pt >> C;

    MPI_Comm_split(inter1, C, rank, &inter2);

    int remoteSize;
    MPI_Comm_rank(inter2, &rank);
    MPI_Comm_remote_size(inter2, &remoteSize);

    pt << rank;

    int X, Y;
    pt >> X;

    MPI_Status s;

    for (int i = 0; i < remoteSize; ++i)
    {
        MPI_Send(&X, 1, MPI_INT, i, 0, inter2);
        MPI_Recv(&Y, 1, MPI_INT, i, 0, inter2, &s);

        pt << Y;
    }
}
