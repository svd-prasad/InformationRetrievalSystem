#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "mddriver.c"
#include "preprocess.c"


#define size  100000                   //QUEUE SIZE
#define true 1
#define false 0
static int d;         //
struct dnode
{
  int tf;
  char docid[100];
  int did;            //
  float tfidf;
  int df;
  struct dnode *dlink;
};
typedef struct dnode * dptr;  

struct node
{
       char key[130];
       int bitindex,a[1128];
       struct node *lchild;
       struct node *rchild;
       int df;
       int visited; 
       dptr head;
       };
typedef struct node * lptr; 
lptr root;                                   //GLOBAL DECLARATION


struct queue
{
 int front,rear;
 int number;
 lptr array[size];                     //
};
typedef struct queue * qptr;

void initialize(qptr q)
{
 q->front=0;
 q->rear=-1;
 q->number=0;
}

int empty(qptr q)
{
 if(q->number==0)
  return true;
 return false;
}
int full(qptr q)
{
 if(q->number==size)
  return true;
 return false;
}
int add_element(qptr q,lptr l)      //
{ 
 if(full(q))
  return false;
 if(q->rear ==size-1)
  q->rear=-1;
  q->array[++q->rear]=l;
  q->number++;
  return true;
}
lptr remove_element(qptr q)           //
{
  lptr i;                           //
  if(empty(q))
   return -1;
  i=q->array[q->front++];
  if(q->front==size) 
   q->front=0;
  q->number--;
  return i;
}

void bfs1(lptr svd)                //creates posting files
{
 qptr q;
 
 lptr l1,l2,l3;//
 q=(qptr)malloc(sizeof(struct queue));
 initialize(q);
 
 add_element(q,svd);
 FILE *p;
 char path[200],path1[200],path2[200],path3[200];
 strcpy(path,"/home/kotikalapudi/Desktop/patriciaedit/postingfiles/");
 //strcat(path,svd->key);
 //p=fopen(path,"w");
 //fclose(p);
    while(svd->head!=NULL && svd->head->dlink!=svd->head)
    {
     
     svd->head=svd->head->dlink;
     strcpy(path1,path);
     strcat(path1,svd->key);
     p=fopen(path1,"ab");                //open in append binary mode(CHECK WHETHER TO BE CREATED IF NOT NULL)

     if(svd->head!=NULL) 
     {
      printf("KEY VALUE:%s \t TERM FREQUENCY:%d \t DOCUMENT NAME:%s \t DOCUMENT FREQUENCY: %d\n\n ",svd->key,svd->head->tf,svd->head->docid,svd->df);
      fwrite(svd->head,sizeof(struct dnode),1,p);
     } 
    
     
     fclose(p);   
    
    }
 //printf("%s\n",svd->key);                                 //WRITE IN FILE
 
 
 while(!empty(q))
 {
  
  l1=remove_element(q);
  l1->visited=1;
  
  if(l1->lchild!=NULL)
  {
   //printf("%s\n",l1->lchild->key);
   if(l1->lchild->visited!=1)
   {
    add_element(q,l1->lchild);
    //printf("%s\n",l1->lchild->key);
    while(l1->lchild->head!=NULL && l1->lchild->head->dlink!=l1->lchild->head)
    {
     l1->lchild->head=l1->lchild->head->dlink;
     strcpy(path2,path); 
     strcat(path2,l1->lchild->key);
     p=fopen(path2,"ab");
     
     if(l1->lchild->head!=NULL) 
     {
      printf("KEY VALUE:%s \t TERM FREQUENCY:%d \t DOCUMENT NAME:%s \t DOCUMENT FREQUENCY: %d\n\n ",l1->lchild->key,l1->lchild->head->tf,l1->lchild->head->docid,l1->lchild->df);
      fwrite(l1->lchild->head,sizeof(struct dnode),1,p);
     }
     fclose(p);
    
    }    
   } 
  }
  if(l1->rchild!=NULL)
  {
   if(l1->rchild->visited!=1)
   {
   add_element(q,l1->rchild);
    while(l1->rchild->head!=NULL && l1->rchild->head->dlink!=l1->rchild->head)
    {
     l1->rchild->head=l1->rchild->head->dlink;
     strcpy(path3,path);
     strcat(path3,l1->rchild->key);
     p=fopen(path3,"ab");
     if(l1->rchild->head!=NULL)
     { 

      printf("KEY VALUE:%s \t TERM FREQUENCY:%d \t DOCUMENT NAME:%s \t DOCUMENT FREQUENCY: %d\n\n ",l1->rchild->key,l1->rchild->head->tf,l1->rchild->head->docid,l1->rchild->df);
      fwrite(l1->rchild->head,sizeof(struct dnode),1,p);
     }
     fclose(p);
    }
   }
  } 
 }
 
 
}  
 
