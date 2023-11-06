#include "pt4.h"

#include "mpi.h"

void Solve()
{
    Task("MPI6File12");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    char name[20];

    if (!rank)
    {
        pt >> name;
    }

    MPI_Bcast(name, 20, MPI_CHAR, 0, MPI_COMM_WORLD);

    int n;
    pt >> n;
    int color = n ? 0 : MPI_UNDEFINED;

    MPI_Comm comm;
    MPI_Comm_split(MPI_COMM_WORLD, color, rank, &comm);

    if (comm == MPI_COMM_NULL)
    {
	    return;
    }
    MPI_Comm_size(comm, &size);
    MPI_Comm_rank(comm, &rank);

    int doubleSize;
    MPI_Type_size(MPI_DOUBLE, &doubleSize);

    MPI_File f;
    MPI_File_open(comm, name, MPI_MODE_RDWR, MPI_INFO_NULL, &f);

    double dst = rank;
    MPI_Offset offset = doubleSize * (n - 1);
    MPI_File_write_at_all(f, offset, &dst, 1, MPI_DOUBLE, MPI_STATUS_IGNORE);

    MPI_File_close(&f);
}
