#include "solver.h"

void printmenu()
{
    printf("----------------------------->> MENU FOR SUDOKU<<----------------------------------\n");
    printf("\n1.生成数独\n2.填写数独\n3.展示答案\n");
	printf("请输入选择0~4：\n");
    printf("--------------------------------------------------------------------------\n>>");
}
void Shuffle(int arr[], int n){//使用随机数来交换数组中的元素，从而实现数组的随机打乱
    srand(time(NULL)); // 随机选一个数与前面的数交换
    for (int i = n - 1; i > 0; i--){
        int j = rand() % (i + 1);
        int temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
}
int Fill_Box(int origin[10][10], int player[10][10], int answerecord[10][10], int rowStart, int colStart){
    int n[9 - 3];       // 除了对角线以外的6个格子
    int flag1[9] = {0}; // 标记1-9是否已经填入
    for (int i = 0; i < 3; i++)
    {
        flag1[origin[rowStart + i][colStart + i] - 1] = 1; // 对角线的3个格子上的数字标记为已填入
    }
    int index = 0;
    for (int i = 0; i < 9; i++)
    {
        if (!flag1[i])
            n[index++] = i + 1;
    }
    Shuffle(n, 6); // 打乱数组
    index = 0;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (origin[rowStart + i][colStart + j] == 0) // 没有填入
            {
                origin[rowStart + i][colStart + j] = n[index];
                player[rowStart + i][colStart + j] = n[index];
                answerecord[rowStart + i][colStart + j] = n[index];
                index++;
            }
        }
    }
    return TRUE;
}