void bfs(lptr svd)                //creates dictionary in disk
{
 qptr q;
 FILE *dic;
 lptr l1,l2,l3;//
 q=(qptr)malloc(sizeof(struct queue));
 initialize(q);
 add_element(q,svd);
 dic=fopen("dictionary","wb+");
 //printf("%s\n",svd->key);                                 //WRITE IN FILE
 fwrite(svd,sizeof(struct node),1,dic); 
 
 while(!empty(q))
 {
  l1=remove_element(q);
  l1->visited=1;
  if(l1->lchild!=NULL)
  {
   if(l1->lchild->visited!=1)
   {
    add_element(q,l1->lchild);
    //printf("%s \n",l1->lchild->key);
    fwrite(l1->lchild,sizeof(struct node),1,dic); 
   }
   else
   {
    //printf("%s \n",l1->lchild->key);
    fwrite(l1->lchild,sizeof(struct node),1,dic); 
   } 
  }
  if(l1->rchild!=NULL)
  {
   if(l1->rchild->visited!=1)
   {
    add_element(q,l1->rchild);
    //printf("%s \n",l1->rchild->key);
    fwrite(l1->rchild,sizeof(struct node),1,dic); 
   }
   else
   {
    //printf("%s \n",l1->rchild->key); 
    fwrite(l1->rchild,sizeof(struct node),1,dic);     
   }
  } 
 }
 fclose(dic);
 dic=fopen("dictionary","rb");
 printf("--------Dictionary contents--------\n");
 while(fread(l1,sizeof(struct node),1,dic))
 printf("\n%s",l1->key);
 printf("\n--------------------------------\n");
 //reconstruct();
 fclose(dic); 
}  

void reset(lptr svd)                //make visited to 0
{
 qptr q;
 FILE *dic;
 lptr l1,l2,l3;//
 q=(qptr)malloc(sizeof(struct queue));
 initialize(q);
 add_element(q,svd);
 
 
 while(!empty(q))
 {
  l1=remove_element(q);
  l1->visited=0;
  if(l1->lchild!=NULL)
  {
   if(l1->lchild->visited!=0)
   {
    add_element(q,l1->lchild);
    //printf("%s \n",l1->lchild->key);
    //fwrite(l1->lchild,sizeof(struct node),1,dic); 
   }
   else
   {
    //printf("%s \n",l1->lchild->key);
    //fwrite(l1->lchild,sizeof(struct node),1,dic); 
   } 
  }
  if(l1->rchild!=NULL)
  {
   if(l1->rchild->visited!=0)
   {
    add_element(q,l1->rchild);
    //printf("%s \n",l1->rchild->key);
    //fwrite(l1->rchild,sizeof(struct node),1,dic); 
   }
   else
   {
    //printf("%s \n",l1->rchild->key); 
    //fwrite(l1->rchild,sizeof(struct node),1,dic);     
   }
  } 
 }
 
}  


