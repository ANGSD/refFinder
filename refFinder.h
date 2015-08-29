struct __perFasta_t;
typedef struct __perFasta_t perFasta;

extern perFasta *init(const char *fname);
extern void destroy(perFasta *f);
extern char getchar(char *chr,int pos,perFasta *ref);
