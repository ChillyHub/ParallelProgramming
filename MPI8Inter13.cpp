#include "pt4.h"

#include "mpi.h"

void Solve()
{
    Task("MPI8Inter13");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int C;
    pt >> C;

    const int color = C ? C : MPI_UNDEFINED;

    MPI_Comm peer, local, inter;
    MPI_Comm_dup(MPI_COMM_WORLD, &peer);
    MPI_Comm_split(MPI_COMM_WORLD, color, rank, &local);

    if (local == MPI_COMM_NULL)
    {
        pt << -1;
        return;
    }

    const int leader = C == 1 ? size / 2 : 0;
    MPI_Intercomm_create(local, 0, peer, leader, 100, &inter);

    int remoteSize;
    MPI_Comm_remote_size(inter, &remoteSize);
    MPI_Comm_size(inter, &size);
    MPI_Comm_rank(inter, &rank);

    pt << rank;

    double X, Y;
    pt >> X;

    MPI_Op op = C == 1 ? MPI_MAX : MPI_MIN;
    MPI_Allreduce(&X, &Y, 1, MPI_DOUBLE, op, inter);

    pt << Y;
}
