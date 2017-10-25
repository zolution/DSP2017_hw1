#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <cstring>
#include <iomanip>

using namespace std;
FILE *t_data_input;
FILE *t_init_model;
double pi[1000];
double a[1000][1000];
double b[1000][1000];
double alpha[1000][1000];
double beta[1000][1000];
double eps[1000][1000];
double gamma[1000][1000];
int n;

void init(){
	for(int i=0;i<1000;i++){
		for(int j=0;j<1000;j++){
			eps[i][j] = 0;
			gamma[i][j] = 0;
		}
	}
	return;
}
void input_init_model(){
	char temp[1000];
	fscanf(t_init_model,"%s%d",temp,&n);
	for(int g=0;g<n;g++) fscanf(t_init_model,"%lf",&pi[g]);
	fscanf(t_init_model,"%s%d",temp,&n);
	for(int g=0;g<n;g++){
		for(int h=0;h<n;h++){
			fscanf(t_init_model,"%lf",&a[g][h]);
		}
	}
	fscanf(t_init_model,"%s%d",temp,&n);
	for(int g=0;g<n;g++){
		for(int h=0;h<n;h++){
			fscanf(t_init_model,"%lf",&b[g][h]);
		}
	}
	return;
}

void calculate_alpha(int *o, int T){
	for(int i=0;i<n;i++){
		alpha[0][i] = pi[i] * b[i][o[i]];
	}
	for(int t=1;t<T;t++){
		for(int j=0;j<n;j++){
			double sum = 0.0;
			for(int i=0;i<n;i++){
				sum += (alpha[t-1][i] * a[i][j]);
			}
			alpha[t][j] = sum * b[j][o[t]];
		}
	}
	return;
}

void calculate_beta(int *o, int T){
	for(int i=0;i<n;i++){
		beta[T-1][i] = 1.0;
	}
	for(int t=T-2;t>=0;t--){
		for(int i=0;i<n;i++){
			double sum = 0.0;
			for(int j=0;j<n;j++){
				sum += (a[i][j] * b[j][o[t+1]] * beta[t+1][j]);
			}
			beta[t][i] = sum;
		}
	}
	return;
}

void iteration(char *o){
	int obs[1000];
	int len = strlen(o);
	for(int g=0;g<len;g++) obs[g] = o[g]='A';
	calculate_alpha(obs, len);
	calculate_beta(obs, len);


}

int main(int argc, char **argv){
	if(argc<4){
		cerr<<"Error: Insufficient Parameters."<<endl;
	}
	t_init_model = fopen(argv[1],"r");
	t_data_input = fopen(argv[2],"r");
	init();
	input_init_model();

}