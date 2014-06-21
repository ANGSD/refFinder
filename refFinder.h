struct __perFasta_t;
typedef struct __perFasta_t perFasta;

perFasta *init(const char *fname);
void destroy(perFasta *f);
char getchar(char *chr,int pos,perFasta *ref);
