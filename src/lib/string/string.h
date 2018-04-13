/*
   evolbug 2017, MIT license
   string library
*/



#ifndef EVOLBUG_STRING
#define EVOLBUG_STRING



int chr_isalpha (const char);
int chr_isdigit (const char);
int chr_isalnum (const char);
int chr_iswhite (const char);

int str_isupper (const char*);
int str_islower (const char*);

unsigned str_length (const char*);
char*    str_new    (const char*);

char* str_lower (const char*);
char* str_upper (const char*);

int str_equal    (const char*, const char*);
int str_comparen (const char*, const char*, int len); // 0 on equal
int str_compare  (const char*, const char*);          // 0 on equal

long  str_find      (const char* in, const char* what); //  -1  on fail
char* str_pfind     (const char* in, const char* what); // NULL on fail
long  str_findc     (const char* in, const char* what); //  -1  on fail
char* str_pfindc    (const char* in, const char* what); // NULL on fail
char* str_plast     (const char* in, const char* what); // NULL on fail
long  str_last      (const char* in, const char* what); //  -1  on fail
unsigned str_occurs (const char* in, const char* what);

char* str_from_fmt   (const char* fmt, ...);
char* str_from_i     (long);
char* str_from_u     (unsigned long);
char* str_from_f     (double);
long  str_to_i       (const char*);
unsigned long str_to_u (const char*);
double str_to_f      (const char*);

char* str_concat (const char*, const char*);
char* str_extend (char*, const char*);
char* str_join   (const char**);

char* str_triml  (const char*);
char* str_trimr  (const char*);
char* str_trim   (const char*);
char* str_slice  (const char*, long from, long to);
char* str_slicep (const char* from, const char* to);
char* str_slicel (const char* from, long length);
char* str_splitl (const char** last, const char* delim);
char** str_split (const char*, const char* delim);

char* str_format (const char*, ...);



#endif
