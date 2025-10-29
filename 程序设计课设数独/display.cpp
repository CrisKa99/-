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
        printf("\n1.cnf文件求解\n2.数独游戏\n0.退出                \n\n");
        printf("--------------------------------------------------------------------------\n>>");
        printf("请输入选择0~2：\n");
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
    			printf("\n1.读取文件\n2.遍历文件\n3.dpll求解\n4.打印结果并保存\n0.退出\n\n");
    			printf("请输入选择0~4：\n");
    			printf("--------------------------------------------------------------------------\n>>");
                scanf("%d", &op1);
                switch (op1)
                {
                case 1: {
                    printf("请输入测试文件地址:\n");  
                    scanf("%s", filename);		
					d=readcnf(filename,head);
                    if (d == TRUE){
						printf("读取成功！\n");
        				printf("变元数量为：%d  子句数量为：%d",varnum,cnum);
					}
                    else
                        printf("读取失败!\n");
					getchar(); 
                 	printf("\n------------------PLEASE 按回车键继续-----------------\n"); 
                    getchar();
                    break;}
                case 2:{
                    if (d){
                    	printf("文件中有如下子句：\n");
						for(clauselist *lp=head;lp;lp=lp->next){
							for(clausenode *np=lp->nodehead;np;np=np->next){
								printf("%d ",np->literal);
							}
							printf("\n");
						}
					}
                    else
                    	printf("尚未读取！\n");
                    printf("\n------------------PLEASE 按回车键继续-----------------\n"); 
                    getchar(), getchar();
                    break;}
                case 3:{
                    if (d==FALSE)
                        printf("尚未读取!\n ");
                    else
                    {
                    	printf("----请选择变元选取策略：\n");
                    	printf("----0:退出\n");
                    	printf("----1:选取第一个变元\n");
                    	printf("----2:选取总体变元中出现次数最多的变元\n");
                    	printf("----3:选取短子句中出现次数最多的变元\n");
                    	scanf("%d", &op3);
                    	assignment = (int*)malloc(sizeof(int) * (varnum + 1));
	                    for (int i = 0; i <= varnum; i++)
	                        assignment[i] = 1;
	                    start = clock();
	                    res = dpll(head, assignment, op3);
	                    finish = clock();
	                    if (!res) printf("不可满足！\n");
	                    else printf("可满足！\n");
	                    
	                    time1 = (double)(finish - start);
	                    printf("运算完成，时间t = %lfms\n", time1);
	                    time2=time1;
	                    while(op3)
	                        {
	                        	time1 = time2;
	                        	printf("\n----请再次选择变元选取策略：\n");
		                    	scanf("%d", &op3);
		                    	if(op3 == 0) break;
		                        for (int i = 0; i <= varnum; i++)
		                            assignment[i] = 1;
		                        start = clock();
		                        res = dpll(head, assignment, op3);
		                        if (!res) printf("不可满足！\n");
	            		        else printf("可满足！\n");
		                        finish = clock();
		                        time2 = (double)(finish - start);
		                        printf("运算完成，时间t = %lfms\n", time2);
		                    
		                        if(time1 == 0 || time1 == time2 || time2 == 0) printf("时间优化率为 0 %%\n"); 
		                        else if(time1 < time2) printf("时间优化率为-%lf %%", (100 * 1.0 *(time2 - time1)) / time1);
			                    else printf("时间优化率为%lf %%", (100 * 1.0 *(time1 - time2)) / time1);
			                    
							}
	                
                    }
		                    
                    printf("\n------------------PLEASE 按回车键继续-----------------\n"); 
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
                        printf("尚未读取!\n ");
                    else{
	                    printf("请输入保存文件地址:\n");
	                    scanf("%s", wfilename);			
                        if (printcnf(res, wfilename, assignment, time1, time2))
                            printf("结果已保存\n");
                        else
                            printf("结果保存失败!\n");
                    }
                    printf("\n------------------PLEASE 按回车键继续-----------------\n"); 
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
                    printf("\n------------------PLEASE 按回车键继续-----------------\n"); 
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
