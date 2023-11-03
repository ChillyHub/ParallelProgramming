#include "pt4.h"

#include "mpi.h"

void Solve()
{
    Task("MPI2Send21");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Status s;

    if (rank)
    {
        int L;
        pt >> L;

        int N;
        double A;
        for (int i = 0; i < L; ++i)
        {
            pt >> A >> N;

            MPI_Send(&A, 1, MPI_DOUBLE, 0, N, MPI_COMM_WORLD);
        }
    }
    else
    {
        double A;
	    for (int i = 0; i < 2 * size; ++i)
	    {
            MPI_Recv(&A, 1, MPI_DOUBLE, MPI_ANY_SOURCE, i + 1, MPI_COMM_WORLD, &s);

            pt << A;
	    }
    }
}
