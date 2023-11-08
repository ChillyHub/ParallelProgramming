#include "pt4.h"

#include "mpi.h"

void Solve()
{
    Task("MPI8Inter6");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Group all, split;
    MPI_Comm_group(MPI_COMM_WORLD, &all);

    int leader = size / 2;
    int ranges[3] = { 0, size / 2 - 1, 1 };
    if (rank >= size / 2)
    {
        ranges[0] = size / 2;
        ranges[1] = size - 1;
        leader = 0;
    }

    MPI_Group_range_incl(all, 1, &ranges, &split);

    MPI_Comm peer, local, inter1, inter2;
    MPI_Comm_dup(MPI_COMM_WORLD, &peer);
    MPI_Comm_create(MPI_COMM_WORLD, split, &local);

    MPI_Intercomm_create(local, 0, peer, leader, 100, &inter1);

    int rank1, size1;
    MPI_Comm_size(local, &size1);
    MPI_Comm_rank(local, &rank1);
    
    pt << rank1;
    
    rank1 = rank < size / 2 ? rank1 : size1 - rank1;
    
    MPI_Comm_split(inter1, rank1 % 2, rank1, &inter2);
    
    int rank2, size2;
    MPI_Comm_size(inter2, &size2);
    MPI_Comm_rank(inter2, &rank2);
    
    pt << rank2;
    
    double A, B;
    pt >> A;
    
    MPI_Status s;
    MPI_Send(&A, 1, MPI_DOUBLE, rank2, 0, inter2);
    MPI_Recv(&B, 1, MPI_DOUBLE, rank2, 0, inter2, &s);
    
    pt << B;
}
