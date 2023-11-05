#include "pt4.h"

#include "mpi.h"

void Solve()
{
    Task("MPI5Comm10");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int N;
    pt >> N;

    int A;
    pt >> A;

    MPI_Comm comm;
    MPI_Comm_split(MPI_COMM_WORLD, N == 1 ? 0 : 1, rank, &comm);

    MPI_Comm_size(comm, &size);
    MPI_Comm_rank(comm, &rank);

    std::vector<int> res(size);

    MPI_Allgather(&A, 1, MPI_INT, res.data(), 1, MPI_INT, comm);

    std::copy(res.begin(), res.end(), ptout_iterator<int>());
}
