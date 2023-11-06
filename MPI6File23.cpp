#include "pt4.h"

#include "mpi.h"

void Solve()
{
    Task("MPI6File23");
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

    int doubleSize;
    MPI_Type_size(MPI_DOUBLE, &doubleSize);

    MPI_Datatype t1, t2;
    MPI_Type_vector(2, 3, 3 * size, MPI_DOUBLE, &t1);
    MPI_Type_create_resized(t1, 0, 6 * doubleSize * size, &t2);

    MPI_File f;
    MPI_File_open(MPI_COMM_WORLD, name, MPI_MODE_RDONLY, MPI_INFO_NULL, &f);

    MPI_Offset offset = 3 * doubleSize * rank;
    MPI_File_set_view(f, offset, MPI_DOUBLE, t2, "native", MPI_INFO_NULL);

    double ABCDEF[6];
    MPI_File_read_all(f, ABCDEF, 6, MPI_DOUBLE, MPI_STATUS_IGNORE);

    MPI_File_close(&f);

    for (int i = 0; i < 6; ++i)
    {
        pt << ABCDEF[i];
    }
}
