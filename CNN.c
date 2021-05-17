#include<stdio.h>
#include<obliv.h>
#include<string.h>
#include<math.h>
#include "CNN.h"
#include<sys/stat.h>
#include<sys/types.h>

//define File-Folder "model/";

char* model_file_folder = "model/";

//Open file in Folder "model"
FILE* Open_File(char* file_name) {
	FILE* fp;
	char str[200];
	strcpy(str, model_file_folder);
	strcat(str, file_name);
	fp = fopen(str, "r");
	if (fp == NULL) {
		exit(0);
	}
	return fp;

}


int main(int argc, char* argv[])
{
	ProtocolDesc pd;
	protocolIO io;

	protocolUseStdio(&pd);
	int i, j, k, l; 	//params used for "for" codes
	FILE* fp;	//param used for opening files

	if (strcmp(argv[1], "user") == 0)
	{
		setCurrentParty(&pd, 1);
		char* input_file_name = argv[2]; //用户输入的文件名
		fp = fopen(input_file_name, "r");
		if (fp == NULL)
		{
			exit(0);
		}
		else
		{
			fscanf(fp, "%f", &io.answer);			//读取答案
			for (i = 0; i < 28; i++)
			{
				for (j = 0; j < 28; j++)
				{
					fscanf(fp, "%f", &io.input[i][j]);
				}
			}
		}

		const char* remote_host = (strcmp(argv[3], "--") ? argv[3] : NULL);  //获取主机号

		//建立连接
		fprintf(stderr, "data load\n\n");
		if (protocolConnectTcp2P(&pd, remote_host, argv[4]) != 0) {
			fprintf(stderr, "TCP connect failed\n");
			exit(1);
		}
		fprintf(stderr, "TCP accept\n\n");
	}
	else if (strcmp(argv[1], "model") == 0)
	{
		setCurrentParty(&pd, 2);
		model_file_folder = argv[2];

		//BinarizeConv2d(0)
			//BConv1_weight[6][5][5]
			//从1_features_0_weight.txt输入
		fp = Open_File("1_features_0_weight.txt");
		for (i = 0; i < 6; i++)
		{
			for (j = 0; j < 5; j++)
			{
				for (k = 0; k < 5; k++)
				{
					fscanf(fp, "%f", &io.BConv1_weight[i][j][k]);
				}
			}
		}

		//BConv1_bias[6]	
		//从2_features_0_bias.txt输入
		fp = Open_File("2_features_0_bias.txt");
		for (i = 0; i < 6; i++)
		{
			fscanf(fp, "%f", &io.BConv1_bias[i]);
		}
		
		//BinarizeConv2d(4)
			//BConv2_weight[96][5][5];
			//从3_features_4_weight.txt输入
		fp = Open_File("3_features_4_weight.txt");
		for (i = 0; i < 96; i++)
		{
			for (j = 0; j < 5; j++)
			{
				for (k = 0; k < 5; k++)
				{
					fscanf(fp, "%f", &io.BConv2_weight[i][j][k]);
				}
			}
		}

		//float BConv2_bias[16];
		//从4_features_4_bias.txt输入
		fp = Open_File("4_features_4_bias.txt");
		for (i = 0; i < 16; i++)
		{
			fscanf(fp, "%f", &io.BConv2_bias[i]);
		}
		
		
		
		//BinarizeLinear(0)
			//float BL_weight[120][400];
			//从10_classifier_0_weight.txt输入
		fp = Open_File("10_classifier_0_weight.txt");
		for (i = 0; i < 120; i++)
		{
			for (j = 0; j < 400; j++)
			{

				fscanf(fp, "%f", &io.BL_weight[i][j]);

			}

		}

		//float BL_bias[120];
		//从11_classifier_0_bias.txt输入
		fp = Open_File("11_classifier_0_bias.txt");
		for (i = 0; i < 120; i++)
		{
			fscanf(fp, "%f", &io.BL_bias[i]);
		}
		
		//Linear(1)
			//float Linear_weight[84][120];
			//从12_classifier_1_weight.txt输入
		fp = Open_File("12_classifier_1_weight.txt");
		for (i = 0; i < 84; i++)
		{
			for (j = 0; j < 120; j++)
			{
				fscanf(fp, "%f", &io.Linear1_weight[i][j]);
			}
		}

		//float Linear_bias[84];
		//从13_classifier_1_bias.txt输入
		fp = Open_File("13_classifier_1_bias.txt");
		for (i = 0; i < 84; i++)
		{
			fscanf(fp, "%f", &io.Linear1_bias[i]);
		}
		
		//Linear(2)
			//float Linear2_weight[10][84];
			//从14_classifier_2_weight.txt输入
		fp = Open_File("14_classifier_2_weight.txt");
		for (i = 0; i < 10; i++)
		{
			for (j = 0; j < 84; j++)
			{
				fscanf(fp, "%f", &io.Linear2_weight[i][j]);
			}
		}

		//float Linear2_bias[10];
		//从15_classifier_2_bias.txt输入
		fp = Open_File("15_classifier_2_bias.txt");
		for (i = 0; i < 10; i++)
		{
			fscanf(fp, "%f", &io.Linear2_bias[i]);
		}

		//建立连接
		fprintf(stderr, "model load\n\n");

		if (protocolAcceptTcp2P(&pd, argv[4]) != 0) {
			fprintf(stderr, "TCP accept failed\n");
			exit(1);
		}
		fprintf(stderr, "TCP connect\n\n");
	}
	fprintf(stderr, "Yeah!\n");


	execYaoProtocol(&pd, nn, &io);
	cleanupProtocol(&pd);
	//fprintf(stderr, "%d\n", io.result);
	/*for (i = 0; i < 84; i++)
	{

		fprintf(stderr, "%f ", io.Check[i]);
	}*/

	fprintf(stderr, "%s \nstart:\t%lds\t%ldus\nend:\t%lds\t%ldus\t\ntimeuse:\t%lds\t%ldus\nResult:\t%d\nAnswer:\t%f\n", argv[1], io.start_s, io.start_us, io.end_s, io.end_us, io.timeuse_s, io.timeuse_us, io.result, io.answer);
	//fprintf(stderr, "%s \nstart:\t%lds\t%ldus\nend:\t%lds\t%ldus\t\ntimeuse:\t%lds\t%ldus\n", argv[1], io.start_s, io.start_us, io.end_s, io.end_us, io.timeuse_s, io.timeuse_us);

	return 0;
}

