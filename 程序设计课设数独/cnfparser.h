#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <malloc.h>
#define TRUE 1
#define FALSE 0

typedef struct clausenode{
	int literal;
	int score;
	struct clausenode* next;
}clausenode;

typedef struct clauselist{
	int listnum;//记录子句中的文字数 
	clausenode* nodehead;
	struct clauselist* next; 
}clauselist;

int varnum,cnum;//变元数量和子句数量 
int *assignment;
int op = 1, op1 = 1, op2 = 1, op3=1,flag_cnf, difficulty;
int d;
clauselist *head=(clauselist*)malloc(sizeof(clauselist));
clock_t start, finish;
double time1, time2, rate;
bool res;
char filename[500],wfilename[500];

int readcnf(char *filename, clauselist *&head){
	FILE* fp=fopen(filename, "r");
	if(fp){
		clauselist *p = head;
		p->nodehead = (clausenode*)malloc(sizeof(clausenode));
		clausenode *pn = p->nodehead;
		p->listnum = 0;
		
		char c;
		while((c = getc(fp)) == 'c'){
			while ((c = getc(fp)) != '\n')  //行首为c时读完此行 
	        	continue;
		}//跳出循环时行首应为p
		if(c != 'p') return FALSE;
		char ch[5];
		while(fscanf(fp, "%s", ch) != EOF){
			if(strcmp(ch, "cnf") == 0){
				fscanf(fp, "%d", &varnum);  // 变元数 
				fscanf(fp, "%d", &cnum);    // 子句数 
				break; 
			}
			else return FALSE;
		}
		int i;
		for(i = 0; i < cnum; i++){
			int dd;
			fscanf(fp, "%d", &dd);
			if(dd > varnum||dd < -1*varnum) return FALSE;
			while(dd){//读到0结束 
				p->listnum++; 
				pn->literal = dd;
				pn->score=0;
				pn->next = (clausenode*)malloc(sizeof(clausenode));
				fscanf(fp, "%d", &dd);
				if(dd > varnum||dd < -varnum) return FALSE;
				if(dd == 0) pn->next = NULL;
				pn = pn->next;
			}
			if(i == cnum - 1){
				p->next = NULL;
				break;
			}
			p->next = (clauselist*)malloc(sizeof(clauselist));
			p->next->listnum = 0;
			p->next->nodehead = (clausenode*)malloc(sizeof(clausenode));
			p = p->next;
			pn = p->nodehead;
		}
		
		fclose(fp);
		return TRUE; 
	}
	else return FALSE;
}
int printcnf(int s, char *wfilename, int *assignment, double t1, double t2)
{
    FILE* fp;
    if (!(fp = fopen(wfilename, "w+")))//创建res文件
    {
        printf("文件创建失败!\n");
        return FALSE;
    }
    fprintf(fp, "s %d\nv ", s);
    if (s)//如果可满足
        for (int i = 1; i <= varnum; i++)//写入结果
        {
            if (assignment[i])
                fprintf(fp, "%d ", i);
            else
                fprintf(fp, "%d ", -i);
        }
    fprintf(fp, "\nt %f ", t1);
    fclose(fp);
    return TRUE;
}


