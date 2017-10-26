#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <cstring>
#include <iomanip>

int model_N = 0;
int n=0;
char model_name[40][100];
double pi[40][100];
double a[40][100][100];
double b[40][100][100];
double delta_final[100];
double delta[100][100];

void init_input_model(FILE *model_list){
	while(fscanf(model_list,"%s",model_name[model_N])!=EOF){
		FILE *t_init_model = fopen(model_name,"r");
		char temp[1000];
		fscanf(t_init_model,"%s%d",temp,&n);
		for(int g=0;g<n;g++) fscanf(t_init_model,"%lf",&pi[model_N][g]);
		fscanf(t_init_model,"%s%d",temp,&n);
		for(int g=0;g<n;g++){
			for(int h=0;h<n;h++){
				fscanf(t_init_model,"%lf",&a[model_N][g][h]);
			}
		}
		fscanf(t_init_model,"%s%d",temp,&n);
		for(int g=0;g<n;g++){
			for(int h=0;h<n;h++){
				fscanf(t_init_model,"%lf",&b[model_N][h][g]);
			}
		}
		model_N++;
	}
	return;
}

double calculate_delta(int *o, int T, int model){
	for(int i=0;i<n;i++){
		delta[0][i] = pi[model][i] * b[model][i][o[0]];
	}
	for(int t=1;t<T;j++){
		for(int j=0;j<n;j++){
			double maxi = 0.0;
			for(int i=0;i<n;i++){
				double temp = delta[t-1][i] * a[model][i][j];
				maxi = (maxi>=temp)?maxi:temp;
			}
			delta[t][j] = maxi * b[model][j][o[t]];
		}
	}
	double maxii = 0.0
	for(int i=0;i<n;i++){
		maxii = (maxii>=delta[T-1][i])?maxii:delta[T-1][i];
	}
	return maxii;
}

void testing(int *o, int T, int *ans, double *ans_prob){
	
}

int main(int argc, char **argv){
	if(argc != 4){
		cerr<<"Error: Insufficient Parameters."<<endl;
	}
	FILE *model_list = fopen(argv[2],"r");
	FILE *testing_data = fopen(argv[3], "r");
	FILE *result = fopen(argv[4], "r");

}