void reconstruct()                               //modify return type
{
 qptr q;
 FILE *dic;

 
 lptr l1,l2,l3,ptr,ptr1;//


printf("--------Dictionary contents--------\n");
dic=fopen("dictionary","rb");

if(dic)
{
l1=(lptr)malloc(sizeof(struct node));
 while(fread(l1,sizeof(struct node),1,dic))
 printf("\n%s",l1->key);
 printf("\n--------------------------------\n");
 fclose(dic); 
 }

 q=(qptr)malloc(sizeof(struct queue));
 initialize(q);

 dic=fopen("dictionary","rb");
 if(!dic) {root=NULL;}
else
{

 root=(lptr)malloc(sizeof(struct node));
 
 

 fread(root,sizeof(struct node),1,dic);         //ROOT of newly reconstructed is l3
 
 root->head=(dptr)malloc(sizeof(struct dnode));
 root->head->dlink=root->head;
 //root->df=0;
 add_element(q,root);
 //if(root->lchild==NULL) printf("IAM NULL\n\n");
 //if(root->rchild==NULL) printf("IAM NULL\n\n");
 //printf("here");
           while(!empty(q))
           {
            
             //l1=(lptr)malloc(sizeof(struct node));
             l1=remove_element(q);
             
             printf("%s root \n",l1->key); 
             
                  if(l1->lchild==NULL) printf("IAM NULL\n\n");
                  if(l1->rchild==NULL) printf("IAM NULL\n\n");
                  if(l1->lchild)
                  {
                                          l2=(lptr)malloc(sizeof(struct node));
                                          
                                          
                                          fread(l2,sizeof(struct node),1,dic);
                                          //l2->df=0;
                                          l2->head=(dptr)malloc(sizeof(struct dnode));
                                          l2->head->dlink=l2->head;
                                          
                                          
                                          if(l1->bitindex==l2->bitindex)
                                          {
                                           l1->lchild=l1;
                                           printf("%s left child is:%s \n",l1->key,l1->lchild->key); 
                                          } 
                                          if(l1->bitindex>l2->bitindex)
                                          {
                                           add_element(q,l2);
                                           l1->lchild=l2;
                                           printf("%s left child is:%s \n",l1->key,l1->lchild->key);                                            
                                          }
                                          if(l1->bitindex<l2->bitindex)
                                          {
                                            ptr1=root;
                                            ptr=root;
                                            while(ptr->bitindex!=l2->bitindex)
                                            {	
                                            if(l2->a[ptr->bitindex]==1)
                                            	ptr=ptr->rchild;
                                            else
                                            	ptr=ptr->lchild;
                                            }//
                                            printf("%s left child is:%s \n",l1->key,ptr->key);  
                                            l1->lchild=ptr;
                                          } 
                  }
                  if(l1->rchild)
                  {
                                          l2=(lptr)malloc(sizeof(struct node));
                                          //l2->df=0;
                                          
                                          fread(l2,sizeof(struct node),1,dic);
                                          l2->head=(dptr)malloc(sizeof(struct dnode));
                                          l2->head->dlink=l2->head;
                                          //l2->df=0;
                                          
                                          if(l1->bitindex==l2->bitindex)
                                          {
                                           l1->rchild=l1;
                                           printf("%s right child is:%s \n",l1->key,l1->rchild->key); 
                                          } 
                                          if(l1->bitindex>l2->bitindex)
                                          {
                                           add_element(q,l2);
                                           l1->rchild=l2;
                                           printf("%s right child is:%s \n",l1->key,l1->rchild->key); 
                                          }
                                          if(l1->bitindex<l2->bitindex)
                                          {
                                            ptr1=root;
                                            ptr=root;
                                            while(ptr->bitindex!=l2->bitindex)
                                            {	
                                            if(l2->a[ptr->bitindex]==1)
                                            	ptr=ptr->rchild;
                                            else
                                            	ptr=ptr->lchild;
                                            }//
                                            l1->rchild=ptr;
                                            printf("%s right child is:%s \n",l1->key,l1->rchild->key); 
                                          } 
                  }

           
           }
 fclose(dic);           
 //bfs(root);
 //return root;
 }
 
 //fclose(dic);
 //return root;
 
}




void increment(dptr head,char docid[])
{
  while(head!=NULL)
  {
    head=head->dlink;
    if(head!=NULL) 
    {
    if(!strcmp(docid,head->docid))
    head->tf=(head->tf)+1;
    }
    
  }  

}  
void insert1(lptr l1,dptr head,char doc[],int x)
{
  dptr d1;
  int flag2=0;
  dptr head1;
  head1=head;
  d1=(dptr)malloc(sizeof(struct dnode));
  strcpy(d1->docid,doc);
  d1->did=x;
  d1->dlink=NULL;
  if(head->dlink==head)
  {
    
    d1->tf=1;                                               //first document
    l1->df=1;
    head->dlink=d1;
  }
 else
 {
                   while(head1!=NULL) 
                   {
                       head1=head1->dlink;
                       if(head1!=NULL)
                       {
                         if(!strcmp(head1->docid,doc))
                         flag2=1; 
                       }
                   }   
                   if(flag2)
                   {
                    increment(head,doc); 
                   } 
                   else
                   { 
                       l1->df++;                               //document freq incremented
                       d1->tf=1;
                       while(head->dlink!=NULL)
                       {    
                        head=head->dlink;
                       } 
                        head->dlink=d1;  
                    
                   }

  } 
}

