#include <cstdio>
#include <cstring>
#include <cstdlib>
#include "refFinder.h"
#include <ctype.h>
#define LENS 100000

int main(int argc, char **argv){
  fprintf(stderr,"pipe into: ./refFinder hg19.fa [inputIsZero full]\n");
  if(argc==1)
    return 0;
  char *refname = argv[1];
  argv+=2;

  int adjust = 1;
  int full =0;
  int toUpper =0;
  argc-=2;
  while(argc>0 &&*argv){
    //fprintf(stderr,"*arg:%s\n",*argv);
    if(!strcasecmp("inputIsZero",*argv)) adjust =0;
    else if(!strcasecmp("full",*argv)) full =1;
    else if(!strcasecmp("toupper",*argv)) toUpper =1;
    else{
      fprintf(stderr,"problem understanding: \'%s\' options are inputIsZero full\n",*argv);
      return 0;
    }
    argv++;
    argc--;
  }
  fprintf(stderr,"Assuming your fasta is: '%s' and we will subtract each position in input with:%d and full:%d\n",refname,adjust,full);
  
  perFasta *ref = init(refname);
  char *buf = new char[LENS];

  while(fgets(buf,LENS,stdin)){
    // fprintf(stderr,"buf: %s\n",buf);
    if(buf[0]=='\n')
      continue;
    char *chr = strtok(buf,"\t\n ");
    char *posStr= strtok(NULL,"\t\n ");
    int pos = atoi(posStr);
    // fprintf(stderr,"\t-> chr:%s\tpos:%d\n",chr,pos);
    
    if(full)
      fprintf(stdout,"%s\t%d\t",chr,pos);
    char refchar = getchar(chr,pos-adjust,ref);
    if(toUpper==0)
      fprintf(stdout,"%c\n",refchar);
    else
      fprintf(stdout,"%c\n",toupper(refchar));
  }
  
  delete [] buf;
  destroy(ref);
  

  return 0;
}
