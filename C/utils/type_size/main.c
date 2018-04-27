#include <stdio.h>

void print_arch()  
{  
    int len=sizeof(int)*8;  
    printf("sizeof(int)=%d\n",len);  
    len=sizeof(int *)*8;  
    printf("sizeof(int*)=%d\n",len);  
  
#ifdef _MSC_VER  
    printf("_MSC_VER is defined\n");  
#endif  
  
#ifdef __GNUC__  
    printf("__GNUC__ is defined\n");  
#endif  
  
#ifdef __INTEL__   
    printf("__INTEL__  is defined\n");  
#endif  
  
#ifdef __i386__   
    printf("__i386__  is defined\n");  
#endif  
  
#ifdef __x86_64__   
    printf("__x86_64__  is defined\n");  
#endif  

#ifdef __arm__ 
    printf("__arm__  is defined\n");  
#endif
  
#ifdef _WIN32   
    printf("_WIN32 is defined\n");  
#endif  
  
#ifdef _WIN64   
    printf("_WIN64 is defined\n");  
#endif  
  
  
#ifdef __linux__   
    printf("__linux__ is defined\n");  
#endif  
  
#ifdef __LP64__   
    printf("__LP64__ is defined\n");  
#endif  
  
  
#ifdef __amd64   
    printf("__amd64 is defined\n");  
#endif  
} 

int main()
{
	print_arch();
	printf("unsigned long long size = %zd\n", sizeof(unsigned long long));
	printf("long long size = %zd\n", sizeof(long long));

	printf( "    short int: %zd\n" , sizeof(short int) ) ;
	printf( "          int: %zd\n" , sizeof(int) ) ;
	printf( "     long int: %zd\n", sizeof(long int) ) ;
	printf( "long long int: %zd\n", sizeof(long long int) ) ;
	printf( "       size_t: %zd\n", sizeof(size_t) ) ;
	printf( "        void*: %zd\n\n", sizeof(void *) ) ;

	return 0;
}

