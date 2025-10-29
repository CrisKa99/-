#include "cnfparser.h"

void destroyClause(clauselist*& cnf)
{
    clauselist *ppc, *pc2; 
    clausenode *pn1, *pn2; 
    ppc = cnf;
    while (ppc) 
    {
        pn1 = ppc->nodehead;   //ѭ��ɾ���Ӿ�
        while (pn1){
        	pn2 = pn1->next;
			free(pn1);
			pn1 = pn2;
		}
        pc2 = ppc->next;
		free(ppc);
		ppc = pc2;   
    }
    cnf = NULL;
}

void removeClause(clauselist *&cnf, clauselist *&cl)
{
    if (cl == cnf) //�����ͷ�Ӿ� 
        cnf = cnf->next;  
    else
    {
        clauselist* ppc = cnf;
        while (ppc && ppc->next != cl)
            ppc = ppc->next;     
        ppc->next = ppc->next->next;
    }
    clausenode* pn1, * pn2;
    for (pn1 = cl->nodehead; pn1;){
    	pn2 = pn1->next;
		free(pn1);
		pn1 = pn2; 
	}
    free(cl);
    cl = NULL;
}

void removeNode(clausenode *&head, clausenode *&nd)
{
    clausenode* ppn = head;
    if (ppn == nd)  //���Ҫɾ��ͷ�ڵ�
        head = head->next;  
    else
    {
        while (ppn && ppn->next != nd)
            ppn = ppn->next;  //��λ�ڵ�
        ppn->next = ppn->next->next;  
    }
    free(nd);
    nd = NULL;
}

int addClause(clauselist *cl, clauselist *&cnf)
{
    if (cl)//�ײ巨���뵥�Ӿ�
    {
        cl->next = cnf;
        cnf = cl;
        return TRUE;
    }
    return FALSE; 
}

int isUnitClause(clauselist *cl)
{
    if (cl->nodehead != NULL) {
        if (cl->nodehead->next == NULL)
            return TRUE;
    }
    return FALSE;
}

void simplify(clauselist *s, clauselist *&cnf)
{
    clauselist* tmp;
    int n = s->nodehead->literal;//Ҫɾ���ı�Ԫ��ֵ
    for (clauselist* ppc = cnf; ppc; ppc = tmp) 
    {
        tmp = ppc->next;
        for (clausenode* ppn = ppc->nodehead; ppn; ppn = ppn->next)
        {
            if (ppn->literal == n)
            {
                removeClause(cnf, ppc);//��������ñ�Ԫ ɾ�����Ӿ�
                break;
            }
            if (ppn->literal == -n)
            {
                removeNode(ppc->nodehead, ppn);//��������ñ�Ԫ�ķ� ɾ���ýڵ�
                ppc->listnum--;
                break;
            }
        }
    }
}

int emptyClause(clauselist *cnf)
{
    for (clauselist *p = cnf; p; p = p->next)
        if (!p->nodehead)
            return TRUE;
    return FALSE;
}

void getcopy(clauselist *&forgery, clauselist* cnf)
{
    clauselist* ppc, * pc;  
    clausenode* ppn, * pn;    
    forgery = (clauselist*)malloc(sizeof(clauselist));
    forgery->nodehead = (clausenode*)malloc(sizeof(clausenode));
    forgery->next = NULL;
    forgery->nodehead->next = NULL;
    forgery->listnum = 0;
    for (pc = cnf, ppc = forgery; pc != NULL; pc = pc->next, ppc = ppc->next)//���Ƶ�ǰcnf Ϊ������׼��
    {
        for (pn = pc->nodehead, ppn = ppc->nodehead; pn != NULL; pn = pn->next, ppn = ppn->next)
        {
            ppc->listnum++;
            ppn->literal = pn->literal;
            ppn->next = (clausenode*)malloc(sizeof(clausenode));
            ppn->next->next = NULL;
            if (pn->next == NULL)
                free(ppn->next), ppn->next = NULL;
        }
        ppc->next = (clauselist*)malloc(sizeof(clauselist));
        ppc->next->nodehead = (clausenode*)malloc(sizeof(clausenode));
        ppc->next->next = NULL;
        ppc->next->nodehead->next = NULL;
        ppc->next->listnum = 0;
        if (pc->next == NULL){
        	free(ppc->next->nodehead);
			free(ppc->next);
			ppc->next = NULL;
		}
    }
}

