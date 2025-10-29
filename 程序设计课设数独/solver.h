#include "cnfparser.h"

void destroyClause(clauselist*& cnf)
{
    clauselist *ppc, *pc2; 
    clausenode *pn1, *pn2; 
    ppc = cnf;
    while (ppc) 
    {
        pn1 = ppc->nodehead;   //循环删除子句
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
    if (cl == cnf) //如果是头子句 
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
    if (ppn == nd)  //如果要删除头节点
        head = head->next;  
    else
    {
        while (ppn && ppn->next != nd)
            ppn = ppn->next;  //定位节点
        ppn->next = ppn->next->next;  
    }
    free(nd);
    nd = NULL;
}

int addClause(clauselist *cl, clauselist *&cnf)
{
    if (cl)//首插法插入单子句
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
    int n = s->nodehead->literal;//要删除的变元的值
    for (clauselist* ppc = cnf; ppc; ppc = tmp) 
    {
        tmp = ppc->next;
        for (clausenode* ppn = ppc->nodehead; ppn; ppn = ppn->next)
        {
            if (ppn->literal == n)
            {
                removeClause(cnf, ppc);//如果包含该变元 删除该子句
                break;
            }
            if (ppn->literal == -n)
            {
                removeNode(ppc->nodehead, ppn);//如果包含该变元的反 删除该节点
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
    for (pc = cnf, ppc = forgery; pc != NULL; pc = pc->next, ppc = ppc->next)//复制当前cnf 为回溯做准备
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
int varMax(clauselist *cnf)  //选取总出现次数最多的变元
{
    int *cnt = (int*)malloc(sizeof(int) * (varnum * 2 + 1));
    for (int i = 0; i <= varnum * 2; i++)  //初始化cnt数组
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
    for (int i = 1; i <= varnum; i++)   //选择变元正
        if (cnt[i] > maxTimes){
        	maxTimes = cnt[i];
			maxBool = i;
		}
            
    if (maxTimes == 1)  //变元的反
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


int varMax2(clauselist* cnf)            //选取在短子句出现次数最多的变元
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
//DPLL算法 
int dpll(clauselist *&list, int *assignment, int op) 
{
    int flag = 1;
    clauselist *p;
    while (flag) {
        flag = 0;
        p = list;//子句集 
        while (p && !isUnitClause(p)) //寻找单位子句，找到退出循环 
		{
            p = p->next;
        }
        if (p != NULL) 
		{
            if (p->nodehead->literal>0) 
			{
                assignment[p->nodehead->literal] = 1;//文字为正 
            } 
			else 
			{
                assignment[-p->nodehead->literal] = 0;//文字为负 
            }
            simplify(p, list);//p指向单位元子句，删除单位子句 
            if (list == NULL) return TRUE;
            else if (emptyClause(list)) return FALSE;//出现空子句，次分支不通，回退至上一节点，走另一分支 
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
    SingleClause->listnum = 1;//被选中的变元组成的单位子句 
    getcopy(forgery, list);//将list复制到forgery 
    addClause(SingleClause, forgery);//将单位子句插入子句集 
    int ans = dpll(forgery, assignment, op);//继续搜素 
    if (ans == TRUE) return TRUE;
    destroyClause(forgery);//删除复制子句集 
    //进行另一分支v取假值 
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

