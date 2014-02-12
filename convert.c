#include<stdio.h>
#include "mddriver.c"

void convert1(char *word)
{
int i=0;
FILE *ptr;
ptr=fopen("hashvalues.txt","a+");
for(i=0;i<32;i++)
{
	switch(word[i])
	{
	case '0':fprintf(ptr,"0000");break;
	case '1':fprintf(ptr,"0001");break;
	case '2':fprintf(ptr,"0010");break;
	case '3':fprintf(ptr,"0011");break;
	case '4':fprintf(ptr,"0100");break;
	case '5':fprintf(ptr,"0101");break;
	case '6':fprintf(ptr,"0110");break;
	case '7':fprintf(ptr,"0111");break;
	case '8':fprintf(ptr,"1000");break;
	case '9':fprintf(ptr,"1001");break;
	case 'a':fprintf(ptr,"1010");break;
	case 'b':fprintf(ptr,"1011");break;
	case 'c':fprintf(ptr,"1100");break;
	case 'd':fprintf(ptr,"1101");break;
	case 'e':fprintf(ptr,"1110");break;
	case 'f':fprintf(ptr,"1111");break;
	}
}fprintf(ptr,"\n");
fclose(ptr);
}

      
void convert(char argv[])//main()
{
      //int i,j,k,v,e,a[10],b[10],x,d[10],h;
      char key1,key2,value[140],hexval[135];
         
FILE *fpp,*fpp1;
fpp=fopen(argv,"r");
while( (fscanf(fpp,"%s",value)) !=EOF)
MDString(value);
fclose(fpp);
      
fpp1=fopen("hfile.txt","r");
while((fscanf(fpp1,"%s",hexval))!=EOF)
convert1(hexval);

fclose(fpp1);

   
}

