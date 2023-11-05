#include "pt4.h"

#include "mpi.h"

#include <vector>

void Solve()
{
    Task("MPI5Comm30");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int N = (size - 1) / 3;
    int iSize = size;
    int eSize = N * (2 + 3 * 2);

    std::vector<int> indices(iSize);
    std::vector<int> edges(eSize);

    int ind = 0;
    for (int i = 0; i < N; ++i)
    {
        edges[ind++] = 3 * (i + 1);
    }
    indices[0] = ind;

    for (int i = 0; i < N; ++i)
    {
	    for (int j = 0; j < 3; ++j)
	    {
            if (j == 2)
            {
                edges[ind++] = 0;
            }
		    for (int k = 0; k < 3; ++k)
		    {
			    if (j != k)
			    {
                    edges[ind++] = i * 3 + k + 1;
			    }
		    }
            indices[i * 3 + j + 1] = ind;
	    }
    }

    MPI_Comm comm;
    MPI_Graph_create(MPI_COMM_WORLD, size, indices.data(), edges.data(), false, &comm);

    int count;
    MPI_Graph_neighbors_count(comm, rank, &count);

    std::vector<int> neighbors(count);
    MPI_Graph_neighbors(comm, rank, count, neighbors.data());

    MPI_Status s;

    int A;
    pt >> A;

    for (int i = 0; i < count; ++i)
    {
        int dst;
        MPI_Sendrecv(&A, 1, MPI_INT, neighbors[i], 0, &dst, 1, MPI_INT, neighbors[i], 0, comm, &s);
        
        pt << dst;
    }
}
