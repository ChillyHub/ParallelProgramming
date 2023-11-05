#include "pt4.h"

#include "mpi.h"

void Solve()
{
    Task("MPI5Comm13");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Comm comm;

    int N;
    if (rank == 0)
    {
        pt >> N;
    }

    MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);

    int dims[] = {N, size / N};
    int periods[] = {false, false};
    MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, false, &comm);

    int coords[2];
    MPI_Cart_coords(comm, rank, 2, coords);

    pt << coords[0] << coords[1];
}
