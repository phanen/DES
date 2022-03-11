#include<stdio.h>
#include<stdlib.h>

long long Key = 0x133457799bbcdff1;//密钥64位
long long M = 0x012345678912323;//明文64位

//密钥矩阵
int key_matrix[8][8] = { 0 };
//密钥进行密钥置换后的矩阵

int key_matrix_plus[8][7] = { 0 };

//明文矩阵
int m_set[8][8] = { 0 };
//明文进行初始置换后
int m_set_plus[8][8] = { 0 };

int C[17][28] = { 0 };
int D[17][28] = { 0 };
//存储每一轮分开的密钥

int L[17][8][4] = { 0 };
int R[17][8][4] = { 0 };
//用于存储左明文和右明文

int L_plus[17][8][6] = { 0 };
//扩展以后的左支
int R_plus[17][8][6] = { 0 };
//扩展以后的右
int R_plus_temp[17][8][4] = { 0 };

int LR_plus[17][8][8] = { 0 };
int LR_plus_temp[17][8][8] = { 0 };

//存放每一轮移位运算产生的56位子密钥
int K[17][8][7] = { 0 };
//存放每一轮运算中，子密钥经过压缩置换后的48位
int K_plus[17][8][7] = { 0 };

//密钥置换表
int perm_key[8][7] = {
	{57,49,41,33,25,17,9},
	{1,58,50,42,34,26,18},
	{10,2,59,51,43,35,27},
	{19,11,3,60,52,44,36},
	{63,55,47,39,31,23,15},
	{7,62,54,46,38,30,22},
	{14,6,61,53,45,37,29},
	{21,13,5,28,10,12,4}
};

//子密钥每轮移动的位数
int num_shift[16] = { 1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1 };

//压缩置换表
int perm_K[8][6] = {
	{14,17,11,24,1,5},
	{3,28,15,6,21,10},
	{23,19,12,4,26,8},
	{16,7,27,20,13,2},
	{41,52,31,37,47,55},
	{30,40,51,45,33,48},
	{44,49,39,56,34,53},
	{46,42,50,36,29,32}

};

//打乱输入的ip置换表
int IP[8][8] = {
	{58, 50, 42, 34,  26,  18, 10,  2},
	{60, 52, 44, 36,  28,  20, 12,  4},
	{62, 54, 46, 38,  30,  22, 14,  6},
	{64, 56, 48, 40,  32,  24, 16,  8},
	{57, 49, 41, 33,  25,  17,  9,  1},
	{59, 51, 43, 35,  27,  19,  11, 3},
	{61, 53, 45, 37,  29,  21,  13, 5},
	{63, 55, 47, 39,  31,  23,  15, 7}
};

//明文初始置换表

int R_E[8][6] = {
						{32, 1,  2,  3,  4,  5},
						{4,  5,  6,  7,  8,  9},
						{8,  9,  10, 11, 12, 13},
						{12, 13, 14, 15, 16, 17},
						{16, 17, 18, 19, 20, 21},
						{20, 21, 22, 23, 24, 25},
						{24, 25, 26, 27, 28, 29},
						{28, 29, 30, 31, 32, 1}
};

//表6-7 S盒 用来将Rn与子密钥异或的结果（48位）转换为32位
int S1[] = { 14,  4, 13,  1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7,
			 0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8,
			 4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0,
			15, 12,  8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6, 13 };

int S2[] = { 15,  1,  8, 14,  6, 11,  3,  4,  9,  7,  2, 13, 12,  0,  5, 10,
			 3, 13,  4,  7, 15,  2,  8, 14, 12,  0,  1, 10,  6,  9, 11,  5,
			 0, 14,  7, 11, 10,  4, 13,  1,  5,  8, 12,  6,  9,  3,  2, 15,
			13,  8, 10,  1,  3, 15,  4,  2, 11,  6,  7, 12,  0,  5, 14,  9 };

int S3[] = { 10,  0,  9, 14,  6,  3, 15,  5,  1, 13, 12,  7, 11,  4,  2,  8,
			13,  7,  0,  9,  3,  4,  6, 10,  2,  8,  5, 14, 12, 11, 15,  1,
			13,  6,  4,  9,  8, 15,  3,  0, 11,  1,  2, 12,  5, 10, 14,  7,
			 1, 10, 13,  0,  6,  9,  8,  7,  4, 15, 14,  3, 11,  5,  2, 12 };

