#include <iostream>
#include <cstring>
#include <cerrno>
#include <unistd.h>
#include <fcntl.h>

#define BSIZE 1024

int
ex_open_and_write_to_file ()
{

  char buf[BSIZE] = "Hello, world ";
  int fd = open ("exemplo.txt", O_WRONLY | O_CREAT, 0644);

  buf[strlen (buf) - 1] = '\0';
  
  if (fd == -1)
    {
      std::cerr << "Error while opening the file."
		<< std::endl
		<< std::strerror(errno)
		<< std::endl;
      return 1;
    }

  if (write (fd, buf, strlen (buf)) == -1)
    {
      std::cerr << "Error while writing the file" << std::endl;
      close (fd);
      return 1;
    }

  close (fd);

  std::cout << "File was wroted with success" << std::endl;
  
  return 0;
}

int
ex_open_reading_file ()
{

  char buf[BSIZE]; 
  int  fd; 

  fd = open ("exemplo.txt", O_RDONLY);
  
  if (fd == -1)
    {
      std::cerr << "Error while opening the file."
		<< std::endl
		<< std::strerror (errno)
		<< std::endl;
      return -1;
    }

  if (read (fd, buf, BSIZE) == -1)  
    return -1;
  else
    std::cout << buf << std::endl;
  
  return 0;
}


int
main (void)
{
  ex_open_and_write_to_file ();
  ex_open_reading_file ();
  return 0;
}
