#include "pt4.h"

#include "mpi.h"

#include <vector>

void Solve()
{
    Task("MPI4Type11");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int position;
    const int packSize = (size - 1) * (sizeof(int) * 2 + sizeof(double));

    int t1;
    int t2;
    double t3;
    std::vector<char> pack(packSize / sizeof(char));

    if (rank)
    {
        MPI_Status s;

        MPI_Recv(pack.data(), packSize, MPI_PACKED, 0, 0, MPI_COMM_WORLD, &s);

        position = 0;
        for (int i = 0; i < size - 1; ++i)
        {
            MPI_Unpack(pack.data(), packSize, &position, &t1, 1, MPI_INT, MPI_COMM_WORLD);
            MPI_Unpack(pack.data(), packSize, &position, &t2, 1, MPI_INT, MPI_COMM_WORLD);
            MPI_Unpack(pack.data(), packSize, &position, &t3, 1, MPI_DOUBLE, MPI_COMM_WORLD);

            pt << t1 << t2 << t3;
        }
    }
    else
    {
        position = 0;
	    for (int i = 0; i < size - 1; ++i)
	    {
            pt >> t1 >> t2 >> t3;

            MPI_Pack(&t1, 1, MPI_INT, pack.data(), packSize, &position, MPI_COMM_WORLD);
            MPI_Pack(&t2, 1, MPI_INT, pack.data(), packSize, &position, MPI_COMM_WORLD);
            MPI_Pack(&t3, 1, MPI_DOUBLE, pack.data(), packSize, &position, MPI_COMM_WORLD);
	    }

	    for (int i = 1; i < size; ++i)
	    {
            MPI_Send(pack.data(), packSize, MPI_PACKED, i, 0, MPI_COMM_WORLD);
	    }
    }
}
