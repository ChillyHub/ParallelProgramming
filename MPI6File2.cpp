#include "pt4.h"

#include "mpi.h"

#include <string>

void Solve()
{
    Task("MPI6File2");
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

    MPI_File f;

    MPI_File_open(MPI_COMM_WORLD, name, MPI_MODE_CREATE | MPI_MODE_WRONLY, MPI_INFO_NULL, &f);

    if (rank)
    {
    	int count;
        pt >> count;
    
        for (int i = 0; i < count; ++i)
        {
            int offset, num;
            pt >> offset >> num;
    
            int intSize;
            MPI_Type_size(MPI_INT, &intSize);
    
            MPI_Offset mOffset = (offset - 1) * intSize;
            MPI_File_write_at(f, mOffset, &num, 1, MPI_INT, MPI_STATUS_IGNORE);
        }
    }

    MPI_File_close(&f);
}
