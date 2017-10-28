#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <iomanip>

using namespace std;

int main(int argc, char **argv){
	if(argc != 4){
		cerr<<"Argument Error"<<endl;
		return 1;
	}
	FILE *answer = fopen(argv[1],"r");
	FILE *result = fopen(argv[2], "r");
	FILE *acc = fopen(argv[3], "w");
	char ans[100],res[100],temp[100];
	int total_count = 0;
	int g = 0;
	while(fscanf(answer,"%s",ans)!=EOF && fscanf(result,"%s%s",res,temp)!=EOF){
		total_count++;
		if(strcmp(ans,res) == 0) g++;
	}
	fprintf(acc, "%.4f\n", (double)g/(double)total_count);
}