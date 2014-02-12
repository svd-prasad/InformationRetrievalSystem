
/*Program to convert an infix expression to postfix.*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INVALID_CHAR 100

struct postfx_node
{
char value;
struct postfx_node* top;
};

typedef struct postfx_node Stack;

int err_code = 0; 

char find_top_element(Stack **ptr_base)
{
	Stack *temp = *ptr_base; 
	if (temp == NULL)
	{
		printf("\nError: Stack empty. Haulting execution.");
		getchar();
		exit(0);
	}
	return temp->value;
}

void push (Stack **ptr_base, int value)
{
	Stack *new_postfx_node; //Points to a new postfx_node of the structure.
	new_postfx_node = (Stack*)malloc(sizeof(Stack));
	if (new_postfx_node == NULL)
	{
		printf("\nFatal: Memory Allocation Error Halting Execution.");
		getchar();
		exit(-1);
	}

	new_postfx_node->value = value;

	if (*ptr_base == NULL)
		new_postfx_node->top = NULL;
	else
		new_postfx_node->top = *ptr_base;
	*ptr_base = new_postfx_node;
}

void display(Stack *ptr_base)
{
	Stack *temp = ptr_base; //Points to the top of the stack.
	if (temp->top == NULL)
	printf(" %d ", temp->value);
	
	else
	{
		while (temp->top != NULL)
		{
			printf(" %d ", temp->value);
			temp = temp->top;
		}
	}
}

//Function to pop an element from stack.
int pop(Stack **ptr_base)
{
	Stack *temp = *ptr_base; 
	int value; 
	
	if (temp == NULL) {
	printf("\nError: Stack Empty!");
	return -1;

	} else if (temp->top == NULL) {
	value = temp->value;
	temp = NULL;

	}
	else
	{
	value = temp->value;
	*ptr_base = temp->top;
	}
	
	free(temp);
	return value;
}

int is_digit (char ch)
{
	 
	 
	if(ch <= '9' && ch >= '0') 
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int get_priority(char ch)
{
	int priority = 0;
	switch(ch)
	{
		case '#':
			priority = 0;
			break;
		case '+':
			priority = 2;
			break;
		case '*':
			priority = 3;
			break;
		case '!':
			priority = 4;
			break;
		default:
			printf("\nInvalid operator in the expression\n");
			break;
	}
	return priority;
}

//Function to test whether a character is operator.
int is_operator(char ch)
{
	if (ch == '+' ||  ch == '!' || ch == '*')
	return 1;
	else
	return 0;
}

char pfop[50];//length of postfix 
int len=0;

//(char *) infix_to_postfix(char *infix)
void infix_to_postfix(char *infix)
{
	int i=0; 
	FILE *out;
	Stack *stack=NULL;
	char stack_token; 
	char ch; 
	int input_token_priority; 
	int stack_token_priority; 
	out = fopen("postfix.txt","w+");
	push(&stack,'#');

	while (*infix != '\0') 
	{
		if (is_operator(*infix))
		{
			input_token_priority = get_priority(*infix);
			stack_token = find_top_element(&stack);
			stack_token_priority = get_priority(stack_token);
			if (input_token_priority > stack_token_priority) 
			push(&stack, *infix);
		
			else 
			{
				while(get_priority(stack ->value )>= input_token_priority) 
				{
					ch = pop(&stack);
					fprintf(out,"%c",ch);
					pfop[len++]=ch;
					++i;
				}
			push(&stack, *infix);
			}
		}
		else if (is_digit(*infix)) 
		{
			fprintf(out,"%c",*infix); pfop[len++]=ch;
			++i;
		} 
		else 
		{
			err_code = INVALID_CHAR;
			break;
		}
		++infix;
	}
	 
	 while(stack ->value != '#') 
	{
		ch = pop(&stack);
		fprintf(out,"%c",ch); pfop[len++]=ch;
		++i;
	}
	 fclose(out);
	
}