int S4[] = { 7, 13, 14,  3,  0,  6,  9, 10,  1,  2,  8,  5, 11, 12,  4, 15,
			13,  8, 11,  5,  6, 15,  0,  3,  4,  7,  2, 12,  1, 10, 14,  9,
			10,  6,  9,  0, 12, 11,  7, 13, 15,  1,  3, 14,  5,  2,  8,  4,
			 3, 15,  0,  6, 10,  1, 13,  8,  9,  4,  5, 11, 12,  7,  2, 14 };

int S5[] = { 2, 12,  4,  1,  7, 10, 11,  6,  8,  5,  3, 15, 13,  0, 14,  9,
			14, 11,  2, 12,  4,  7, 13,  1,  5,  0, 15, 10,  3,  9,  8,  6,
			 4,  2,  1, 11, 10, 13,  7,  8, 15,  9, 12,  5,  6,  3,  0, 14,
			11,  8, 12,  7,  1, 14,  2, 13,  6, 15,  0,  9, 10,  4,  5,  3 };

int S6[] = { 12,  1, 10, 15,  9,  2,  6,  8,  0, 13,  3,  4, 14,  7,  5, 11,
			10, 15,  4,  2,  7, 12,  9,  5,  6,  1, 13, 14,  0, 11,  3,  8,
			 9, 14, 15,  5,  2,  8, 12,  3,  7,  0,  4, 10,  1, 13, 11,  6,
			 4,  3,  2, 12,  9,  5, 15, 10, 11, 14,  1,  7,  6,  0,  8, 13 };

int S7[] = { 4, 11,  2, 14, 15,  0,  8, 13,  3, 12,  9,  7,  5, 10,  6,  1,
			13,  0, 11,  7,  4,  9,  1, 10, 14,  3,  5, 12,  2, 15,  8,  6,
			 1,  4, 11, 13, 12,  3,  7, 14, 10, 15,  6,  8,  0,  5,  9,  2,
			 6, 11, 13,  8,  1,  4, 10,  7,  9,  5,  0, 15, 14,  2,  3, 12 };

int S8[] = { 13,  2,  8,  4,  6, 15, 11,  1, 10,  9,  3, 14,  5,  0, 12,  7,
			 1, 15, 13,  8, 10,  3,  7,  4, 12,  5,  6, 11,  0, 14,  9,  2,
			 7, 11,  4,  1,  9, 12, 14,  2,  0,  6, 10, 13, 15,  3,  5,  8,
			 2,  1, 14,  7,  4, 10,  8, 13, 15, 12,  9,  0,  3,  5,  6, 11 };

//表6-8 P盒置换表，用来对S盒替换后产生的32位结果进行置换
int P_replace[8][4] = {
						{16,  7,   20,  21},
						{29,  12,  28,  17},
						{1,   15,  23,  26},
						{5,   18,  31,  10},
						{2,   8,   24,  14},
						{32,  27,  3,   9},
						{19,  13,  30,  6},
						{22,  11,  4,  25}
};

//IP逆置换表
int re_IP[8][8] = {
						{40,  8, 48, 16, 56, 24, 64, 32},
						{39,  7, 47, 15, 55, 23, 63, 31},
						{38,  6, 46, 14, 54, 22, 62, 30},
						{37,  5, 45, 13, 53, 21, 61, 29},
				{36,  4, 44, 12, 52, 20, 60, 28},
						{35,  3, 43, 11, 51, 19, 59, 27},
						{34,  2, 42, 10, 50, 18, 58, 26},
						{33,  1, 41,  9, 49, 17, 57, 25}
};

//将数存入矩阵
void num_to_matrix(long long key, int key_matrix[8][8]) {
	int tem[64] = { 0 };//中间存储
	int i = 0;
	for (i = 0; i < 64; i++) {
		tem[64 - 1 - i] = key % 2;//首先将小数存储在尾端
		key /= 2;
	}
	for (int m = 0; m < 8; m++) {
		for (int n = 0; n < 8; n++) {
			key_matrix[m][n] = tem[m * 8 + n];//将中间数组的值存入矩阵
		}
	}

}

