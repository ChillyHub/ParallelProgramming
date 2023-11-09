#include "pt4.h"

#include "mpi.h"

void Solve()
{
    Task("MPI8Inter20");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int oldRank = rank;

    MPI_Comm peer, inter, interMerge, interCart;
    MPI_Comm_dup(MPI_COMM_WORLD, &peer);
    MPI_Comm_get_parent(&inter);

    bool isParent = false;
    if (inter == MPI_COMM_NULL)
    {
        const int K = size;
        const int K0 = size % 4 ? 4 + size - size % 4 : size;

        MPI_Comm_spawn("ptprj.exe", NULL, K0 - K, MPI_INFO_NULL, 0, MPI_COMM_WORLD, &inter, MPI_ERRCODES_IGNORE);
        MPI_Intercomm_merge(inter, 0, &interMerge);
        isParent = true;
    }
    else
    {
        MPI_Intercomm_merge(inter, 1, &interMerge);
    }

    MPI_Comm_size(interMerge, &size);
    MPI_Comm_rank(interMerge, &rank);

    const int dims[2] = { size / 4, 4 };
    constexpr int periods[2] = { true, true };
    MPI_Cart_create(interMerge, 2, dims, periods, false, &interCart);

    int coords[2];
    MPI_Cart_coords(interCart, rank, 2, coords);

    int A;
    if (isParent)
    {
        pt >> A;
    }
    else
    {
        A = -oldRank - 1;
    }

    int rankSrc, rankDst;
    MPI_Cart_shift(interCart, 0, -1, &rankSrc, &rankDst);

    MPI_Status s;
    MPI_Sendrecv_replace(&A, 1, MPI_INT, rankDst, 0, rankSrc, 0, interCart, &s);

    if (isParent)
    {
        pt << coords[0] << coords[1] << A;
    }
    else
    {
        Show("X = ", coords[0]);
        Show("Y = ", coords[1]);
        Show("A = ", A);
    }
}
