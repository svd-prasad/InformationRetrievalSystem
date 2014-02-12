#include <dirent.h> 
#include <sys/types.h> 
#include <sys/param.h> 
#include <sys/stat.h> 
#include <unistd.h> 
#include <stdio.h> 
#include <string.h> 

//COPY THE PROGRAM AND RUN IN A DIRECTORY FOR GETTING THE LIST OF FILES
//DELETE THE PROGRAM AND A.OUT EXE FILE FROM THE OUTPUT


//FILE *ftemp;

void exploredir( char *result ) {
  DIR           *d;
  struct dirent *dir;
  
   d = opendir(".");
  
  if( d == NULL )
   {
    return ;
   }
  while( ( dir = readdir( d ) ) ) 
  {
	    if( strcmp( dir->d_name, "." ) == 0 || strcmp( dir->d_name, ".." ) == 0 ) 
	    continue;
	    
	    if( dir->d_type == DT_DIR ) 
	    {
	      chdir( dir->d_name );
	      exploredir( result );
	      chdir( ".." );
	    } 
	    else 
	    {
	        int  len;
		getcwd( result, MAXPATHLEN );
		len = strlen( result );
		snprintf( result + len, MAXPATHLEN - len, "/%s", dir->d_name );
		//fprintf( ftemp,"%s\n", result );
		printf( "%s\n", result );
	    }
	    
	    
  }
  closedir( d );
  
}

int main(int argc, char* argv[] ) {
  char buf[MAXPATHLEN] = { Sample };
  //ftemp=fopen("docs.txt","w+");
    exploredir(  buf );
  return 0;
}
