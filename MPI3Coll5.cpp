#include "pt4.h"

#include "mpi.h"

#include <vector>

void Solve()
{
    Task("MPI3Coll5");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<int> dst((size + 3) * size / 2);
    std::vector<int> counts(size);
    std::vector<int> offsets(size);
    for (int i = 0; i < size; ++i)
    {
        counts[i] = i + 2;
        offsets[i] = (i + 3) * i / 2;
    }

    std::vector<int> src(rank + 2);
    for (int i = 0; i < rank + 2; ++i)
    {
        pt >> src[i];
    }

    MPI_Gatherv(src.data(), src.size(), MPI_INT, dst.data(), counts.data(), offsets.data(), MPI_INT, 0, MPI_COMM_WORLD);

    for (unsigned int i = 0; i < dst.size() && !rank; ++i)
    {
        pt << dst[i];
    }
}
