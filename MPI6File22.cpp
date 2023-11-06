#include "pt4.h"

#include "mpi.h"

void Solve()
{
    Task("MPI6File22");
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

    int intSize;
    MPI_Type_size(MPI_INT, &intSize);

    MPI_Datatype t1, t2;
    MPI_Type_vector(3, 1, size, MPI_INT, &t1);
    MPI_Type_create_resized(t1, 0, 3 * intSize * size, &t2);

    MPI_File f;
    MPI_File_open(MPI_COMM_WORLD, name, MPI_MODE_CREATE | MPI_MODE_WRONLY, MPI_INFO_NULL, &f);

    MPI_Offset offset = intSize * (size - rank - 1);
    MPI_File_set_view(f, offset, MPI_INT, t2, "native", MPI_INFO_NULL);

    int ABC[3];
    pt >> ABC[0] >> ABC[1] >> ABC[2];
    MPI_File_write_all(f, ABC, 3, MPI_INT, MPI_STATUS_IGNORE);

    MPI_File_close(&f);
}
