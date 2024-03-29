#include <cstdio>
#include <cstring>
#include <cstdlib>
#include "refFinder.h"
#include <ctype.h>
#define LENS 100000

int onlyPrint = 5;

char * get_region(char *extra,int &start,int &stop) {
  if(!extra){
    fprintf(stderr,"Must supply parameter for -r option\n");
    return NULL;
  }
  if(strrchr(extra,':')==NULL){//only chromosomename
    char *ref = extra;
    start = stop = -1;;
    return ref;
  }
  char *tok=NULL;
  tok = strtok(extra,":");

  char *ref = tok;

  start =stop=-1;

  tok = extra+strlen(tok)+1;//tok now contains the rest of the string
 
  if(strlen(tok)==0)//not start and/or stop ex: chr21:
    return ref;
  

  if(tok[0]=='-'){//only contains stop ex: chr21:-stop
    tok =strtok(tok,"-");
    stop = atoi(tok);
  }else{
    //catch single point
    int isProper =0;
    for(size_t i=0;i<strlen(tok);i++)
      if(tok[i]=='-'){
	isProper=1;
	 break;
      }
    //fprintf(stderr,"isProper=%d\n",isProper);
    if(isProper){
      tok =strtok(tok,"-");
      start = atoi(tok)-1;//this is important for the zero offset
      tok = strtok(NULL,"-");
      if(tok!=NULL)
	stop = atoi(tok);
    }else{
      //single point
      stop = atoi(tok);
      start =stop -1;
      
    }
    
  }
  if(stop!=-1&&stop<start){
    fprintf(stderr,"endpoint:%d is larger than startpoint:%d\n",start,stop);
    exit(0);
    
  }
  if(0){
    fprintf(stderr,"[%s] ref=%s,start=%d,stop=%d\n",__FUNCTION__,ref,start,stop);
    exit(0);
  }
  return ref;
}



int main(int argc, char **argv){
  
  if(argc==1){
    fprintf(stderr,"\t-> Pipe into: ./refFinder hg19.fa [inputIsZero full toupper plink]\n");
    fprintf(stderr,"\t-> For outputting .tped file then pipe in bim file\n");
    return 0;
  }
  char *refname = argv[1];
  argv+=2;

  int adjust = 1;
  int full =0;
  int toUpper =0;
  int isplink =0;
  argc-=2;
  while(argc>0 &&*argv){
    //fprintf(stderr,"*arg:%s\n",*argv);
    if(!strcasecmp("inputIsZero",*argv)) adjust = 0;
    else if(!strcasecmp("full",*argv)) full = 1;
    else if(!strcasecmp("toupper",*argv)) toUpper = 1;
    else if(!strcasecmp("plink",*argv)) isplink = 1;
    else{
      fprintf(stderr,"\t-> Problem understanding: \'%s\' options are inputIsZero full\n",*argv);
      return 0;
    }
    argv++;
    argc--;
  }
  fprintf(stderr,"\t-> Assuming your fasta is: '%s' and we will subtract each position in input with:%d and full:%d isplink:%d\n",refname,adjust,full,isplink);
  
  perFasta *ref = init(refname);
  char *buf = new char[LENS];
  char *chr=(char*)malloc(LENS);
  char *snpname=(char*)malloc(LENS);
  int isSpace =1;
  int pos;
  int start,stop;
  int ntok=0;
  while(fgets(buf,LENS,stdin)){
    // fprintf(stderr,"buf: %s\n",buf);
    if(buf[0]=='\n')
      continue;

    if(isplink==0){
      if(strchr(buf,'\t'))
	isSpace = 0;
      if(isSpace)
	ntok = sscanf(buf,"%s %d",chr,&pos);
      else
	ntok = sscanf(buf,"%s\t%d",chr,&pos);
      
      if(ntok==2){
	if(onlyPrint-->0) //decrement compare not pointer
	  fprintf(stderr,"\t-> chr:%s\tpos:%d isSpace(theseperator):%d (this will only be printed: %d times more)\n",chr,pos,isSpace,onlyPrint);
      }else{
	chr = get_region(buf,start,stop);
	fprintf(stderr,"\t-> chr:%s\tstart:%d stpo:%d\n",chr,start,stop);
	start++;
	pos=start;
      }
    }else{
      float dist;
      char al1;
      char al2;
      int nparsed = sscanf(buf,"%s\t%s\t%f\t%d\t%c\t%c\n",chr,snpname,&dist,&pos,&al1,&al2);
      //      fprintf(stderr,"nparsed:%d al1:%c al2:%c dist:%f pos:%d chr:%s\n",nparsed,al1,al2,dist,pos,chr);

      char refchar = getchar(chr,pos-adjust,ref);
      //      fprintf(stderr,"nparsed:%d al1:%c al2:%c dist:%f pos:%d chr:%s refchar:%c\n",nparsed,al1,al2,dist,pos,chr,refchar);
      refchar = toupper(refchar);
      if(refchar!=al1 &refchar!=al2)
	refchar='N';
      if(refchar=='N')
	refchar='0';
      if(full){
	buf[strlen(buf)-1]='\0';
	fprintf(stdout,"%s\t%c %c\n",buf,refchar,refchar);
      }else
	fprintf(stdout,"%c %c\n",refchar,refchar);
      
      continue;
    }
    
  here:
    if(full)
      fprintf(stdout,"%s\t%d\t",chr,pos);
    char refchar = getchar(chr,pos-adjust,ref);
    if(toUpper==0)
      fprintf(stdout,"%c\n",refchar);
    else
      fprintf(stdout,"%c\n",toupper(refchar));
    if(ntok!=2){
      pos++;
      if(pos>stop)
	break;
      goto here;

    }
  }
  
  delete [] buf;
  destroy(ref);
  

  return 0;
}
