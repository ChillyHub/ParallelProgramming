#include "pt4.h"

#include "mpi.h"

#include <vector>

void Solve()
{
    Task("MPI8Inter16");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Comm inter;
    MPI_Comm_get_parent(&inter);

    double B = 0.0;
    std::vector<double> A(size);

    bool isParent = false;
    if (inter == MPI_COMM_NULL)
    {
        MPI_Comm_spawn("ptprj.exe", NULL, size, MPI_INFO_NULL, 0, MPI_COMM_WORLD, &inter, MPI_ERRCODES_IGNORE);

        std::copy(ptin_iterator<double>(size), ptin_iterator<double>(), A.data());
        isParent = true;
    }

    MPI_Reduce_scatter_block(A.data(), &B, 1, MPI_DOUBLE, MPI_MAX, inter);

    if (isParent)
    {
        MPI_Status s;
        MPI_Recv(&B, 1, MPI_DOUBLE, rank, 0, inter, &s);

        pt << B;
    }
    else
    {
        Show(B);

        MPI_Send(&B, 1, MPI_DOUBLE, rank, 0, inter);
    }
}
