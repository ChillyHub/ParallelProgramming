#include "pt4.h"

#include "mpi.h"

void Solve()
{
    Task("MPI5Comm24");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int M, N;

    if (!rank)
    {
        pt >> M >> N;
    }

    MPI_Bcast(&M, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (M * N <= rank)
    {
	    return;
    }

    int X, Y;
    pt >> X >> Y;

    MPI_Comm comm;

    const int dims[] = { M, N };
    constexpr int periods[] = { false, true };
    MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, false, &comm);
    MPI_Comm_set_errhandler(comm, MPI_ERRORS_RETURN);

    const int coords[] = { X, Y };
    if (MPI_Cart_rank(comm, coords, &rank))
    {
        pt << -1;
    }
    else
    {
        pt << rank;
    }
}
