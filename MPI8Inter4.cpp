#include "pt4.h"

#include "mpi.h"

void Solve()
{
    Task("MPI8Inter4");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int C;
    pt >> C;

    MPI_Comm peer, local;
    MPI_Comm inter[3];

    MPI_Comm_dup(MPI_COMM_WORLD, &peer);
    MPI_Comm_split(MPI_COMM_WORLD, C, rank, &local);

    const int leader1 = C ? 0 : 1;
    const int leader2 = C - 2 ? 2 : 1;
    const int index1 = leader1 + C - 1;
    const int index2 = leader2 + C - 1;

    MPI_Intercomm_create(local, 0, peer, leader1, (index1 + 1) * 100, &inter[index1]);
    MPI_Intercomm_create(local, 0, peer, leader2, (index2 + 1) * 100, &inter[index2]);
    
    MPI_Comm_size(local, &size);
    MPI_Comm_rank(local, &rank);
    
    int X, Y, Z;
    pt >> X >> Y;
    
    pt << rank;
    
    MPI_Status s;

    const int i1 = C - 1 ? index1 : index2;
    const int i2 = C - 1 ? index2 : index1;
    
    MPI_Send(&Y, 1, MPI_INT, rank, 0, inter[i1]);
    MPI_Send(&X, 1, MPI_INT, rank, 0, inter[i2]);

    MPI_Recv(&Z, 1, MPI_INT, rank, 0, inter[i2], &s);
    pt << Z;

    MPI_Recv(&Z, 1, MPI_INT, rank, 0, inter[i1], &s);
    pt << Z;
}
