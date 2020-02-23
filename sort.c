#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

char buf[512];

int main(int argc, char *argv[])
{
  int _r, _o, _n;
  _r = 0;
  _o = 0;
  _n = 0;
  char* files[10];
  int fileNum = 0;
  int i,j;
  for(i = 1; i < argc; i++)
  {
    if(strcmp(argv[i], "-r") == 0)
      _r = 1;
    else if(strcmp(argv[i], "-o") == 0)
      _o = 1;
    else if(strcmp(argv[i], "-n") == 0)
      _n = 1;
    else
      files[fileNum++] = argv[i];
  }
  char *items[100];
  int num = 0;
  int n = 0;
  int index = 0;
  int fd;
  
  if(fileNum == 0)
    fd = 0;
  else if(_o)
    fd = open(files[1], 0);
  else
    fd = open(files[0], 0);

  if(fd < 0)
  {
    printf(1, "can't open ");
    printf(1, files[0]);
    printf(1, "\n");
    return 0;
  }
  while((n = read(fd, buf, sizeof(buf))) > 0){
    while(index < n){
      int length = 0;
      for(; buf[index] != '\n' && index < n; index++)
        length++;
      if(length > 0)
      {
        items[num] = malloc((length + 1) * sizeof(char));
        int start = index - length;
        int indexOfLine = 0;
        for(; indexOfLine + start < index; indexOfLine++)
          items[num][indexOfLine] = buf[indexOfLine+start];
      }
      num++;
      index++;
    }
    index = 0;
  }
  for(i = 0; i < num; i++)
  {
    for(j = i + 1; j < num; j++)
    {
      int sortNum = 0;
      int num1, num2;
      if(_n == 1)
      {
	num1 = atoi(items[i]);
	num2 = atoi(items[j]);
	sortNum = num1 - num2;
      }
      if((((strcmp(items[i], items[j]) > 0 && _r == 0) || (strcmp(items[i], items[j]) < 0 && _r == 1)) && _n == 0) || (_n == 1 && ((sortNum > 0 && _r == 0) || (sortNum < 0 && _r == 1))))
      {
	char *temp;
	temp = items[i];
	items[i] = items[j];
	items[j] = temp;
      }
    }
  }
  int wr = 0;
  if(_o == 1)
  {
    wr = open(files[0], O_WRONLY);
    if(wr < 0)
    {
      wr = open(files[0], O_CREATE | O_RDWR);
      if(wr < 0)
      {
        printf(1, "can't open file...\n");
        return 0;
      }
    }
  }
  for(i = 0; i < num; i++){
    if(_o != 1)
    {
      printf(1, items[i]); 
      printf(1, "\n");
    }
    if(_o == 1)
    {
      write(wr, items[i], strlen(items[i]));
      write(wr, "\n", sizeof(char));
    }
  }
  close(fd);
  if(_o == 1)
    close(wr);
  return 0;
}
