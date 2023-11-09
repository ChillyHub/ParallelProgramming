#include "pt4.h"

#include "mpi.h"

void Solve()
{
    Task("MPI8Inter17");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int root;
    MPI_Comm inter1, inter2;
    MPI_Comm_get_parent(&inter1);

    std::vector<double> A;

    if (inter1 == MPI_COMM_NULL)
    {
        MPI_Comm_spawn("ptprj.exe", NULL, 2, MPI_INFO_NULL, 0, MPI_COMM_WORLD, &inter1, MPI_ERRCODES_IGNORE);

        root = 0;
    }
    else
    {
        root = MPI_ROOT;
    }

    MPI_Comm_split(inter1, rank % 2, rank, &inter2);

    int remoteSize;
    MPI_Comm_rank(inter2, &rank);
    MPI_Comm_remote_size(inter2, &remoteSize);

    if (root == MPI_ROOT)
    {
        A.resize(remoteSize);

        MPI_Status s;
        MPI_Recv(A.data(), A.size(), MPI_DOUBLE, 0, 0, inter2, &s);

        Show(A.begin(), A.end());
    }
    else
    {
        A.resize(size / 2);

	    if (!rank)
	    {
            std::copy(ptin_iterator<double>(size / 2), ptin_iterator<double>(), A.data());
	    }

        MPI_Send(A.data(), A.size(), MPI_DOUBLE, 0, 0, inter2);
    }

    double B;
    MPI_Scatter(A.data(), 1, MPI_DOUBLE, &B, 1, MPI_DOUBLE, root, inter2);

    if (root != MPI_ROOT)
    {
        pt << B;
    }
}
