#include <iostream>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/uio.h>
#include <string.h>
#include <vector>

#define FILENAME "exemplo.txt"

struct Data
{
  int id;
  char name[20];
  float value;
};

void
write_bin_iov(const std::vector<Data> &data_list)
{
  int fd;
  ssize_t bytes_written;
  
  if ((fd = open(FILENAME,
		 O_WRONLY | O_CREAT | O_APPEND,
		 S_IRUSR | S_IWUSR)) == -1)
    {
      std::cerr << "Error while trying to create the file\n";
      exit (EXIT_FAILURE);
    }

  std::vector<struct iovec> iov;
  
  for (const auto &data : data_list)
    {
      struct iovec entry[3];

      entry[0].iov_base = (void*)&data.id;
      entry[0].iov_len  = sizeof (data.id);

      entry[1].iov_base = (void*)&data.name;
      entry[1].iov_len  = sizeof(data.name);

      entry[2].iov_base = (void*)&data.value;
      entry[2].iov_len = sizeof(data.value);

      iov.insert(iov.end(), std::begin(entry), std::end(entry));
    }

  bytes_written = writev (fd, iov.data(), iov.size());

  if (bytes_written == -1)
    {
      std::cerr << "Error while trying to write to file\n";
      exit (EXIT_FAILURE);
    }

  close (fd);

  std::cout << "Data written with success" << std::endl;
}

void
write_bin(const Data &data)
{
  int fd;
  ssize_t bytes_written;

  if ((fd = open(FILENAME,
		 O_WRONLY | O_CREAT | O_APPEND,
		 S_IRUSR | S_IWUSR)) == -1)
    {
      std::cerr << "Error while trying to create the file\n";
      exit (EXIT_FAILURE);
    }

  bytes_written = write(fd, &data, sizeof(Data));

  if (bytes_written != sizeof(Data))
    {
      std::cerr << "Error while trying to write to file\n";
      exit (EXIT_FAILURE);
    }

  close(fd);

  std::cout << "Data written with success" << std::endl;
}

void
mmap_load(void)
{
  int fd;
  
  struct stat file_stat;
  size_t file_size, n_elements;

  if ((fd = open (FILENAME, O_RDONLY)) == -1)
    {
      std::cerr << "Error while opening the file.\n";
      return;
    }


  fstat (fd, &file_stat);
  file_size = file_stat.st_size;

  Data* data = (Data*) mmap(nullptr, file_size, PROT_READ, MAP_PRIVATE, fd, 0);

  close (fd);
  
  if (data == MAP_FAILED)
    {

      std::cerr << "Error while mapping the file in memmory.\n";
      return;
    }

  n_elements = file_size / sizeof(Data);

  for (size_t i = 0; i < n_elements; ++i)
    {
      std::cout << "ID:    " << data[i].id << std::endl;
      std::cout << "Name:  " << data[i].name << std::endl;
      std::cout << "Value: " << data[i].value << std::endl;
    }

  munmap(data, file_size);
}

void
ex_01 (void)
{
  Data data1 = {1, "Exemplo", 3.14};
  Data data2 = {2, "Exemplo2", 2.71};

  write_bin (data1);
  write_bin (data2);

  mmap_load ();
}

void
ex_02 (void)
{
  std::vector<Data> data_list = {
    {1, "Exemplo", 3.14},
    {2, "Exemplo2", 2.71},
  };

  write_bin_iov(data_list);

  mmap_load();
}


int
main (void)
{
  ex_02 ();
  return 0;
}