int Writecnf(int origin[10][10], int num, char name[])
{
    FILE *fp=fopen(name, "w");
    if (!fp)
    {
        printf("ERROR!\n");
        return FALSE;
    }
    fprintf(fp, "c %s\n", name);
    fprintf(fp, "p cnf 729 %d\n", num + 12654); // 约束条件
    
    for (int i = 1; i <= 9; i++)
    {
        for (int j = 1; j <= 9; j++)
        {
            if (origin[i][j] != 0)
                fprintf(fp, "%d 0\n", (i - 1) * 81 + (j - 1) * 9 + origin[i][j]);
        }
    }
    for (int i = 1; i <= 9; i++){
        for (int j = 1; j <= 9; j++){
            for (int k = 1; k <= 9; k++){
                fprintf(fp, "%d ", (i - 1) * 9 * 9 + (j - 1) * 9 + k);
            }
            fprintf(fp, "0\n");
        }
    }
    for (int i = 1; i <= 9; i++){//每个格子不能填入两个数字
        for (int j = 1; j <= 9; j++){
            for (int k = 1; k <= 9; k++){
                for (int l = k + 1; l <= 9; l++){
                    fprintf(fp, "%d %d 0\n", 0 - ((i - 1) * 9 * 9 + (j - 1) * 9 + k), 0 - ((i - 1) * 9 * 9 + (j - 1) * 9 + l));
                }
            }
        }
    }
    for (int i = 1; i <= 9; i++){//行约束
        for (int j = 1; j <= 9; j++){
            for (int k = 1; k <= 9; k++){
                fprintf(fp, "%d ", (i - 1) * 9 * 9 + (k - 1) * 9 + j);
            }
            fprintf(fp, "0\n");
        }
    }
    for (int i = 1; i <= 9; i++){// 每一行不能填入两个相同的数字
        for (int j = 1; j <= 9; j++){
            for (int k = 1; k <= 9; k++){
                for (int l = k + 1; l <= 9; l++){
                    fprintf(fp, "%d %d 0\n", 0 - ((i - 1) * 9 * 9 + (k - 1) * 9 + j), 0 - ((i - 1) * 9 * 9 + (l - 1) * 9 + j));
                }
            }
        }
    }
    for (int i = 1; i <= 9; i++){//列约束
        for (int j = 1; j <= 9; j++){
            for (int k = 1; k <= 9; k++){
                fprintf(fp, "%d ", (k - 1) * 9 * 9 + (i - 1) * 9 + j);
            }
            fprintf(fp, "0\n");
        }
    }
    for (int i = 1; i <= 9; i++){// 每一列不能填入两个相同的数字
        for (int j = 1; j <= 9; j++){
            for (int k = 1; k <= 9; k++){
                for (int l = k + 1; l <= 9; l++){
                    fprintf(fp, "%d %d 0\n", 0 - ((k - 1) * 9 * 9 + (i - 1) * 9 + j), 0 - ((l - 1) * 9 * 9 + (i - 1) * 9 + j));
                }
            }
        }
    }
    for (int i = 1; i <= 9; i += 3){//3x3宫格约束
        for (int j = 1; j <= 9; j += 3){
            for (int k = 1; k <= 9; k++){
                for (int l = 0; l < 3; l++){
                    for (int m = 0; m < 3; m++){
                        fprintf(fp, "%d ", ((i + l - 1) * 9 * 9 + (j + m - 1) * 9 + k));
                    }
                }
                fprintf(fp, "0\n");
            }
        }
    }
    for (int i = 1; i <= 9; i += 3){// 每个3x3宫格不能填入两个相同的数字
        for (int j = 1; j <= 9; j += 3){
            for (int k = 1; k <= 9; k++){
                for (int l = 0; l < 3; l++){
                    for (int m = 0; m < 3; m++){
                        for (int n = k + 1; n <= 9; n++){
                            fprintf(fp, "%d %d 0\n", 0 - ((i + l - 1) * 9 * 9 + (j + m - 1) * 9 + k), 0 - ((i + l - 1) * 9 * 9 + (j + m - 1) * 9 + n));
                        }
                    }
                }
            }
        }
    }
    
    for (int i = 1; i <= 9; i++){// 主对角线约束 
        for (int j = 1; j <= 9; j++){
            fprintf(fp, "%d ", (j - 1) * 9 * 9 + (j - 1) * 9 + i);
        }
        fprintf(fp, "0\n");
    }
    for (int i = 1; i <= 9; i++){// 主对角线不能填入两个相同的数字
        for (int j = 1; j <= 9; j++){
            for (int k = j + 1; k <= 9; k++){
                fprintf(fp, "%d %d 0\n", 0 - ((j - 1) * 9 * 9 + (j - 1) * 9 + i), 0 - ((k - 1) * 9 * 9 + (k - 1) * 9 + i));
            }
        }
    }
    for (int i = 1; i <= 9; i++){// 副对角线必须填入1-9
        for (int j = 1; j <= 9; j++){
            fprintf(fp, "%d ", (j - 1) * 9 * 9 + (9 - j + 1) * 9 + i);
        }
        fprintf(fp, "0\n");
    }
    for (int i = 1; i <= 9; i++){// 副对角线不能填入两个相同的数字
        for (int j = 1; j <= 9; j++){
            for (int k = j + 1; k <= 9; k++){
                fprintf(fp, "%d %d 0\n", 0 - ((j - 1) * 9 * 9 + (9 - j + 1) * 9 + i), 0 - ((k - 1) * 9 * 9 + (9 - k + 1) * 9 + i));
            }
        }
    }
    fclose(fp);
    return TRUE;
}
int Generate(int origin[10][10], int player[10][10], int answerecord[10][10], int prompt[10][10], int num, int value[10*10*10 + 1])
{
    char name[100] = "Sudoku.cnf";
START:
    srand(time(NULL));
    for (int i = 1; i <= 9; i++)
        for (int j = 1; j <= 9; j++)
        {
            origin[i][j] = 0;
            player[i][j] = 0;
            answerecord[i][j] = 0;
            prompt[i][j] = TRUE; // 默认全为提示数字
        }
    int n[9];
    for (int i = 0; i < 9; i++)
        n[i] = i + 1;
    Shuffle(n, 9); // 打乱数组
    int index = 0;
    for (int i = 1; i <= 9; i++) // 对角线填入1-9
    {
        origin[i][i] = n[index];
        player[i][i] = n[index];
        answerecord[i][i] = n[index];
        index++;
    }
    for (int i = 1; i <= 9; i += 3)
    {
        Fill_Box(origin, player, answerecord, i, i); // 主对角线3个填充3x3的宫格
    }
    
    Writecnf(origin, 27, name); // 将数独约束条件写入文件
    clauselist *p=(clauselist*)malloc(sizeof(clauselist));
    readcnf(name,p); 
    for (int i = 1; i <= 9 * 9 * 9; i++)
        value[i] = FALSE;
    if (dpll(p, value, 3) == FALSE) // 求解数独
        goto START;
    // 将DPLL的结果填入数独
    for (int i = 1; i <= 9 * 9 * 9 + 1; i++)
    {
        if (value[i] == TRUE)
        {
            int row = (i - 1) / (9 * 9) + 1;
            int col = (i - 1) / 9 % 9 + 1;
            int v = (i - 1) % 9 + 1;
            origin[row][col] = v;
            player[row][col] = v;
            answerecord[row][col] = v;
        }
    }
    int remove = 81 - num;// 挖洞
    int single = remove / 9; 
    int res = remove - 9 * single;
    // int c[SIZE]={9-single};
    for (int row = 1; row <= 9; row++) // 每行挖single个
    {
        int s = single;
        while (s)
        {
            int col = rand() % 9 + 1;
            if (origin[row][col] != 0) // 没有被挖
            {
                origin[row][col] = 0;
                player[row][col] = 0;
                answerecord[row][col] = 0;
                prompt[row][col] = FALSE;
                s--;
            }
        }
    }
    while (res) // 挖剩下的
    {
        int row = rand() % 9 + 1;
        int col = rand() % 9 + 1;
        if (origin[row][col] != 0)
        {
            origin[row][col] = 0;
            player[row][col] = 0;
            answerecord[row][col] = 0;
            prompt[row][col] = FALSE;
            res--;
            // c[row-1]--;
        }
    }
    return TRUE;
}
void PrintSudoku(int origin[9 + 1][9 + 1]){
    for (int i = 1; i <= 9; i++){
        for (int j = 1; j <= 9; j++){
            if (origin[i][j] == 0) // 未填入
                printf(" *");
            else // 已填入
                printf("%2d", origin[i][j]);
            if (j % 3 == 0 && j != 9) // 每3列打印一个竖线
                printf(" |");
        }
        printf("\n");
        if (i % 3 == 0 && i != 9) // 每3行打印一个横线
            printf("-------+-------+-------\n");
    }
}
int isvalid(int origin[9 + 1][9 + 1], int row, int col, int v){
    for (int i = 1; i <= 9; i++)
    {
        if (origin[row][i] == v || origin[i][col] == v) // 行或列有重复
            return FALSE;
    }
    int startRow = (row - 1) / 3 * 3 + 1; // 宫格的起始行
    int startCol = (col - 1) / 3 * 3 + 1; // 宫格的起始列
    for (int i = startRow; i < startRow + 3; i++)
    {
        for (int j = startCol; j < startCol + 3; j++)
        {
            if (origin[i][j] == v) // 宫格内有重复
                return FALSE;
        }
    }
    // 检查主对角线
    if (row == col)
    {
        for (int i = 1; i <= 9; i++)
        {
            if (origin[i][i] == v)
                return FALSE;
        }
    }
    // 检查副对角线
    if (row + col == 9 + 1)
    {
        for (int i = 1; i <= 9; i++)
        {
            if (origin[i][9 - i + 1] == v)
                return FALSE;
        }
    }
    return TRUE;
}
void Play_Sudoku(int origin[9 + 1][9 + 1], int prompt[9 + 1][9 + 1])
{
    system("cls");       // 清屏
    PrintSudoku(origin); // 打印初始数独
    printf("\n");
    while (1)
    {
        int row, col, v;
        printf("请分别输入要填入的行，列，数值大小: ");
        scanf("%d", &row);
        if (row == 0) // 退出
        {
            system("cls");
            return;
        }
        scanf("%d%d", &col, &v);
        if (row < 1 || row > 9 || col < 1 || col > 9 || v < 1 || v > 9) // 输入不合法
        {
            printf(" 输入无效\n");
            continue;
        }
        if (prompt[row][col]) // 是提示数
        {
            printf("输入的是提示数\n");
            continue;
        }
        if (!isvalid(origin, row, col, v)) // 不符合数独规则
        {
            printf(" 答案错误.\n");
            continue;
        }
        else // 符合数独规则
        {
            origin[row][col] = v;
            system("cls");
            PrintSudoku(origin); // 打印新数独
            printf("\n");
        }
    }
}
int Slove(int origin[9 + 1][9 + 1], int value[9 * 9 * 9 + 1])
{
    for (int i = 1; i <= 9 * 9 * 9 + 1; i++)
    {
        if (value[i] == TRUE)
        {
            int row = (i - 1) / (9 * 9) + 1;
            int col = (i - 1) / 9 % 9 + 1;
            int v = (i - 1) % 9 + 1;
            origin[row][col] = v;
        }
    }
    return TRUE;
}
void Sudoku()
{
    system("cls");
    
    int num;//提示数的个数
    int prompt[10][10]; //记录是否为提示数字
    int origin[10][10];//生成的初始数独
    int player[10][10];  //用来玩的数独
    int answerecord[10][10]; // 保存答案的数独
    int value[10 * 10 * 10 + 1]; // 记录DPLL的结果
    for (int i = 1; i <= 10*10*10; i++)
        value[i] = FALSE;
    int opx = 1;
    int flag = 0; // 是否生成数独
    while (opx)
    {
    	printmenu();
        scanf("%d", &opx);
        system("cls");
        switch (opx){
	        case 1:{
	            printf("请输入数独的初始数字个数：\n");
	            scanf("%d", &num);
	            while (num < 18 || num > 81){
	                printf("错误！请重新输入：");
	                scanf("%d", &num);
	            }
	            if (Generate(origin, player, player, prompt, num, value))
	            {
	                printf("数独生成成功！\n");
	                flag = 1; // 生成成功
	            }
	            else
	                printf("数独生成失败！\n");
	            break;
	        }
	        case 2:
	        {
	            if (flag)
	            {
	                Play_Sudoku(player, prompt);
	                printmenu(); // 每次玩完跳转回来重新打印菜单
	            }
	            else
	                printf("请先生成数独\n");
	            break;
	        }
	        case 3:
	        {
	            if (flag)
	            {
	                
	                PrintSudoku(origin); // 打印原始数独
	                printf("\n");
	                if (Slove(answerecord, value)) // 求解数独
	                {
	                    printf("数独的正确答案：\n");
	                    PrintSudoku(answerecord); // 打印答案
	                    
	                }
	                else
	                    printf("数独无解\n"); // 无解
	                
	            }
	            else
	                printf("请先生成数独\n");
	            break;
	        }
	        case 0:
	        {
	            system("cls"); // 退出时清屏
	            break;
	        }
        }
    }
}
