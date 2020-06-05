#include <stdio.h>
#include <stdlib.h>

#define MAX_EXPR_SIZE 100

typedef enum { lparen, rparen, plus, minus, times, divide, mod, eos, operand }
precedence;

typedef struct _list {
	precedence token;
	struct _list* link;
} list;
list* stack = NULL;

int top = -1;
char expr[MAX_EXPR_SIZE];

int isp[] = { 0, 19, 12, 12, 13, 13, 13, 0 }; // 교과서 3.6절 pp. 136 참고
int icp[] = { 20, 19, 12, 12, 13, 13, 13, 0 };

// 필요한 함수 선언
precedence getToken(char* symbol, int* n) {
	precedence token;

	switch (expr[*n]) {
	case '(' :
		token = lparen;
		break;
	case ')' :
		token = rparen;
		break;
	case '+' :
		token = plus;
		break;
	case '-' :
		token = minus;
		break;
	case '*' :
		token = times;
		break;
	case '/' :
		token = divide;
		break;
	case '%' :
		token = mod;
		break;
	case '\0' :
		token = eos;
		break;
	default:
		token = operand;
	}

	*symbol = *(expr + *n);
	++(*n);
	return token;
}

precedence pop() {
	list* toDel;
	precedence ret;

	if (stack == NULL) {
		fprintf(stderr, "error");
		exit(1);
	}

	toDel = stack;
	stack = stack->link;
	ret = toDel->token;
	free(toDel);
	return ret;
}

void printToken(precedence token) {
	switch (token) {
	case lparen:
		printf("( ");
		return;
	case rparen:
		printf(") ");
		return;
	case plus:
		printf("+ ");
		return;
	case minus:
		printf("- ");
		return;
	case times:
		printf("* ");
		return;
	case divide:
		printf("/ ");
		return;
	case mod:
		printf("% ");
		return;
	}
}

void push(precedence token) {
	list* new = (list*)calloc(sizeof(list), 1);
	new->token = token;
	new->link = stack;
	stack = new;
}

void postfix() {
	int n = 0;
	char symbol;
	precedence token;

	stack = (list*)calloc(sizeof(list), 1);
	stack->token = eos;
	stack->link = NULL;

	for (token = getToken(&symbol, &n); token != eos; token = getToken(&symbol, &n)) {
		if (token == operand)
			printf("%c ", symbol);

		else if (token == rparen) {
			while (stack->token != lparen)
				printToken(pop());
			pop();
		}

		else {
			while (isp[stack->token] >= icp[token])
				printToken(pop());
			push(token);
		}
	}

	while ((token = pop()) != eos)
		printToken(token);
}

void rExpr(FILE* fp, char* expr) {
	int i = 0;

	while (1) {
		if (fscanf(fp, "%c", expr + i) == -1)
			return;

		if (*(expr + i) == ' ')
			continue;

		i++;
	}
}

void erase() {
	list* toDel;
	while (stack) {
		toDel = stack;
		stack = stack->link;
		free(toDel);
	}
}

void main()
{
	FILE* fp = fopen("expr.txt", "r");
	rExpr(fp, expr);
	postfix();
	erase();
	fclose(fp);
}