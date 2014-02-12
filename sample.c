#include<stdio.h>
#include<string.h>
main(int argc,char *argv[])
{
    FILE *f3;
    char file[30];
    if(argc<2) printf("TOO FEW ARGUMENTS");
   else 
   {
      strcpy(file,argv[2]);
      printf("%s\n",file);
      /*
      f3=fopen("input.txt","r");
      while(fscanf(f3,"%s",file)!=EOF)
      {
          
          printf("%s\n",file);
      }*/
      
      
   }
   fclose(f3);
}
