#include<string>
#include<iostream>

#define MAX 1001

using namespace std;

int c[MAX][MAX];
char b[MAX][MAX];

void LCS_length(string X, string Y){

    int m = X.size();
    int n = Y.size();
    for(int i = 1; i <= m; i++ ){
        for(int j = 1; j <= n; j++){
            if(X[i-1] == Y[j-1]){
                c[i][j] = (c[i-1][j-1] + 1);
                b[i][j] = 'u'; //U+2196
            }
            else if(c[i-1][j] >= c[i][j-1]){
                c[i][j] = c[i-1][j];
                b[i][j] = 'g'; //U+2191
            }
            else{
                c[i][j] = c[i][j-1];
                b[i][j] = 'l'; //U+2190
            }
        }
    }
}

void LCS_print(string X, int i, int j){
    if(i == 0 || j == 0) return;
    if(b[i][j] == 'u'){
        LCS_print(X,i-1,j-1);
        cout<<X[i-1];
    }
    else if (b[i][j] == 'g')
        LCS_print(X,i-1,j);
    else LCS_print(X,i,j-1);
}
int main(){

	string str1 = "siakalaka";
	string str2 = "aaa";
    int m = str1.size();
    int n = str2.size();
    LCS_length(str1,str2);
    for(int i=0; i<=m; i++){
        for(int j=0; j<=n; j++)
            cout << c[i][j] << " ";
        cout<<endl;
    }
    cout<<c[m][n]<<endl;
    LCS_print(str1,m,n);
	return 0;
}
