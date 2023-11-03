#include "pt4.h"

#include "mpi.h"

void Solve()
{
    Task("MPI2Send7");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Status s;

    if (rank == 0)
    {
        int N;
        pt >> N;

        double nums[10];
        for (int i = 0; i < N; ++i)
        {
            pt >> nums[i];
        }

        for (int i = 1; i < size - 1; ++i)
        {
            MPI_Send(&nums[i - 1], 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
        }
        MPI_Send(&nums[size - 2], N - size + 2, MPI_DOUBLE, size - 1, 0, MPI_COMM_WORLD);
    }
    else if (rank == size - 1)
    {
        double nums[10];
        MPI_Recv(nums, 10, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &s);

        int count;
        MPI_Get_count(&s, MPI_DOUBLE, &count);

        for (int i = 0; i < count; ++i)
        {
            pt << nums[i];
        }
    }
    else
    {
        double num;
        MPI_Recv(&num, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &s);
        pt << num;
    }
}
