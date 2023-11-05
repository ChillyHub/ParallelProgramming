#include "pt4.h"

#include "mpi.h"

void Solve()
{
    Task("MPI5Comm20");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Comm commCart, commSub;

    int num;
    pt >> num;

    const int dims[] = { 2, 2, size / 4 };
    const int periods[] = { false, false, false };
    MPI_Cart_create(MPI_COMM_WORLD, 3, dims, periods, false, &commCart);

    const int remainDims[] = { true, true, false };
    MPI_Cart_sub(commCart, remainDims, &commSub);
    MPI_Comm_size(commSub, &size);
    MPI_Comm_rank(commSub, &rank);

    int res[4];
    MPI_Gather(&num, 1, MPI_INT, res, 1, MPI_INT, 0, commSub);

    if (rank == 0)
    {
        pt << res[0] << res[1] << res[2] << res[3];
    }
}
