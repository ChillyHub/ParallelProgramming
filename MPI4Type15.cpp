#include "pt4.h"

#include "mpi.h"

#include <vector>

void Solve()
{
    Task("MPI4Type15");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int K = size - 1;

    if (rank)
    {
        MPI_Status s;
        std::vector<double> col(K);

        MPI_Recv(col.data(), K, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &s);

        std::copy(col.begin(), col.end(), ptout_iterator<double>());
    }
    else
    {
        std::vector<double> mat(ptin_iterator<double>(K * K), ptin_iterator<double>());

        MPI_Datatype ColDouble;
        int sizeDouble;

        MPI_Type_size(MPI_DOUBLE, &sizeDouble);
        MPI_Type_create_resized(MPI_DOUBLE, 0, sizeDouble * K, &ColDouble);
        MPI_Type_commit(&ColDouble);

        for (int i = 1; i < size; ++i)
        {
            MPI_Send(&mat[i-1], K, ColDouble, i, 0, MPI_COMM_WORLD);
        }
    }
}