int bitindex1(int a[])
{
    int i=127;                               //MODIFICATION
    while(a[i]!=1)                       i--;
    return i;
}
int bitindex2(int a[],int b[])
{
    int i=127,j=0;                           //MODIFICATION    
    while(a[i]==b[i])
    {
                     i--;
    }
     return i;                
}     
lptr closest(int a[],lptr root)
{
     int i,j,k;
     lptr ptr,ptr1;
     i=0;
     ptr1=root;
     ptr=root;
     do     
     {
         ptr1=ptr;
         if(a[ptr->bitindex]!=1) ptr=ptr->lchild;
         else ptr=ptr->rchild;//printf("LOOP");
         if(ptr==NULL) break;
     }while(ptr1->bitindex>ptr->bitindex);
     return ptr;
}   

void insert(char *file1,char* file,int x1)
{
      int i,j,k,v,e,b[1129],x,d[1129],h,a[1129];
      char key1[1100],key2[1100],value[140],hexval[135],val[1129],str[1100];
      lptr temp,tnode,root1,search,tem,z,ptr,ptr1,addr;
      
 
      convert(file);
      FILE *fp,*fp2;
      fp=fopen("hashvalues.txt","r");
      fp2=fopen(file,"r");
      while( (fscanf(fp,"%s",value)) !=EOF)
      {
	strcpy(val,value);
	for(i=0;i<128;i++)                        //MODIFICATION
	{
		a[i]=val[i]-48;
		
	}
	               /* for(i=0;i<128;i++) 
	                printf("%d",a[i]);
	                printf("\n");*/
	                fscanf(fp2,"%s",key1);
			puts(key1);
                        printf("\n");
                   //CREATION OF ROOT NODE
                    if(root==NULL)
                    {            printf("\n 1 ");
                                 root=(lptr)malloc(sizeof(struct node));

                                       root->df=0; 

                                       root->head=(dptr)malloc(sizeof(struct dnode));
                                       root->head->dlink=root->head;
                                       insert1(root,root->head,file1,x1);                           //posting file

                                       root->visited=0;  
                                 
                                 strcpy(root->key,key1);
                                 root->bitindex=bitindex1(a);
                                 root->rchild=root;
                                 root->lchild=NULL;
                                 for(i=0;i<128;i++)               //MODIFICATION  
                                 root->a[i]=a[i]; 
                                 //printf(" 1 ");
                           }
                     else
                     {
                             printf("prasas");
                      printf(" \nNONROOT 2 \n");
                        printf("prasas");
                                  temp=closest(a,root);
                                  printf("prasas");
                                  if(temp!=NULL)
                                  {
                                    for( k=0;k<128;k++)            //MODIFICATION
                                    b[k]=temp->a[k];
                                    v=bitindex2(a,b);
                                   } //temp!=NULL
                                   else
                                   {
                                    v=bitindex1(a);  
                                   }//temp==NULL
                                 if(v<0){
                                           if(temp!=NULL)
                                           {
                                            //temp->head=(dptr)malloc(sizeof(struct dnode));
                                            //temp->head->dlink=temp->head;
                                            insert1(temp,temp->head,file1,x1);                                   //posting file
                                           } 

                                           
                                            printf("\nTHE KEY IS ALREADY INSERTED\n");  
                                            continue;
                                          } 
                                 printf("\nThe bitindices for key %s is:-%d",key1,v);
                                 tnode=(lptr)malloc(sizeof(struct node));

                                              tnode->df=0; 
                                         
                                              tnode->head=(dptr)malloc(sizeof(struct dnode));
                                              tnode->head->dlink=tnode->head;
                                              insert1(tnode,tnode->head,file1,x1);                                 //posting file     

                                              tnode->visited=0;
                                 
                                 strcpy(tnode->key,key1);
                                 tnode->bitindex=v;
                                 for(i=0;i<128;i++)                //MODIFICATION
                                 tnode->a[i]=a[i]; 

                                 //Inserting the node
                                                   ptr=root;
                                                   ptr1=root;
                                                   while(ptr->bitindex>v)
                                                   {
                                                       ptr1=ptr;
                                                       if(tnode->a[ptr->bitindex]!=1) ptr=ptr->lchild;                  
                                                       else                    ptr=ptr->rchild;
                                                       if(ptr==NULL) break;
                                                       if(ptr->bitindex<=v || ptr1->bitindex<=ptr->bitindex) break;
                                                       }

                                                   if(v>root->bitindex)
                                                   {
                                                    //printf("\nCASE 3 \n");
                                                       if(tnode->a[v]!=1)
                                                       {
                                                        tnode->lchild=tnode;
                                                        tnode->rchild=root;
                                                        root=tnode;
                                                        printf("\nNEW NODE LEFT CHILD IS:%s\n",tnode->lchild->key);
                                                        printf("\nNEW NODE RIGHT CHILD IS:%s\n",tnode->rchild->key);  
                                                       }
                                                       else
                                                       {
                                                        tnode->lchild=root;
                                                        tnode->rchild=tnode;
                                                        root=tnode;
                                                        printf("\nNEW NODE LEFT CHILD IS:%s\n",tnode->lchild->key);
                                                        printf("\nNEW NODE RIGHT CHILD IS:%s\n",tnode->rchild->key);
                                                        }
                                                         //printf("\nCASE  3\n ");
                                                       
                                                   } 
                                                   else if(ptr==NULL)
                                                   {
                                                      ptr1->lchild=tnode;
                                                      tnode->lchild=NULL;
                                                      tnode->rchild=tnode;
                                                      printf("\nNEW NODE LEFT CHILD IS NULL\n");
                                                      printf("\nNEW NODE RIGHT CHILD IS:%s\n",tnode->rchild->key);
                                                   }
                                             
                                                   else
                                                   {
                                                     printf("\nLAST CASE \n");
                                                     if(tnode->a[ptr1->bitindex]!=1)      {
                                                                                              ptr1->lchild=tnode;
                                                                                              printf("NEW NODE IS LEFT CHILD OF :%s",ptr1->key);
                                                                                          }     
                                                                  else                    {   ptr1->rchild=tnode;
                                                                                              printf("NEW NODE IS RIGHT CHILD OF:%s",ptr1->key);
                                                                                          }  
                                                                  if(tnode->a[v]!=1)      {               
                                                                                              tnode->lchild=tnode;
                                                                                              tnode->rchild=ptr;
                                                                                              printf("\nNEW NODE LEFT CHILD IS:%s\n",tnode->lchild->key);
                                                                                              printf("\nNEW NODE RIGHT CHILD IS:%s\n",tnode->rchild->key);  
                                                                                
                                                                                          }
                                                                  else                    {
                                                                                              tnode->rchild=tnode;
                                                                                              tnode->lchild=ptr;
                                                                                              printf("\nNEW NODE LEFT CHILD IS:%s\n",tnode->lchild->key);
                                                                                              printf("\nNEW NODE RIGHT CHILD IS:%s\n",tnode->rchild->key);   
                                                                                          }
                                                      //printf("\nLAST CASE\n");                                    
                                                   }
                                                   
                                 
                           //printf("\nNONROOT2 ELSE EXIT\n");       
                           } 
                           
                } 
             
     fclose(fp2);  
     fclose(fp);   
  
      FILE *ptr11;
      ptr11=fopen("hashvalues.txt","w");
      fclose(ptr11); 
      FILE *ptr12;
      ptr12=fopen("hfile.txt","w");
      fclose(ptr12); 

}//insert
void search()
{
     char str[1000],val[500],hexval[1000]; 
     int d[1000],i;
     lptr z;
     fflush(stdin);
     printf("\nEnter the key to be searched:");
     gets(str);
     


                 	FILE *fp1,*fp;

			fp1 = fopen("hfile.txt","w+"); 
			fclose(fp1);

			MDString(str);

			fp=fopen("hfile.txt","r");
			fscanf(fp,"%s",hexval);         
			fclose(fp);

			fp1=fopen("hashvalues.txt","w+");
			fclose(fp1);

			convert1(hexval);

			fp1=fopen("hashvalues.txt","r");
			fscanf(fp1,"%s",val);
			fclose(fp1);

			for(i=0;i<128;i++)
			d[i]=val[i]-48;
				
               
               
               z=closest(d,root);
               if(!strcmp(z->key,str)) printf("\nSuccessful\n");
               else                     printf("\nNO\n");  
  }      
