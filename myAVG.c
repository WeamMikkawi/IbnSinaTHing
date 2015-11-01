#ifdef STANDARD
/* STANDARD is defined, don't use any mysql functions */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#ifdef __WIN__
typedef unsigned __int64 ulonglong;/* Microsofts 64 bit types */
typedef __int64 longlong;
#else
typedef unsigned long long ulonglong;
typedef long long longlong;
#endif /*__WIN__*/
#else
#include <my_global.h>
#include <my_sys.h>
#if defined(MYSQL_SERVER)
#include <m_string.h>/* To get strmov() */
#else
/* when compiled as standalone */
#include <string.h>
#define strmov(a,b) stpcpy(a,b)
#define bzero(a,b) memset(a,0,b)
#define memcpy_fixed(a,b,c) memcpy(a,b,c)
#endif
#endif
#include <mysql.h>
#include <ctype.h>
   
#ifdef HAVE_DLOPEN
   
#if !defined(HAVE_GETHOSTBYADDR_R) || !defined(HAVE_SOLARIS_STYLE_GETHOST)
static pthread_mutex_t LOCK_hostname;
#endif

my_bool MySum_init(UDF_INIT *initid, UDF_ARGS *args, char *message);                                                           
void MySum_deinit(UDF_INIT *initid);
double MySum(UDF_INIT *initid, UDF_ARGS *args, char *is_null, char *error);
void MySum_clear(UDF_INIT *initid, char *is_null, char *error);
void MySum_add(UDF_INIT *initid, UDF_ARGS *args, char *is_null, char *error);

typedef struct unit{
    double sum;
    int    count;
}Unit;

my_bool myAVG_init(UDF_INIT *initid, UDF_ARGS *args, char *message){
   Unit* i = (Unit*)malloc(sizeof(Unit)); 
   i->sum = 0;
   i->count = 0;                 
        
   initid->ptr = (char*)i;
        
   // check the arguments format
   if (args->arg_count != 1)
   {
      strcpy(message,"myAVG() requires one argument");
      return 1;
   }

   if (args->arg_type[0] != REAL_RESULT)
   {
      strcpy(message,"myAVG() requires a real");
      return 1;
   }       
   return 0;    
}

void MySum_deinit(UDF_INIT *initid){
   free( (Unit*)initid->ptr);
}

double myAVG(UDF_INIT *initid, UDF_ARGS *args, char *is_null, char *error){
    Unit *i = (Unit*) initid->ptr;
    return ((i->sum) / (i->count));
}

void myAVG_clear(UDF_INIT *initid, char *is_null, char *error){
   Unit *i = (Unit*) initid->ptr;
   i->sum = 0;
   i->count = 0;
}

void myAVG_add(UDF_INIT *initid, UDF_ARGS *args, char *is_null, char *error){
    Unit *i = (Unit*) initid->ptr;
    i->sum +=  *((double*)args->args[0]);
    ++(i->count);
}

#endif /* HAVE_DLOPEN */
   
