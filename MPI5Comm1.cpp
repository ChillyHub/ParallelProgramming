#include "pt4.h"

#include "mpi.h"

#include <vector>

void Solve()
{
    Task("MPI5Comm1");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Comm comm;
    MPI_Group groupAll, groupEven;

    std::vector<int> ranks;
    for (int i = 0; i < size; ++i)
    {
	    if (i % 2)
	    {
		    continue;
	    }

        ranks.push_back(i);
    }

    MPI_Comm_group(MPI_COMM_WORLD, &groupAll);
    MPI_Group_incl(groupAll, ranks.size(), ranks.data(), &groupEven);
    MPI_Comm_create(MPI_COMM_WORLD, groupEven, &comm);

    if (comm == MPI_COMM_NULL)
    {
	    return;
    }

    int res;
    std::vector<int> src;
    std::vector<int> counts(ranks.size(), 1);
    std::vector<int> offsets(ranks.size());
    for (unsigned int i = 0; i < offsets.size(); ++i)
    {
        offsets[i] = static_cast<int>(i);
    }

    if (rank == 0)
    {
        src = std::vector<int>(ptin_iterator<int>(ranks.size()), ptin_iterator<int>());
    }

    MPI_Scatterv(src.data(), counts.data(), offsets.data(), MPI_INT, &res, 1, MPI_INT, 0, comm);

    pt << res;
}