void matrix_to_two(char mode)//将矩阵分开
{
	int i = 0, j = 0;
	if (mode == 'k') {       //将56位密钥8*7拆成4*7==28两部分
		for (int m = 0; m < 4; m++) {
			for (int n = 0; n < 7; n++) {
				C[0][i] = key_matrix_plus[m][n];
				i++;
			}
		}
		for (int m = 4; m < 8; m++) {
			for (int n = 0; n < 7; n++) {
				D[0][j] = key_matrix_plus[m][n];
				j++;
			}
		}

	}
	else {
		int m_plus = 0;
		for (int m = 0; m < 4; m++) {
			int n_plus = 0;
			for (int n = 0; n < 4; n++) {
				L[0][m_plus][n_plus] = m_set_plus[m][n];
				n_plus++;
			}
			n_plus = 0;
			for (int m = 4; m < 8; m++) {
				int n_plus = 0;
				for (int n = 0; n < 4; n++) {
					R[0][m_plus][n_plus] = m_set_plus[m][n];
					n_plus++;
				}
				n_plus = 0;
				m_plus++;
				for (int n = 4; n < 8; n++) {
					R[0][m_plus][n_plus] = m_set_plus[m][n];
					n_plus++;
				}
				m_plus++;
			}
		}
	}
}


//实现循环左移
int* left_shift(int a[28], int num) {
	int* b = (int*)malloc(28 * sizeof(int));//用来接受循环移位的数组
	for (int i = 0; i < 28 - num; i++) {
		b[i] = a[i + num];
	}
	int j = 0;
	for (int i = 28 - num; i < 28; i++) {
		b[i] = a[j];
		j++;
	}
	return b;
}


//将移位后的两部分C和D合并成一个子密钥8*7 =56
void CD_K(int* a, int* b, int k[8][7]) {



	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 7; j++) {
			if (i < 4) {
				k[i][j] = a[i * 7 + j];
			}
			else {
				k[i][j] = b[(i - 4) * 7 + j];
			}
		}
	}
}
//将64位密钥置换成 56位
void key_replace() {
	int index = 0;
	for (int m = 0; m < 8; m++) {
		for (int n = 0; n < 7; n++) {
			index = perm_key[m][n];
			key_matrix_plus[m][n] = key_matrix[(index - 1) / 8][(index - 1) % 8];
		}
	}
}


void Kn_create() {
	for (int i = 0; i < 16; i++) {
		int index = num_shift[i];//num_shift每轮移动的位数
		int* cindex = left_shift(C[i], index);//将左密钥进行移动
		int* dindex = left_shift(D[i], index);//右密钥进行移动
		//将移位以后的cd合并
		for (int m = 0; m < 28; m++) {
			C[i + 1][m] = cindex[m];//放到下一轮的密钥中
			D[i + 1][m] = dindex[m];
		}
		CD_K(C[i + 1], D[i + 1], K[i + 1]);
		//用于存储左明文和右明文
		index = 0;
		for (int m = 0; m < 8; m++) {
			for (int n = 0; n < 6; n++) {
				index = perm_K[m][n];//选出最终的48位
				K_plus[i + 1][m][n] = K[i + 1][(index - 1) / 7][(index - 1) % 7];

			}
		}
	}
}


//明文进行置换
void M_replace() {
	int index = 0;
	for (int m = 0; m < 8; m++) {
		for (int n = 0; n < 8; n++) {
			index = IP[m][n];
			m_set_plus[m][n] = m_set[(index - 1) / 8][(index - 1) % 8];//每行8个元素，对列数进行处理
		}
	}

}


//将二进制矩阵变成数字
long long result(int arr[8][8])
{
	long long result = 0;
	int i, j;
	long long temp = 1;
	for (i = 7; i >= 0; i--)
	{
		for (j = 7; j >= 0; j--)
		{
			result += temp * arr[i][j];
			temp *= 2;

		}

	}
	return result;
}



