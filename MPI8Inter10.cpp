#include "pt4.h"

#include "mpi.h"

void Solve()
{
    Task("MPI8Inter10");
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

    int R1, R2;
    pt >> R1 >> R2;
    
    const int src = rank < size / 2 ? R2 : R1;
    const int dst = rank < size / 2 ? R1 : R2;
    
    int remoteSize;
    MPI_Comm_remote_size(inter, &remoteSize);
    MPI_Comm_size(inter, &size);
    MPI_Comm_rank(inter, &rank);
    
    pt << rank;
    
    int X[3], Y[3];
    if (dst == rank)
    {
        pt >> X[0] >> X[1] >> X[2];
        
        for (int i = 0; i < remoteSize; ++i)
        {
            MPI_Send(X, 3, MPI_INT, i, 0, inter);
        }
    }
    
    MPI_Status s;
    MPI_Recv(Y, 3, MPI_INT, src, 0, inter, &s);
    
    pt << Y[0] << Y[1] << Y[2];
}
