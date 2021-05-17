typedef struct protocolIO
{
	//user input
	float input[28][28];				//从文件输入
	float answer;						//从文件输入

	//model params
	//BinarizeConv2d(0)
	float BConv1_weight[6][5][5];		//从1_features_0_weight.txt输入
	float BConv1_bias[6];				//从2_features_0_bias.txt输入
	//BinarizeConv2d(4)
	float BConv2_weight[96][5][5];		//从3_features_4_weight.txt输入
	float BConv2_bias[16];				//从4_features_4_bias.txt输入
	
	
	//BinarizeLinear(0)
	float BL_weight[120][400];			//从10_classifier_0_weight.txt输入
	float BL_bias[120];					//从11_classifier_0_bias.txt输入
	//Linear(1)
	float Linear1_weight[84][120];		//从12_classifier_1_weight.txt输入
	float Linear1_bias[84];				//从13_classifier_1_bias.txt输入
	//Linear(2)
	float Linear2_weight[10][84];		//从14_classifier_2_weight.txt输入
	float Linear2_bias[10];				//从14_classifier_2_bias.txt输入

	//output
	int result;
	long start_s;
	long start_us;
	long end_s;
	long end_us;
	long timeuse_s;
	long timeuse_us;
	float Check[120];

} protocolIO;

void nn(void* args);
