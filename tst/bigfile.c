/* Create a 6 giga byte file */

#include <stdio.h>

int main(int argc, char **argv)
{
  int i ;

  for(i=0; i<6; i++)
    if ( fseek(stdout, 1000000000, SEEK_CUR) )
      {
	perror("Creating a 6Gb file\n") ;
	return 1 ;
      }

  printf("eof\n") ;
  return 0 ;
}
