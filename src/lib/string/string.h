/*
   evolbug 2017, MIT license
   string library
*/



#ifndef EVOLBUG_STRING
#define EVOLBUG_STRING



int chr_isalpha (char);
int chr_isdigit (char);
int chr_isalnum (char);
int chr_iswhite (char);

unsigned str_length (char*);
char*    str_new    (char*);

char* str_lower (char*);
char* str_upper (char*);

int str_equal    (char*, char*);
int str_comparen (char*, char*, int len); // 0 on equal
int str_compare  (char*, char*);          // 0 on equal

long  str_find      (char* in, char* what); //  -1  on fail
char* str_pfind     (char* in, char* what); // NULL on fail
long  str_findc     (char* in, char* what); //  -1  on fail
char* str_pfindc    (char* in, char* what); // NULL on fail
char* str_plast     (char* in, char* what); // NULL on fail
long  str_last      (char* in, char* what); //  -1  on fail
unsigned str_occurs (char* in, char* what);

char* str_from_fmt   (char* fmt, ...);
char* str_from_i     (long);
char* str_from_u     (unsigned long);
char* str_from_f     (double);
long  str_to_i       (char*);
unsigned long str_to_u (char*);
double str_to_f      (char*);

char* str_concat (char*, char*);
char* str_extend (char*, char*);

char* str_triml  (char*);
char* str_trimr  (char*);
char* str_trim   (char*);
char* str_slice  (char*, long from, long to);
char* str_slicep (char* from, char* to);
char* str_slicel (char* from, long length);
char* str_splitl (char** last, char* delim);
char** str_split (char*, char* delim);

char* str_format (char*, ...);



#endif
