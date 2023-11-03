#include "pt4.h"

#include "mpi.h"

#include <vector>

void Solve()
{
    Task("MPI3Coll24");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int dst;
    std::vector<int> src(size);
    std::vector<int> counts(size, 1);
    for (int i = 0; i < size; ++i)
    {
        pt >> src[i];
    }

    MPI_Reduce_scatter(src.data(), &dst, counts.data(), MPI_INT, MPI_SUM, MPI_COMM_WORLD);

    pt << dst;
}
