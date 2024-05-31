#include <iostream>
#include <cstdio>
#include <cstdlib>

void
open_and_read (void)
{
  char  line[256];
  FILE* fp = std::fopen("exemple.txt", "r");

  if (fp == nullptr)
    {
      std::cerr << "Error while opening file for readingg!"
		<< std::endl;
      exit (EXIT_FAILURE);
    }

  
  while (std::fgets (line, sizeof(line), fp))
    std::cout << line;

  std::fclose (fp);

}

void
open_and_write (void)
{
  FILE* fp = std::fopen("exemple.txt", "w");

  if (fp == nullptr)
    {
      std::cerr << "Error while opening file for writing!"
		<< std::endl;
      exit (EXIT_FAILURE);
    }

  std::fprintf(fp, "Hello, world\n");
  std::fclose(fp);
}

int
main (void)
{
  open_and_write ();
  open_and_read  ();
  return 0;
}
