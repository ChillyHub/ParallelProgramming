#include "pt4.h"

#include "mpi.h"

void Solve()
{
    Task("MPI2Send18");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Status s;

    int N;
    pt >> N;

    int count;
    double src[5];
    double dst[5];

    int sr = rank % 2 ? rank - 1 : rank + 1;

    for (int i = 0; i < N; ++i)
    {
        pt >> src[i];
    }

    MPI_Sendrecv(src, N, MPI_DOUBLE, sr, 0, dst, 5, MPI_DOUBLE, sr, 0, MPI_COMM_WORLD, &s);
    MPI_Get_count(&s, MPI_DOUBLE, &count);

    for (int i = 0; i < count; ++i)
    {
        pt << dst[i];
    }
}
