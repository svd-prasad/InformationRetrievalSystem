#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <math.h>                                                       //FIND QUERY TERMS AND COPY IT IN i
#include "postfix.c"
#include "preprocess.c"

int D;
char q1[20][50];
char qry_f[30][50];//={"facebook","OR","NOT","orkut"};

int queryterms()
{
 int i;
 i=0;
 while(strlen(qry_f[i])>0)
 i++;
 //printf("%d",i);
 return i;
} 
struct dnode
{
 int tf;
 char docid[100];
 int did;
 float tfidf;
 int df;
 struct dnode *dlink;
};
typedef struct dnode * dptr;
dptr postfile;

typedef struct q_t_struct
{
	 char term[50];
	 dptr pf;//Pointer to Posting List
}q_t_struct;

q_t_struct *qt_ar[20];


typedef struct stack_str//NOT using
{
	struct q_t_struct *qt;
	struct stack_str *next;	
}stack_str;

//stack_str *top=NULL;

q_t_struct *stk_arr[100];
int stk_top=-1;
void stk_push(q_t_struct *rec)
{

	if(stk_top==-1)
	{
		stk_top++;
		stk_arr[stk_top] = rec;
	}
	else
	{
		stk_top++;
		stk_arr[stk_top] = rec;
	}
	
}
q_t_struct * stk_pop()
{
	q_t_struct *temp;
	temp = stk_arr[stk_top];
	stk_arr[stk_top]=NULL;
	stk_top--;
	return temp; 
}


void globalcount()
{
 D=0;
 FILE *f;
 char str[100];
 f=fopen("indexedfiles","r");
               while(fscanf(f,"%s",str)!=EOF)
               {
                   D++;
               }
}

