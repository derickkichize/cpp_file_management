#include <iostream>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>


struct Data
{
  int id;
  char name[20];
  float value;
};

#define FILENAME "exemplo.txt"

void
write_bin (void)
{
  struct Data data;
  int     fd;
  ssize_t bytes_read;

  if ((fd = open (FILENAME,
		  O_WRONLY | O_CREAT | O_TRUNC,
		  S_IRUSR | S_IWUSR)) == -1)
    {
      perror("open");
      exit (EXIT_FAILURE);
    }

  data.id    = 1;
  strncpy(data.name, "Exemplo", sizeof(data.name));
  data.value = 3.14;

  if ((bytes_read = write (fd, &data, sizeof (data))) == -1)
    {
      perror ("close");
      exit (EXIT_FAILURE);
    }

  printf("Dados escritos com sucesso em %s.\n", FILENAME);
}

void
write_bin_refac (const Data &data)
{
  int fd = open(FILENAME, O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);

  if (fd == -1)
    {
      std::cerr << "Erro ao abrir ou criar o arquivo.\n";
      exit (EXIT_FAILURE);
    }

  ssize_t bytes_written = write(fd, &data, sizeof(data));

  if (bytes_written != sizeof(Data))
    {
      std::cerr << "Erro ao escrever no arquivo.\n";
      exit (EXIT_FAILURE);
    }

  close (fd);
  
  std::cout << "Dados escritos com sucesso em " << FILENAME << ".\n";
}

int
mmap_load (void)
{
  int fd = open ("exemplo.txt", O_RDONLY);

  if (fd == -1)
    {
      std::cerr << "Erro ao abrir o arquivo.\n";
      return 1;
    }

  struct stat file_stat;
  
  fstat (fd, &file_stat);
  size_t file_size = file_stat.st_size;

  Data* data = (Data*) mmap (nullptr, file_size, PROT_READ, MAP_PRIVATE, fd, 0);
  close (fd);

  if (data == MAP_FAILED)
    {
      std::cerr << "Erro ao mapear o arquivo na memória.\n";
      return 1;
    }

  std::cout << "ID: "    << data->id    << std::endl;
  std::cout << "Nome: "  << data->name  << std::endl;
  std::cout << "Valor: " << data->value << std::endl;

  munmap (data, file_size);

  return 0;
}

int
main ()
{
  Data data1 = {1, "Exemplo", 3.14};
  Data data2 = {2, "Exemplo2", 2.71};
  write_bin_refac (data1);
  write_bin_refac (data2);  
  mmap_load ();
}
