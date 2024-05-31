#include <iostream>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/mman.h>
#include <cstring>

using namespace std;

struct Record
{
  int  id;
  char name[80];
};

int
main()
{
  int fd = open("arquivo.txt", O_RDONLY);

  if (fd == -1)
    {
      cerr << "Error to open the file!" << endl;
      return -1;
    }

  off_t size = lseek(fd, 0, SEEK_END);

  lseek(fd, 0, SEEK_SET);

  Record* records = (Record*) mmap(
		NULL, size,
		PROT_READ, MAP_PRIVATE,
		fd, 0);
  if (records == MAP_FAILED)
    {
      cerr << "Error while trying to map file!" << endl;
      close (fd);
      return -1;
    }

  sscanf(records->name, "%d", &records->id);
  printf("%s", records->name);

  msync(records, size, MS_SYNC);
  munmap(records, size);
  
  close(fd);

  return 0;
}
