#include "pt4.h"

#include "mpi.h"

#include <vector>

void Solve()
{
    Task("MPI3Coll18");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<double> dst(size * 2);
    std::vector<double> src(size + 1);
    for (int i = 0; i <= size; ++i)
    {
        pt >> src[i];
    }

    std::vector<int> sCounts(size, 2);
    std::vector<int> rCounts(size, 2);
    std::vector<int> sOffsets(size);
    std::vector<int> rOffsets(size);
    for (int i = 0; i < size; ++i)
    {
        sOffsets[i] = size - i - 1;
        rOffsets[i] = i * 2;
    }

    MPI_Alltoallv(src.data(), sCounts.data(), sOffsets.data(), MPI_DOUBLE, dst.data(), rCounts.data(), rOffsets.data(), MPI_DOUBLE, MPI_COMM_WORLD);

    for (int i = 0; i < size * 2; ++i)
    {
        pt << dst[i];
    }
}
