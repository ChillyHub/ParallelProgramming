#include "pt4.h"

#include "mpi.h"

void Solve()
{
    Task("MPI5Comm25");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Comm comm;

    const int dims[] = { size };
    constexpr int periods[] = { true };
    MPI_Cart_create(MPI_COMM_WORLD, 1, dims, periods, false, &comm);

    int rankSrc, rankDst;
    MPI_Cart_shift(comm, 0, -1, &rankSrc, &rankDst);

    double in, out;
    if (rank)
    {
        pt >> in;

        MPI_Send(&in, 1, MPI_DOUBLE, rankDst, 0, comm);
    }
    if (rankSrc)
    {
        MPI_Status s;
        MPI_Recv(&out, 1, MPI_DOUBLE, rankSrc, 0, comm, &s);

        pt << out;
    }
}