void eval_pfx()
{

	char pf_exp[100];
	FILE *pf_file;
	int len,i;//len length of postfix
	char ch;
	float idf1,idf2;
	pf_file = fopen("/home/kotikalapudi/Desktop/patriciaedit/postfix.txt","r");

	fscanf(pf_file,"%s",pf_exp);
	fclose(pf_file);

	printf("PostFix Exp: %s",pf_exp);
	len = strlen(pf_exp);
	pf_exp[len]='\0';

	i=0;
	while(i<len)
	{
		ch = pf_exp[i];
		if(ch <= '9' && ch >= '0')
		{       
			//push to stack
			stk_push(qt_ar[ch-48]);
			//printf("%s",qt_ar[ch-48]->pf->docid);
			//printf("--------------------------\n");
		}
		
		else if (ch=='*' || ch=='+')		
		{
		
			q_t_struct *e2,*e1;
			e2 = stk_pop();
			e1 = stk_pop();
			dptr n_root=NULL;
	        	dptr pf1,pf2,prev;
	        	pf1 = e1->pf;
			pf2 = e2->pf;
                        if(ch == '*')
			{
				
				if( /*(s1->found && s2->found) ||*/  (e1->pf && e2->pf) )//(!strcmp(e1->term,"merged") && s2->found) )
				{
					//Create New PL in MM				
					//push its addr to stack
					
					
					
					while(pf1 && pf2)
					{
						if(pf1->did == pf2->did)
						{        
							dptr re_b = (dptr)malloc(sizeof(struct dnode));
							re_b->did = pf1->did; 
							strcpy(re_b->docid,pf1->docid);
							re_b->tfidf = pf1->tfidf+pf2->tfidf;  //Total tf-idf	
							re_b->dlink = NULL;
							if(!n_root)
								n_root = re_b;
							else
								prev->dlink = re_b;
							prev = re_b;

							pf1 = pf1->dlink;
							pf2 = pf2->dlink;
						}
						else if(pf1->did < pf2->did)
							pf1 = pf1->dlink;
						else
							pf2 = pf2->dlink;
					}
					
					//Create record & insert into stack with root pointer
					q_t_struct *rec = (q_t_struct *)malloc(sizeof(q_t_struct));
					strcpy(rec->term,"merged");
					rec->pf = n_root;
					stk_push(rec);
					
				}
				else//New = NULL
				{
					q_t_struct *rec = (q_t_struct *)malloc(sizeof(q_t_struct));
					strcpy(rec->term,"merged");
					rec->pf = n_root;
					stk_push(rec);
				}
			}
			else //ch =='+'if()
			{
				
				while(pf1 && pf2) //Merrge Both
				{ 
						if(pf1->did == pf2->did)
						{
							dptr re_b = (dptr)malloc(sizeof(struct dnode));
							re_b->did = pf1->did;
							strcpy(re_b->docid,pf1->docid); 
							
							re_b->tfidf = pf1->tfidf+pf2->tfidf;  //Total tf-idf	
							re_b->dlink = NULL;
							if(!n_root)
								n_root = re_b;
							else
								prev->dlink = re_b;
							prev = re_b;

							pf1 = pf1->dlink;
							pf2 = pf2->dlink;
						}
						else if(pf1->did < pf2->did)
						{
							dptr re_b = (dptr)malloc(sizeof(struct dnode));
							re_b->did = pf1->did; 
							strcpy(re_b->docid,pf1->docid); 

							
							re_b->tfidf = pf1->tfidf;  //Total tf-idf	
							re_b->dlink = NULL;
							if(!n_root)
								n_root = re_b;
							else
								prev->dlink = re_b;
							prev = re_b;

							
							pf1 = pf1->dlink;
						}
						else
						{
							dptr re_b = (dptr)malloc(sizeof(struct dnode));
							re_b->did = pf2->did; 
							strcpy(re_b->docid,pf1->docid); 

							re_b->tfidf = pf2->tfidf;  //Total tf-idf	
							re_b->dlink = NULL;
							if(!n_root)
								n_root = re_b;
							else
								prev->dlink = re_b;
							prev = re_b;	
							pf2 = pf2->dlink;
						}
					
				}//while
				if(pf1) //Append pf1
				{
					while(pf1)
					{
						dptr re_b = (dptr)malloc(sizeof(struct dnode));
						re_b->did = pf1->did;
						strcpy(re_b->docid,pf1->docid); 
							
							re_b->tfidf = pf1->tfidf;  //Total tf-idf	
							re_b->dlink = NULL;
							if(!n_root)
								n_root = re_b;
							else
								prev->dlink = re_b;
							prev = re_b;

							
							pf1 = pf1->dlink;
					}
					
				}
				else if(pf2)//Append pf2
				{
					while(pf2)
					{
					           dptr re_b = (dptr)malloc(sizeof(struct dnode));
						   re_b->did = pf2->did;
						   strcpy(re_b->docid,pf2->docid); 
							
							re_b->tfidf = pf2->tfidf;  //Total tf-idf	
							re_b->dlink = NULL;
							if(!n_root)
								n_root = re_b;
							else
								prev->dlink = re_b;
							prev = re_b;

							
							pf2 = pf2->dlink;
							
					}

				}
				else//Both Not found
				{
				}

					q_t_struct *rec = (q_t_struct *)malloc(sizeof(q_t_struct));
					strcpy(rec->term,"merged");
					rec->pf = n_root;
					stk_push(rec);
				
			}
			
			
		}
		else// ch =='!'
		{
			dptr n_root=NULL,prev;
			q_t_struct *e1;
			e1 = stk_pop();

			//sres *s1 = build_search(e1->term);
			long file_count=1;
			dptr pf1;
			
                        FILE *fp;
                        char str[100];
                        fp=fopen("indexedfiles","r");
			pf1 = e1->pf;
			for(;file_count <= D;file_count++)
			{       fscanf(fp,"%s",str);
				if(pf1 && file_count == pf1->did )
				{
					//printf("word present in docID: %ld\n",file_count);
					pf1 = pf1->dlink;
					continue;
				}

				dptr re_b = (dptr)malloc(sizeof(struct dnode));
				re_b->did =file_count; 
				re_b->tfidf = 1.0f*0;
				strcpy(re_b->docid,str);

				re_b->dlink = NULL;
				if(!n_root)
					n_root = re_b;
				else
					prev->dlink = re_b;
				prev = re_b;

				
			}

			q_t_struct *rec = (q_t_struct *)malloc(sizeof(q_t_struct));
			strcpy(rec->term,"merged");
			rec->pf = n_root;
			stk_push(rec);
		

		}
		i++;
		

	}//while

	

	//Sort Linked List
	q_t_struct *res;
	res = stk_pop();
	
        dptr pf1,root,pf2;
	pf1 = pf2 = res->pf;
	root = res->pf;
	
        
	char t1[100]; float t2;
	while(pf1)
	{
		pf2 = pf1->dlink;
		while(pf2)
		{
			if(pf1->tfidf < pf2->tfidf)
			{
				//swap
				strcpy(t1 , pf1->docid); t2 = pf1->tfidf;

				strcpy(pf1->docid , pf2->docid);  pf1->tfidf = pf2->tfidf;

				strcpy(pf2->docid , t1); pf2->tfidf = t2;
				
			}
			pf2 = pf2->dlink;
		}
			
		pf1 = pf1->dlink;
	}


	// Retrieve from stack & display file names
        if(root)  
     l1:printf("\nDoc ID: %s\tTF_IDF: %f\n",root->docid,root->tfidf);
	root=root->dlink;
	if(root!=NULL) goto l1;
	else printf("\nLIST IS TRAVERSED\n");
          
	/*printf("\n\nResults found in\n");
	if(root=NULL) printf("NULL\n");
	while(root!=NULL)
	{printf("NOT NULL\n");
		printf("Doc ID: %s\tTF_IDF: %lf\n",root->docid,root->tfidf);
		root = root->dlink;
		
	}*/
	


}


