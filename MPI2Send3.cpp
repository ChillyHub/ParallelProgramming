#include "pt4.h"

#include "mpi.h"

void Solve()
{
    Task("MPI2Send3");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Status s;
    int nums[4];
    
    if (rank)
    {
	    for (int i = 0; i < 4; ++i)
	    {
            pt >> nums[i];
	    }

        MPI_Send(nums, 4, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
    else
    {
	    for (int i = 1; i < size; ++i)
	    {
            MPI_Recv(nums, 4, MPI_INT, i, 0, MPI_COMM_WORLD, &s);

            for (int j = 0; j < 4; ++j)
            {
                pt << nums[j];
            }
	    }
    }
}