int varFirst(clauselist *cnf)
{
	clauselist *pc = cnf;
	return abs(pc->nodehead->literal);
} 
int varMax(clauselist *cnf)  //ѡȡ�ܳ��ִ������ı�Ԫ
{
    int *cnt = (int*)malloc(sizeof(int) * (varnum * 2 + 1));
    for (int i = 0; i <= varnum * 2; i++)  //��ʼ��cnt����
        cnt[i] = 0;
    for (clauselist* pc = cnf; pc; pc = pc->next)
        for (clausenode* pn = pc->nodehead; pn; pn = pn->next)
        {
            if (pn->literal > 0)
                cnt[pn->literal]++;   
            else
                cnt[varnum - pn->literal]++;
        }
    int maxBool, maxTimes = 0;
    for (int i = 1; i <= varnum; i++)   //ѡ���Ԫ��
        if (cnt[i] > maxTimes){
        	maxTimes = cnt[i];
			maxBool = i;
		}
            
    if (maxTimes == 1)  //��Ԫ�ķ�
    {
        for (int i = varnum + 1; i <= varnum * 2; i++)
            if (cnt[i] > maxTimes){
            	maxTimes = cnt[i];
				maxBool = varnum - i;  
			}
                
    }
    free(cnt);
    return maxBool;
}


int varMax2(clauselist* cnf)            //ѡȡ�ڶ��Ӿ���ִ������ı�Ԫ
{
    double *weight = (double*)malloc(sizeof(double) * (varnum * 2 + 1));
    for (int i = 0; i <= varnum * 2; i++)
        weight[i] = 0.0;
    for (clauselist* pc = cnf; pc; pc = pc->next)
        for (clausenode* pn = pc->nodehead; pn; pn = pn->next)
        {
            if (pn->literal > 0)
                weight[pn->literal] += pow(2.0, (double)(-(pc->listnum)));
            else
                weight[varnum - pn->literal] += pow(2.0, (double)(-(pc->listnum)));
        }
    double maxWeight = 0.0;
    int maxBool=0;
    for (int i = 1; i <= varnum; i++)
        if (weight[i] + weight[i + varnum] > maxWeight)
        {
            maxWeight = weight[i] + weight[i + varnum];
			maxBool = i;
        }
    if (weight[maxBool] < weight[maxBool + varnum])
        maxBool = -maxBool;
    free(weight);
    return maxBool;
}
//DPLL�㷨 
int dpll(clauselist *&list, int *assignment, int op) 
{
    int flag = 1;
    clauselist *p;
    while (flag) {
        flag = 0;
        p = list;//�Ӿ伯 
        while (p && !isUnitClause(p)) //Ѱ�ҵ�λ�Ӿ䣬�ҵ��˳�ѭ�� 
		{
            p = p->next;
        }
        if (p != NULL) 
		{
            if (p->nodehead->literal>0) 
			{
                assignment[p->nodehead->literal] = 1;//����Ϊ�� 
            } 
			else 
			{
                assignment[-p->nodehead->literal] = 0;//����Ϊ�� 
            }
            simplify(p, list);//pָ��λԪ�Ӿ䣬ɾ����λ�Ӿ� 
            if (list == NULL) return TRUE;
            else if (emptyClause(list)) return FALSE;//���ֿ��Ӿ䣬�η�֧��ͨ����������һ�ڵ㣬����һ��֧ 
            flag = 1;
        }
    }
    int v;
    if (op == 1) v = varFirst(list);
    else if (op == 2) v = varMax(list);
    else if (op == 3) v = varMax2(list);
    clauselist *SingleClause = (clauselist*)malloc(sizeof(clauselist)), *forgery;
    SingleClause->nodehead = (clausenode*)malloc(sizeof(clausenode));
    SingleClause->next = NULL;
    SingleClause->nodehead->literal = v;
    SingleClause->nodehead->next = NULL;
    SingleClause->listnum = 1;//��ѡ�еı�Ԫ��ɵĵ�λ�Ӿ� 
    getcopy(forgery, list);//��list���Ƶ�forgery 
    addClause(SingleClause, forgery);//����λ�Ӿ�����Ӿ伯 
    int ans = dpll(forgery, assignment, op);//�������� 
    if (ans == TRUE) return TRUE;
    destroyClause(forgery);//ɾ�������Ӿ伯 
    //������һ��֧vȡ��ֵ 
    SingleClause = (clauselist*)malloc(sizeof(clauselist));
    SingleClause->nodehead = (clausenode*)malloc(sizeof(clausenode));
    SingleClause->next = NULL;
    SingleClause->nodehead->literal = -v;
    SingleClause->nodehead->next = NULL;
    SingleClause->listnum = 1;
    addClause(SingleClause, list);
    getcopy(forgery, list);
    ans = dpll(forgery, assignment, op);
    destroyClause(forgery);
    return ans;
}

