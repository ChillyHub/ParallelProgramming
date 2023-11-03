#include "pt4.h"

#include "mpi.h"

void Solve()
{
    Task("MPI2Send17");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Status s;
    int num;

    for (int i = 0; i < size; ++i)
    {
	    if (i < rank)
	    {
            pt >> num;
            MPI_Ssend(&num, 1, MPI_INT, i, 0, MPI_COMM_WORLD);

            MPI_Recv(&num, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &s);
            pt << num;
	    }
	    else if (i > rank)
	    {
            MPI_Recv(&num, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &s);
            pt << num;

            pt >> num;
            MPI_Ssend(&num, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
	    }
    }
}
