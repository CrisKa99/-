#include "X-Sudoku.h"

int main()
{
    int op = 1, op1 = 1, op2 = 1, op3=1,flag_cnf, difficulty;
    int d;
    clauselist *head=(clauselist*)malloc(sizeof(clauselist));
    clock_t start, finish;
    double time1, time2, rate;
    bool res;
    char filename[500],wfilename[500];
    while (op)
    {
        system("cls");
        printf("\n----------------------------->> MENU <<----------------------------------\n");
        printf("\n1.cnf�ļ����\n2.������Ϸ\n0.�˳�                \n\n");
        printf("--------------------------------------------------------------------------\n>>");
        printf("������ѡ��0~2��\n");
        printf("--------------------------------------------------------------------------\n>>");
        scanf("%d", &op);
        switch (op)
        {
        case 1:
            flag_cnf = 1, op1 = 1; 
            while (op1)
            {
                system("cls");
                printf("\n----------------------------->> MENU <<----------------------------------\n");
    			printf("\n1.��ȡ�ļ�\n2.�����ļ�\n3.dpll���\n4.��ӡ���������\n0.�˳�\n\n");
    			printf("������ѡ��0~4��\n");
    			printf("--------------------------------------------------------------------------\n>>");
                scanf("%d", &op1);
                switch (op1)
                {
                case 1: {
                    printf("����������ļ���ַ:\n");  
                    scanf("%s", filename);		
					d=readcnf(filename,head);
                    if (d == TRUE){
						printf("��ȡ�ɹ���\n");
        				printf("��Ԫ����Ϊ��%d  �Ӿ�����Ϊ��%d",varnum,cnum);
					}
                    else
                        printf("��ȡʧ��!\n");
					getchar(); 
                 	printf("\n------------------PLEASE ���س�������-----------------\n"); 
                    getchar();
                    break;}
                case 2:{
                    if (d){
                    	printf("�ļ����������Ӿ䣺\n");
						for(clauselist *lp=head;lp;lp=lp->next){
							for(clausenode *np=lp->nodehead;np;np=np->next){
								printf("%d ",np->literal);
							}
							printf("\n");
						}
					}
                    else
                    	printf("��δ��ȡ��\n");
                    printf("\n------------------PLEASE ���س�������-----------------\n"); 
                    getchar(), getchar();
                    break;}
                case 3:{
                    if (d==FALSE)
                        printf("��δ��ȡ!\n ");
                    else
                    {
                    	printf("----��ѡ���Ԫѡȡ���ԣ�\n");
                    	printf("----0:�˳�\n");
                    	printf("----1:ѡȡ��һ����Ԫ\n");
                    	printf("----2:ѡȡ�����Ԫ�г��ִ������ı�Ԫ\n");
                    	printf("----3:ѡȡ���Ӿ��г��ִ������ı�Ԫ\n");
                    	scanf("%d", &op3);
                    	assignment = (int*)malloc(sizeof(int) * (varnum + 1));
	                    for (int i = 0; i <= varnum; i++)
	                        assignment[i] = 1;
	                    start = clock();
	                    res = dpll(head, assignment, op3);
	                    finish = clock();
	                    if (!res) printf("�������㣡\n");
	                    else printf("�����㣡\n");
	                    
	                    time1 = (double)(finish - start);
	                    printf("������ɣ�ʱ��t = %lfms\n", time1);
	                    time2=time1;
	                    while(op3)
	                        {
	                        	time1 = time2;
	                        	printf("\n----���ٴ�ѡ���Ԫѡȡ���ԣ�\n");
		                    	scanf("%d", &op3);
		                    	if(op3 == 0) break;
		                        for (int i = 0; i <= varnum; i++)
		                            assignment[i] = 1;
		                        start = clock();
		                        res = dpll(head, assignment, op3);
		                        if (!res) printf("�������㣡\n");
	            		        else printf("�����㣡\n");
		                        finish = clock();
		                        time2 = (double)(finish - start);
		                        printf("������ɣ�ʱ��t = %lfms\n", time2);
		                    
		                        if(time1 == 0 || time1 == time2 || time2 == 0) printf("ʱ���Ż���Ϊ 0 %%\n"); 
		                        else if(time1 < time2) printf("ʱ���Ż���Ϊ-%lf %%", (100 * 1.0 *(time2 - time1)) / time1);
			                    else printf("ʱ���Ż���Ϊ%lf %%", (100 * 1.0 *(time1 - time2)) / time1);
			                    
							}
	                
                    }
		                    
                    printf("\n------------------PLEASE ���س�������-----------------\n"); 
                    getchar(), getchar();
                    break;}
                case 4:{
                    for (int i = 1; i <= varnum; i++)
                    {
                        if (assignment[i])
                            printf("%d \n", i);
                        else
                            printf("%d \n", -i);
                    }
                    if (!d)
                        printf("��δ��ȡ!\n ");
                    else{
	                    printf("�����뱣���ļ���ַ:\n");
	                    scanf("%s", wfilename);			
                        if (printcnf(res, wfilename, assignment, time1, time2))
                            printf("����ѱ���\n");
                        else
                            printf("�������ʧ��!\n");
                    }
                    printf("\n------------------PLEASE ���س�������-----------------\n"); 
                    getchar(), getchar();
                    break;}
                case 0:
                    break;
                }
            }
            break;
        case 2:
             op2 = 1;
            while (op2)
            {
                system("cls");
                printf("\n---------------------------*** X-Sudoku***------------------------\n\n");
                printf("\n--------------------------------OPTIONS---------------------------------\n\n");
                printf("     	              1.Sudoku   0.Return                            \n\n");
                printf("------------------------------------------------------------------------\n");
                scanf("%d", &op2);
                switch (op2)
                {
                case 1:     
                    Sudoku();
                    printf("\n------------------PLEASE ���س�������-----------------\n"); 
                    getchar(), getchar();
                    break;
                default:
                    break;
                }
            }
            break;
        case 0:
            break;
        }
    }
    return 0;
}
