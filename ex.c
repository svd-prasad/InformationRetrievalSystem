#include<stdio.h>
int bitindex2(int a[],int b[])
{
    int i=4,j=0;       // PLEASE CHANGE THE SIZE OF ARRAY HERE (ONE LESS THAN THE ARRAY SIZE)
    //while(a[i]!='\0')                    i++;
    
    
    while(a[i]==b[i])
    {
                     i--;
                     }
     return i;                
                     }    
int bitindex1(int a[])
{
    int i=4;
    //while(a[i]!='\0')                    i++;
    //i--;
    while(a[i]!=1)                       i--;
    return i;
                  } 
               
main()
{
int a[]={1,1,0,0,0},b[]={1,0,0,1,0};
int c;
c=bitindex1(a);
printf("%d",c);
}
