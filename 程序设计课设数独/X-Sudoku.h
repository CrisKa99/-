#include "solver.h"

void printmenu()
{
    printf("----------------------------->> MENU FOR SUDOKU<<----------------------------------\n");
    printf("\n1.��������\n2.��д����\n3.չʾ��\n");
	printf("������ѡ��0~4��\n");
    printf("--------------------------------------------------------------------------\n>>");
}
void Shuffle(int arr[], int n){//ʹ������������������е�Ԫ�أ��Ӷ�ʵ��������������
    srand(time(NULL)); // ���ѡһ������ǰ���������
    for (int i = n - 1; i > 0; i--){
        int j = rand() % (i + 1);
        int temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
}
int Fill_Box(int origin[10][10], int player[10][10], int answerecord[10][10], int rowStart, int colStart){
    int n[9 - 3];       // ���˶Խ��������6������
    int flag1[9] = {0}; // ���1-9�Ƿ��Ѿ�����
    for (int i = 0; i < 3; i++)
    {
        flag1[origin[rowStart + i][colStart + i] - 1] = 1; // �Խ��ߵ�3�������ϵ����ֱ��Ϊ������
    }
    int index = 0;
    for (int i = 0; i < 9; i++)
    {
        if (!flag1[i])
            n[index++] = i + 1;
    }
    Shuffle(n, 6); // ��������
    index = 0;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (origin[rowStart + i][colStart + j] == 0) // û������
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
    fprintf(fp, "p cnf 729 %d\n", num + 12654); // Լ������
    
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
    for (int i = 1; i <= 9; i++){//ÿ�����Ӳ���������������
        for (int j = 1; j <= 9; j++){
            for (int k = 1; k <= 9; k++){
                for (int l = k + 1; l <= 9; l++){
                    fprintf(fp, "%d %d 0\n", 0 - ((i - 1) * 9 * 9 + (j - 1) * 9 + k), 0 - ((i - 1) * 9 * 9 + (j - 1) * 9 + l));
                }
            }
        }
    }
    for (int i = 1; i <= 9; i++){//��Լ��
        for (int j = 1; j <= 9; j++){
            for (int k = 1; k <= 9; k++){
                fprintf(fp, "%d ", (i - 1) * 9 * 9 + (k - 1) * 9 + j);
            }
            fprintf(fp, "0\n");
        }
    }
    for (int i = 1; i <= 9; i++){// ÿһ�в�������������ͬ������
        for (int j = 1; j <= 9; j++){
            for (int k = 1; k <= 9; k++){
                for (int l = k + 1; l <= 9; l++){
                    fprintf(fp, "%d %d 0\n", 0 - ((i - 1) * 9 * 9 + (k - 1) * 9 + j), 0 - ((i - 1) * 9 * 9 + (l - 1) * 9 + j));
                }
            }
        }
    }
    for (int i = 1; i <= 9; i++){//��Լ��
        for (int j = 1; j <= 9; j++){
            for (int k = 1; k <= 9; k++){
                fprintf(fp, "%d ", (k - 1) * 9 * 9 + (i - 1) * 9 + j);
            }
            fprintf(fp, "0\n");
        }
    }
    for (int i = 1; i <= 9; i++){// ÿһ�в�������������ͬ������
        for (int j = 1; j <= 9; j++){
            for (int k = 1; k <= 9; k++){
                for (int l = k + 1; l <= 9; l++){
                    fprintf(fp, "%d %d 0\n", 0 - ((k - 1) * 9 * 9 + (i - 1) * 9 + j), 0 - ((l - 1) * 9 * 9 + (i - 1) * 9 + j));
                }
            }
        }
    }
    for (int i = 1; i <= 9; i += 3){//3x3����Լ��
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
    for (int i = 1; i <= 9; i += 3){// ÿ��3x3����������������ͬ������
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
    
    for (int i = 1; i <= 9; i++){// ���Խ���Լ�� 
        for (int j = 1; j <= 9; j++){
            fprintf(fp, "%d ", (j - 1) * 9 * 9 + (j - 1) * 9 + i);
        }
        fprintf(fp, "0\n");
    }
    for (int i = 1; i <= 9; i++){// ���Խ��߲�������������ͬ������
        for (int j = 1; j <= 9; j++){
            for (int k = j + 1; k <= 9; k++){
                fprintf(fp, "%d %d 0\n", 0 - ((j - 1) * 9 * 9 + (j - 1) * 9 + i), 0 - ((k - 1) * 9 * 9 + (k - 1) * 9 + i));
            }
        }
    }
    for (int i = 1; i <= 9; i++){// ���Խ��߱�������1-9
        for (int j = 1; j <= 9; j++){
            fprintf(fp, "%d ", (j - 1) * 9 * 9 + (9 - j + 1) * 9 + i);
        }
        fprintf(fp, "0\n");
    }
    for (int i = 1; i <= 9; i++){// ���Խ��߲�������������ͬ������
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
            prompt[i][j] = TRUE; // Ĭ��ȫΪ��ʾ����
        }
    int n[9];
    for (int i = 0; i < 9; i++)
        n[i] = i + 1;
    Shuffle(n, 9); // ��������
    int index = 0;
    for (int i = 1; i <= 9; i++) // �Խ�������1-9
    {
        origin[i][i] = n[index];
        player[i][i] = n[index];
        answerecord[i][i] = n[index];
        index++;
    }
    for (int i = 1; i <= 9; i += 3)
    {
        Fill_Box(origin, player, answerecord, i, i); // ���Խ���3�����3x3�Ĺ���
    }
    
    Writecnf(origin, 27, name); // ������Լ������д���ļ�
    clauselist *p=(clauselist*)malloc(sizeof(clauselist));
    readcnf(name,p); 
    for (int i = 1; i <= 9 * 9 * 9; i++)
        value[i] = FALSE;
    if (dpll(p, value, 3) == FALSE) // �������
        goto START;
    // ��DPLL�Ľ����������
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
    int remove = 81 - num;// �ڶ�
    int single = remove / 9; 
    int res = remove - 9 * single;
    // int c[SIZE]={9-single};
    for (int row = 1; row <= 9; row++) // ÿ����single��
    {
        int s = single;
        while (s)
        {
            int col = rand() % 9 + 1;
            if (origin[row][col] != 0) // û�б���
            {
                origin[row][col] = 0;
                player[row][col] = 0;
                answerecord[row][col] = 0;
                prompt[row][col] = FALSE;
                s--;
            }
        }
    }
    while (res) // ��ʣ�µ�
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
            if (origin[i][j] == 0) // δ����
                printf(" *");
            else // ������
                printf("%2d", origin[i][j]);
            if (j % 3 == 0 && j != 9) // ÿ3�д�ӡһ������
                printf(" |");
        }
        printf("\n");
        if (i % 3 == 0 && i != 9) // ÿ3�д�ӡһ������
            printf("-------+-------+-------\n");
    }
}
int isvalid(int origin[9 + 1][9 + 1], int row, int col, int v){
    for (int i = 1; i <= 9; i++)
    {
        if (origin[row][i] == v || origin[i][col] == v) // �л������ظ�
            return FALSE;
    }
    int startRow = (row - 1) / 3 * 3 + 1; // �������ʼ��
    int startCol = (col - 1) / 3 * 3 + 1; // �������ʼ��
    for (int i = startRow; i < startRow + 3; i++)
    {
        for (int j = startCol; j < startCol + 3; j++)
        {
            if (origin[i][j] == v) // ���������ظ�
                return FALSE;
        }
    }
    // ������Խ���
    if (row == col)
    {
        for (int i = 1; i <= 9; i++)
        {
            if (origin[i][i] == v)
                return FALSE;
        }
    }
    // ��鸱�Խ���
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
    system("cls");       // ����
    PrintSudoku(origin); // ��ӡ��ʼ����
    printf("\n");
    while (1)
    {
        int row, col, v;
        printf("��ֱ�����Ҫ������У��У���ֵ��С: ");
        scanf("%d", &row);
        if (row == 0) // �˳�
        {
            system("cls");
            return;
        }
        scanf("%d%d", &col, &v);
        if (row < 1 || row > 9 || col < 1 || col > 9 || v < 1 || v > 9) // ���벻�Ϸ�
        {
            printf(" ������Ч\n");
            continue;
        }
        if (prompt[row][col]) // ����ʾ��
        {
            printf("���������ʾ��\n");
            continue;
        }
        if (!isvalid(origin, row, col, v)) // ��������������
        {
            printf(" �𰸴���.\n");
            continue;
        }
        else // ������������
        {
            origin[row][col] = v;
            system("cls");
            PrintSudoku(origin); // ��ӡ������
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
    
    int num;//��ʾ���ĸ���
    int prompt[10][10]; //��¼�Ƿ�Ϊ��ʾ����
    int origin[10][10];//���ɵĳ�ʼ����
    int player[10][10];  //�����������
    int answerecord[10][10]; // ����𰸵�����
    int value[10 * 10 * 10 + 1]; // ��¼DPLL�Ľ��
    for (int i = 1; i <= 10*10*10; i++)
        value[i] = FALSE;
    int opx = 1;
    int flag = 0; // �Ƿ���������
    while (opx)
    {
    	printmenu();
        scanf("%d", &opx);
        system("cls");
        switch (opx){
	        case 1:{
	            printf("�����������ĳ�ʼ���ָ�����\n");
	            scanf("%d", &num);
	            while (num < 18 || num > 81){
	                printf("�������������룺");
	                scanf("%d", &num);
	            }
	            if (Generate(origin, player, player, prompt, num, value))
	            {
	                printf("�������ɳɹ���\n");
	                flag = 1; // ���ɳɹ�
	            }
	            else
	                printf("��������ʧ�ܣ�\n");
	            break;
	        }
	        case 2:
	        {
	            if (flag)
	            {
	                Play_Sudoku(player, prompt);
	                printmenu(); // ÿ��������ת�������´�ӡ�˵�
	            }
	            else
	                printf("������������\n");
	            break;
	        }
	        case 3:
	        {
	            if (flag)
	            {
	                
	                PrintSudoku(origin); // ��ӡԭʼ����
	                printf("\n");
	                if (Slove(answerecord, value)) // �������
	                {
	                    printf("��������ȷ�𰸣�\n");
	                    PrintSudoku(answerecord); // ��ӡ��
	                    
	                }
	                else
	                    printf("�����޽�\n"); // �޽�
	                
	            }
	            else
	                printf("������������\n");
	            break;
	        }
	        case 0:
	        {
	            system("cls"); // �˳�ʱ����
	            break;
	        }
        }
    }
}
