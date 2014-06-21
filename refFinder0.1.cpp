/*
  thorfinn thorfinn@binf.ku.dk 18dec 2012
  assumes zero index positions

  should remove mutexs, and make function thread safe. dragon daemon

  part of angsd using faidx
 
  Actually no need to mutex, except for the first chunk. Lets fix it at some point.
*/

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <map>

#include "faidx.h"

#define LENS 100000
struct cmp_str
{
   bool operator()(char const *a, char const *b)
   {
      return std::strcmp(a, b) < 0;
   }
};
typedef std::map<char *,int,cmp_str> aMap;


typedef struct{
  faidx_t *fai;//contains the faidx structure
  char **seqs;//contains the reference[chr][position]
  int *chrLen;//length of chromosome
  aMap asso;
}perFasta;


//this will initialize our data
perFasta *init(const char *fname){
  fprintf(stderr,"[%s.%s:%d] %s\n",__FILE__,__FUNCTION__,__LINE__,fname);
  perFasta *r= new perFasta;
  r->fai = NULL;
  r->seqs = NULL;
  
  if(NULL==(r->fai = fai_load(fname))){
    fprintf(stderr,"[%s:%s] error reading fai file:%s\n",__FILE__,__FUNCTION__,fname);
    exit(0);
  }
  int nref = getnref(r->fai);
  r->chrLen = new int[nref];
  r->seqs = new char*[nref];
  for(int i=0;i<nref;i++)
    r->seqs[i] = NULL;
  return r;
}


//this will destroy a perfasta structure
void destroy(perFasta *f){
  fai_destroy(f->fai);
  free(f->seqs);
  f=NULL;
}

int loadChr(perFasta *f, char*chrName){
  fprintf(stderr,"[%s] \t->loading chr:%s from faidx=%p cursize=%zu\n",__FUNCTION__,chrName,f,f->asso.size());
  fflush(stderr);
  f->asso.insert(std::pair<char*,int>(strdup(chrName),f->asso.size()));
  
  
  f->seqs[f->asso.size()-1] = faidx_fetch_seq(f->fai, chrName, 0, 0x7fffffff, &f->chrLen[f->asso.size()-1]);
  if(f->seqs[f->asso.size()-1]==NULL){
    fprintf(stderr,"[%s] Error loading fasta info from chr:\'%s\' \n",__FUNCTION__,chrName);  
    f->chrLen[f->asso.size()-1] = 0;
  }
  //  fprintf(stderr,"[%s] done\n",__FUNCTION__);
  return f->asso.size()-1;
}



int main(int argc, char **argv){
  fprintf(stderr,"pipe into: ./refFinder hg19.fa [inputIsZero full]\n");
  if(argc==1)
    return 0;
  char *refname = argv[1];
  argv+=2;

  int adjust = 1;
  int full =0;
  argc-=2;
  while(argc>0 &&*argv){
    //fprintf(stderr,"*arg:%s\n",*argv);
    if(!strcasecmp("inputIsZero",*argv)) adjust =0;
    else if(!strcasecmp("full",*argv)) full =1;
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
  int verbCounter =20;
  while(fgets(buf,LENS,stdin)){
    // fprintf(stderr,"buf: %s\n",buf);
    if(buf[0]=='\n')
      continue;
    char *chr = strtok(buf,"\t\n ");
    int pos = atoi(strtok(NULL,"\t\n "));
    // fprintf(stderr,"\t-> chr:%s\tpos:%d\n",chr,pos);
    aMap::const_iterator it = ref->asso.find(chr);
    int which = -1;
    if(it==ref->asso.end()){
      which = loadChr(ref,chr);
    }else
      which = it->second;
    char refchar = 'N';
    if(pos-adjust < ref->chrLen[which])
      refchar = ref->seqs[which][pos-adjust];
    else
      if(verbCounter-->0)
      fprintf(stderr,"Problem try to access after end of chr: \'%s\' with pos: %d reflength is only:%d char set to 'N'\n",chr,pos-adjust,ref->chrLen[which]);
    
    if(full)
      fprintf(stdout,"%s\t%d\t",chr,pos);
    fprintf(stdout,"%c\n",refchar);
    //    break;
  }
  
  

  return 0;
}