int main()
{
	//将密钥存到64位的密钥矩阵中
	num_to_matrix(Key, key_matrix);
	//进行密钥置换
	key_replace();
	matrix_to_two('k');
	Kn_create();//创造16轮的密钥

	//打印子密钥
	for (int i = 1; i < 17; i++) {
		printf("k%d：", i);
		for (int j = 0; j < 8; j++) {
			for (int k = 0; k < 6; k++) {
				printf("%d", K_plus[i][j][k]);
			}
			printf(" ");
		}
		printf(" \n");
	}

	//将明文变为矩阵m_set存到矩阵
	num_to_matrix(M, m_set);

	//将明文进行初始的ip置换
	M_replace();
	matrix_to_two('M');
	//分成L0和R0


	for (int x = 0; x < 16; x++) {
		int index = 0;
		for (int m = 0; m < 8; m++) {
			for (int n = 0; n < 6; n++) {
				index = R_E[m][n];
				R_plus[x][m][n] = R[x][(index - 1) / 4][(index - 1) % 4];//在谁里面取数就用谁的列标
			}
		}
		for (int m = 0; m < 8; m++) {
			for (int n = 0; n < 6; n++)
			{
				R_plus[x][m][n] = K_plus[x + 1][m][n] ^ R_plus[x][m][n];
			}

		}

		for (int m = 0; m < 8; m++)
		{
			int tmp = 0;
			int row = R_plus[x][m][0] * 2 + R_plus[x][m][5] * 1;
			int col = R_plus[x][m][4] * 1 + R_plus[x][m][3] * 2 + R_plus[x][m][2] * 4 + R_plus[x][m][1] * 8;
			switch (m) {
			case 0:
				tmp = S1[row * 16 + col];
				break;
			case 1:
				tmp = S2[row * 16 + col];
				break;
			case 2:
				tmp = S3[row * 16 + col];
				break;
			case 3:
				tmp = S4[row * 16 + col];
				break;
			case 4:
				tmp = S5[row * 16 + col];
				break;
			case 5:
				tmp = S6[row * 16 + col];
				break;
			case 6:
				tmp = S7[row * 16 + col];
				break;
			default:
				tmp = S8[row * 16 + col];
				break;
			}
			for (int n = 0; n < 4; n++) {
				R_plus[x][m][3 - n] = tmp % 2;
				R_plus_temp[x][m][3 - n] = tmp % 2;
				tmp /= 2;
			}
		}






		//s盒替代48位

		//P盒置换
		index = 0;
		for (int m = 0; m < 8; m++) {
			for (int n = 0; n < 4; n++) {
				index = P_replace[m][n];
				R_plus[x][m][n] = R_plus_temp[x][(index - 1) / 4][(index - 1) % 4];
			}
		}
		//与ln异或
		for (int m = 0; m < 8; m++) {
			for (int n = 0; n < 4; n++) {
				R_plus[x][m][n] = L[x][m][n] ^ R_plus[x][m][n];
			}
		}
		//Rn+1=R_plus,Ln+1 = Rn
		for (int m = 0; m < 8; m++) {
			for (int n = 0; n < 4; n++) {
				R[x + 1][m][n] = R_plus[x][m][n];
			}
		}
		for (int m = 0; m < 8; m++) {
			for (int n = 0; n < 4; n++) {
				L[x + 1][m][n] = R[x][m][n];
			}
		}

	}

	//最后一轮，不需要交换L与R，所以得再转过来
	int i = 16;
	int tj = 0;
	for (int j = 0; j < 4; j++) {
		for (int k = 0; k < 8; k++) {
			if (k < 4) {
				LR_plus[i][j][k] = R[i][tj][k];
			}
			else {
				LR_plus[i][j][k] = R[i][tj + 1][k - 4];
			}
		}
		tj += 2;
	}
	tj = 0;
	for (int j = 4; j < 8; j++) {
		for (int k = 0; k < 8; k++) {
			if (k < 4) {
				LR_plus[i][j][k] = L[i][tj][k];
			}
			else {
				LR_plus[i][j][k] = L[i][tj + 1][k - 4];
			}
		}
		tj += 2;
	}


	//printf("%d",LR_plus);


	//末置转换,对合并后的密文再进行末置换
	i = 16;
	for (int m = 0; m < 8; m++) {
		for (int n = 0; n < 8; n++) {
			LR_plus_temp[i][m][n] = LR_plus[i][m][n];
		}
	}
	int index = 0;
	for (int m = 0; m < 8; m++) {
		for (int n = 0; n < 8; n++) {
			index = re_IP[m][n];
			LR_plus[i][m][n] = LR_plus_temp[i][(index - 1) / 8][(index - 1) % 8];
		}
	}


	//转为16进制
	long long a = result(LR_plus[16]);
	//printf("%d",a);
	printf("%+016llX \n", a);
	return 0;



}
