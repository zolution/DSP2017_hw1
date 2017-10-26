#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <cstring>
#include <iomanip>

#define max_operation_class 6
using namespace std;
FILE *t_data_input;
FILE *t_init_model;
char observations[10005][55];
int trans_observations[10005][55];
double pi[1000];
double a[1000][1000];
double b[1000][1000];
double alpha[1000][1000];
double beta[1000][1000];
double eps[1000][1000];
double gam[1000];
double gam_full[1000];
double gam_bucket[1000][1000];
double temp_pi[1000];
int n;

void init(){
	for(int i=0;i<1000;i++){
		gam[i] = 0;
		gam_full[i] = 0;
		for(int j=0;j<1000;j++){
			eps[i][j] = 0;
			gam_bucket[i][j] = 0;
			
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
			fscanf(t_init_model,"%lf",&b[h][g]);
		}
	}
	return;
}

void calculate_alpha(int *o, int T){
	for(int i=0;i<n;i++){
		alpha[0][i] = pi[i] * b[i][o[0]];
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

void calculate_eps(int *o, int T){
	double local_eps[1000][1000];
	for(int t=0;t<T-1;t++){
		double total = 0.0;
		for(int i=0;i<n;i++){
			for(int j=0;j<n;j++){
				local_eps[i][j] = alpha[t][i] * a[i][j] * b[j][o[t+1]] * beta[t+1][j];
				total += local_eps[i][j];
			}
		}
		for(int i=0;i<n;i++){
			for(int j=0;j<n;j++){
				eps[i][j] += (local_eps[i][j] / total);
			}
		}
	}
	return;
}

void calculate_gam(int *o, int T){
	double local_gam[1000];
	for(int t=0;t<T;t++){
		double total = 0.0;
		for(int i=0;i<n;i++){
			local_gam[i] = alpha[t][i] * beta[t][i];
			total += local_gam[i];
		}
		for(int i=0;i<n;i++){
			local_gam[i] /= total;
			gam[i] += local_gam[i];
			gam_full[i] += local_gam[i];
			gam_bucket[i][o[t]] += local_gam[i];
			if(t==0) temp_pi[i] += local_gam[i];
		}
	}
	for(int i=0;i<n;i++) gam[i] -= local_gam[i];
	return;
}

void update_a(){
	for(int i=0;i<n;i++){
		for(int j=0;j<n;j++){
			a[i][j] = eps[i][j] / gam[i];
		}
	}
	return;
}

void update_b(){
	for(int j=0;j<n;j++){
		for(int k=0; k<max_operation_class; k++){
			b[j][k] = gam_bucket[j][k] / gam_full[j];
		}
	}
	return;
}

void update_pi(int num){
	for(int h=0;h<n;h++){
		pi[h] = temp_pi[h] / (double)num;
	}
	memset(temp_pi,0,sizeof(temp_pi));
	return;
}


int read_observations(){
	int ptr = 0;
	while(fscanf(t_data_input,"%s",observations[ptr]) != EOF){
		int len = strlen(observations[ptr]);
		for(int h=0;h<len;h++){
			trans_observations[ptr][h] = observations[ptr][h] - 'A';
		}
		ptr++;
	}
	return ptr;
}

void iteration(int num){
	init();
	int len = strlen(observations[0]);
	for(int i=0;i<num;i++){
		calculate_alpha(trans_observations[i], len);
		calculate_beta(trans_observations[i], len);
		calculate_eps(trans_observations[i], len);
		calculate_gam(trans_observations[i], len);
	}
	update_a();
	update_b();
	update_pi(num);
}

void output_model(){
	fprintf(stdout, "initial: %d\n", n);
	for(int h=0;h<n;h++){
		fprintf(stdout, "%.5f%c", pi[h], h==n-1?'\n':' ');
	}
	fprintf(stdout, "\ntransition: %d\n", n);
	for(int h=0;h<n;h++){
		for(int i=0;i<n;i++){
			fprintf(stdout, "%.5f%c", a[h][i], i==n-1?'\n':' ');
		}
	}
	fprintf(stdout, "\nobservation: %d\n", n);
	for(int h=0;h<n;h++){
		for(int i=0;i<max_operation_class;i++){
			fprintf(stdout, "%.5f%c", b[i][h], i==max_operation_class-1?'\n':' ');
		}
	}
	return;
}

int main(int argc, char **argv){
	if(argc!=5){
		cerr<<"Error: Insufficient Parameters."<<endl;
	}
	int iteration_n = atoi(argv[1]);
	t_init_model = fopen(argv[2],"r");
	t_data_input = fopen(argv[3],"r");
	init();
	input_init_model();
	int num = read_observations();
	for(int i=0;i<iteration_n;i++) iteration(num);
	output_model();
	return 0;

}