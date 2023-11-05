#include "pt4.h"

#include "mpi.h"

void Solve()
{
    Task("MPI5Comm7");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int N;
    pt >> N;

    MPI_Comm comm;
    MPI_Comm_split(MPI_COMM_WORLD, N ? 0 : MPI_UNDEFINED, rank, &comm);

    if (comm == MPI_COMM_NULL)
    {
	    return;
    }
    MPI_Comm_size(comm, &size);
    MPI_Comm_rank(comm, &rank);

    double A;
    pt >> A;

    std::vector<double> res(size);

    MPI_Gather(&A, 1, MPI_DOUBLE, res.data(), 1, MPI_DOUBLE, 0, comm);

    if (rank == 0)
    {
        std::copy(res.begin(), res.end(), ptout_iterator<double>());
    }
}