dptr loadPF(char term[])
{
           
	char pf_name[200];
	FILE *pf_hdd,*p;
        int d;            //calculate the document frequencies
	dptr pf_temp,pf;
	dptr pf_hdd_root,prev;


	strcpy(pf_name,"/home/kotikalapudi/Desktop/patriciaedit/postingfiles/");
        strcat(pf_name,term);

        p= fopen(pf_name,"rb");
        d=0;
        pf = (dptr)malloc(sizeof(struct dnode));
        while(fread(pf,sizeof(struct dnode),1, p))
		{
		 d++;
                }
		fclose(p);

	//printf("%d----------------\n\n",d);
	pf_hdd = fopen(pf_name,"rb");
	pf_hdd_root=NULL;
	if(pf_hdd)
	{	
	       
		pf_temp = (dptr)malloc(sizeof(struct dnode));
		
		while(fread(pf_temp,sizeof(struct dnode),1, pf_hdd))
		{
		        pf_temp->df=d; 
		        pf_temp->tfidf=(  (pf_temp->tf)  *  logl((float)D / (pf_temp->df)) );        //USE -lm DURING COMPILATION
			pf_temp->dlink = NULL;
			if(!pf_hdd_root)
				pf_hdd_root = pf_temp;//starting address of the posting file
			else
				prev->dlink = pf_temp;
			prev = pf_temp;
		
			pf_temp = (dptr)malloc(sizeof(struct dnode));
			
			
		
		}
		fclose(pf_hdd);

		
		return pf_hdd_root;
	}
	else
	{
        	return NULL;
	}

}
void build_query()
{
	FILE *qf,*v;
	char word1[50];
	int p1,p2,j;
	int qt=0,i;

	char postfx_ip[50];
	char q_t_array[20][50];
	char path[200];

	
        i=queryterms();
	for(p1=0;p1<i;p1++) 
	printf("%s ",qry_f[p1]);

	
	
	strcpy(postfx_ip,"");
	j=0;qt=0;
	for(p1=0;p1<i;p1++)
	{
		if( !strcmp(qry_f[p1],"and") )                        //
			postfx_ip[j++]='*';
		else if( !strcmp(qry_f[p1],"or") )
			postfx_ip[j++]='+';
		else if( !strcmp(qry_f[p1],"not") )
			postfx_ip[j++]='!';
		else
		{
			postfx_ip[j++]=qt+48;// postfx_ip[j]='\0';

			strcpy(q_t_array[qt],qry_f[p1]);

			qt_ar[qt] = (q_t_struct *)malloc(sizeof(q_t_struct));//Create Memory for mapping word, values in postfix
			strcpy(qt_ar[qt]->term,qry_f[p1]);
			qt_ar[qt]->term[strlen(qt_ar[qt]->term)]='\0';

			printf("\n\nTERM FOUND:%s\n\n",qt_ar[qt]->term);
                        strcpy(path,"/home/kotikalapudi/Desktop/patriciaedit/postingfiles/");
                        strcat(path,qt_ar[qt]->term);
                        v=fopen(path,"rb");
                        strcpy(path,"");
                        if(v==NULL){ printf("POSTING FILE NOT FOUND\n"); exit(1);}
			
			qt_ar[qt]->pf = loadPF(qt_ar[qt]->term);

                     /*   while(qt_ar[qt]->pf)
                        {
                         printf("DId:%d\t TF: %d\t DOCNAME: %s\tDF:%d\tTFIDF:%f\n",qt_ar[qt]->pf->did,qt_ar[qt]->pf->tf,qt_ar[qt]->pf->docid,qt_ar[qt]->pf->df,qt_ar[qt]->pf->tfidf);
                         qt_ar[qt]->pf=qt_ar[qt]->pf->dlink;
                        }*/


			qt++;
		}
	}
	postfx_ip[j]='\0';

	printf("\nPostfix Input string: %s\n",postfx_ip);


	
	infix_to_postfix(postfx_ip);//Postfix expression will be created in ../bin/postfix.txt file
	
	//printf("\n\n%s",qt_ar[0]->pf->docid);
	eval_pfx();
	
	
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


main()
{
 globalcount();
 char str12[100];
 int i,j,k;
 k=0;
 FILE *f,*f1,*f2;
 f1=fopen("queryinput","r"); 
 porter(f1);
 
 filecopy("queryinput","stem");
 
            f2=fopen("steminput","w");               //EMPTY THE FILES DURING SUCCESSIVE ITERAITONS FOR CORRECTNESS
            fclose(f2);
            f2=fopen("stem","w");
            fclose(f2);
 f=fopen("queryinput","r");                        //contains the query terms
 while(fscanf(f,"%s\n",str12)!=EOF)
 {
  strcpy(qry_f[k++],str12);
 }
 build_query();
} 



 