void filecopy(char ft1[],char fs1[])
{
    FILE *fs,*ft;
    char ch;
    fs = fopen(fs1,"r");
    ft = fopen(ft1,"w");
    while(1)
    {
    ch = getc(fs);
    if(ch==EOF)
    {
    break;
    }
    else
    putc(ch,ft);
    }
    fclose(fs);
    fclose(ft);
}
main(int argc,char *argv[])
{
    FILE *f3,*sptr,*p,*f2,*f9;
    char file[130];
    char path6[200],str;
    dptr svd;
    int i=2;
    lptr root1,root2;
    
    f9=fopen("docid","r");
    fscanf(f9,"%d",&d);
    fclose(f9);
    printf("%d",d);
    if(argc<3) printf("TOO FEW ARGUMENTS");
    else if(argv[1][0]=='-' && argv[1][1]=='i')
    {
      //insert(argv[1][1],argv[2]);
      //root=NULL;//reconstruct();                               //load the dictionary from the disk and reconstruct the tree

      //root=(lptr)malloc(sizeof(struct node));

      //root->df=0; 
      //root->head=(dptr)malloc(sizeof(struct dnode));
      //root->head->dlink=root->head;

      reconstruct(); 
      //root=NULL;
      
      //if(root->head->dlink)printf("NULL");
      //printf("\n%s\n",root->head->docid);
      //bfs1(root);
      f3=fopen(argv[2],"r"); //argv[2] contains file names to be indexed

      while(fscanf(f3,"%s",file)!=EOF)
      {

                                               f2=fopen("indexedfiles","a");
                                               fprintf(f2,"%s\n",file);
                                               fclose(f2);

      
       sptr=fopen(file,"r");
       porter(sptr);                                    //steminput file contains stop word removal and stem contains porter stemming output(NOTE:- TO BE CLEARED)
      
            d++; 
            //filecopy(file,"stem");
            insert(file,"stem",d);                        //file contains original file name and stem contains stem input
      
                 
            FILE *sptr2,*sptr3; 
            sptr2=fopen("steminput","w");               //EMPTY THE FILES DURING SUCCESSIVE ITERAITONS FOR CORRECTNESS
            fclose(sptr2);
            sptr3=fopen("stem","w");
            fclose(sptr3);
      } 
      root1=root;
      bfs(root);                                        //update the dictionary   BUG  FOUND IS ROOT MODIFIED and then write posting files
      reset(root);                                      //RESET THE PREVIOUS VISITED VALUES TO 0
      //reconstruct();
      bfs1(root);                                     //update the posting files after writing dictionary to hard disk

                                                f9=fopen("docid","w");
                                                fprintf(f9,"%d",d);
                                                fclose(f9);
    }
    else if(argv[1][0]=='-' && argv[1][1]=='s')
    {
      reconstruct(); 
      printf("%d",root->df);

      svd=(dptr)malloc(sizeof(struct dnode));
      strcpy(path6,"/home/kotikalapudi/Desktop/patriciaedit/postingfiles/");
      strcat(path6,argv[2]);
      puts(path6);
      p=fopen(path6,"rb");                //open in read binary mode
      //fread(svd,sizeof(struct dnode),1,p);
      
      
      while(fread(svd,sizeof(struct dnode),1,p))
      {
       
        if(svd!=NULL) 
        {
         printf(" \t TERM FREQUENCY:%d \t DOCUMENT NAME:%s \t \n\n ",svd->tf,svd->docid);
         
        } 
      }
      fclose(p);  
    }   
}

      
                                          
                                                         
                                  
                                                                                      
                                                               
                                                             
                          
