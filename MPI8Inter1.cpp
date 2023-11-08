#include "pt4.h"

#include "mpi.h"

void Solve()
{
    Task("MPI8Inter1");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Group all, split;
    MPI_Comm_group(MPI_COMM_WORLD, &all);

    int leader = 0;
    int ranges[3] = { 0, size - 2, 2 };
    if (rank % 2)
    {
        ranges[0] += 1;
        ranges[1] += 1;
        leader = 1;
    }

    MPI_Group_range_incl(all, 1, &ranges, &split);

    MPI_Comm peer, inter, comm;
    MPI_Comm_dup(MPI_COMM_WORLD, &peer);
    MPI_Comm_create(MPI_COMM_WORLD, split, &comm);

    MPI_Intercomm_create(comm, 0, peer, (int)!leader, 100, &inter);

    MPI_Comm_size(inter, &size);
    MPI_Comm_rank(inter, &rank);

    int X, Y;
    pt >> X;

    MPI_Status s;
    MPI_Send(&X, 1, MPI_INT, rank, 0, inter);
    MPI_Recv(&Y, 1, MPI_INT, rank, 0, inter, &s);

    pt << rank << Y;
}
