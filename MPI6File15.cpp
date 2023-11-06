#include "pt4.h"

#include "mpi.h"

void Solve()
{
    Task("MPI6File15");
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

    MPI_Comm comm;

    int color = rank % 2 ? 0 : MPI_UNDEFINED;
    MPI_Comm_split(MPI_COMM_WORLD, color, size - rank, &comm);

    if (comm == MPI_COMM_NULL)
    {
	    return;
    }
    MPI_Comm_size(comm, &size);
    MPI_Comm_rank(comm, &rank);

    int doubleSize;
    MPI_Type_size(MPI_DOUBLE, &doubleSize);

    MPI_File f;
    MPI_File_open(comm, name, MPI_MODE_RDONLY, MPI_INFO_NULL, &f);

    MPI_Offset offset = 2 * doubleSize * size;
    MPI_File_seek_shared(f, -offset, MPI_SEEK_END);

    double dst[2];
    MPI_File_read_ordered(f, dst, 2, MPI_DOUBLE, MPI_STATUS_IGNORE);

    MPI_File_close(&f);

    pt << dst[0] << dst[1];
}
