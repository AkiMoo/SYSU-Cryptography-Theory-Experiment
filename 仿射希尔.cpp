# include <iostream>
# include <fstream>
# include <string.h>
using namespace std;
#define N 3
//按第一行展开计算|A|
int getA(int arcs[N][N],int n){
    if(n==1){
        return arcs[0][0];
    }
    int ans = 0;
    int temp[N][N]={0};
    int i,j,k;
    for(i=0;i<n;i++){
        for(j=0;j<n-1;j++){
            for(k=0;k<n-1;k++){
                temp[j][k] = arcs[j+1][(k>=i)?k+1:k];

            }
        }
        int t = getA(temp,n-1);
        if(i%2==0){
            ans += arcs[0][i]*t;
        }
        else{
            ans -= arcs[0][i]*t;
        }
    }
    return ans;
}

//计算每一行每一列的每个元素所对应的余子式，组成A*
void  getAStart(int arcs[N][N],int n,int ans[N][N]){
    if(n==1){
        ans[0][0] = 1;
        return;
    }
    int i,j,k,t;
    int temp[N][N];
    for(i=0;i<n;i++){
        for(j=0;j<n;j++){
            for(k=0;k<n-1;k++){
                for(t=0;t<n-1;t++){
                    temp[k][t] = arcs[k>=i?k+1:k][t>=j?t+1:t];
                }
            }
            ans[j][i]  =  getA(temp,n-1);
            if((i+j)%2 == 1){
                ans[j][i] = - ans[j][i];
            }
        }
    }
}
//得到给定矩阵src的逆矩阵保存到des中。
bool GetMatrixInverse(int src[N][N],int n,int des[N][N]){
    int flag=getA(src,n);
    int t[N][N];
    if(flag==0){
        return false;
    }
    else{
        getAStart(src,n,t);
        for(int i=0;i<n;i++){
            for(int j=0;j<n;j++)//逆矩阵求逆
            {
                des[i][j]=t[i][j];//这里先乘以21是因为-915mod26=21
                if(t[i][j]<0){
                    des[i][j] = 26 - 21*(-(t[i][j]))%26;
                }
                else{
                    des[i][j]=21*t[i][j]%26;
                }
            }
        }
    }
    return true;
}

int main(){
    //读入密文
    ifstream readFile1("Plain.txt");
    char plains[100];
    memset(plains,0,sizeof(plains));
    readFile1 >> plains;
    readFile1.close();
    //读入明文
    ifstream readFile2("Answer.txt");
    char answer[100];
    memset(answer,0,sizeof(answer));
    readFile2 >> answer;
    readFile2.close();

    int matrixA[N][N] = {0};
    int matrixB[N][N] = {0};
    int matrixC[N][N] = {0};
    int matrixD[N][N] = {0};
    int cnt = 0;
    for(int i = 0;i<3;i++){
        for(int k=0;k<3;k++){
            matrixA[i][k] = plains[cnt] - 'A';
            cnt ++;
        }
    }
    
    for(int i = 0;i<3;i++){
        for(int k=0;k<3;k++){
            matrixB[i][k] = plains[cnt] - 'A';
            cnt ++;
        }
    }

    cnt = 0;//从头开始数

    for(int i = 0;i<3;i++){
        for(int k=0;k<3;k++){
            matrixC[i][k] = answer[cnt] - 'a';
            cnt ++;
        }
    }

    for(int i = 0;i<3;i++){
        for(int k=0;k<3;k++){
            matrixD[i][k] = answer[cnt] -'a';
            cnt ++;
        }
    }

    int matrixE[N][N] = {0},matrixF[N][N] = {0};

    for(int i=0;i<3;i++){
        for(int k = 0;k<3;k++){
            if(matrixA[i][k] - matrixB[i][k]<0){
                matrixE[i][k] = matrixA[i][k] - matrixB[i][k]+26;
            }
            else{
                matrixE[i][k] = matrixA[i][k] - matrixB[i][k];
            }
        }
    }//密

    for(int i=0;i<3;i++){
        for(int k = 0;k<3;k++){
            if(matrixC[i][k] - matrixD[i][k]<0){
                matrixF[i][k] = matrixC[i][k] - matrixD[i][k]+26;
            }
            else{
                matrixF[i][k] = matrixC[i][k] - matrixD[i][k];
            }
        }
    }//明
    
    int matrixG[N][N] = {0},matrixanswer[N][N]={0};
    int support=0;
    GetMatrixInverse(matrixF,3,matrixG);
    cout<<"The result of the key matrix is:"<<endl;
    for(int m=0;m<3;m++){  
        for(int s=0;s<3;s++){  
            matrixanswer[m][s]=0;
            for(int n=0;n<3;n++){  
                matrixanswer[m][s]+=matrixG[m][n]*matrixE[n][s]; 
            }
            matrixanswer[m][s]=26 - matrixanswer[m][s]%26;
            cout<<matrixanswer[m][s]<<" ";
        }
        cout<<endl;
    }  
    
    int b1 = 0,b2=0,b3=0;
    for(int m=0;m<3;m++){  
        for(int s=0;s<3;s++){  
            matrixB[m][s]=0;
            for(int n=0;n<3;n++){  
                matrixB[m][s]+=matrixC[m][n]*matrixanswer[n][s]; 
            }  
        }  
    }  
    for(int i=0;i<3;i++){
        for(int j = 0;j<3;j++){
            if(matrixB[i][j]<0){
                matrixB[i][j] = 26 - (-(matrixB[i][j]))%26;
            }
            else{
                matrixB[i][j]=matrixB[i][j]%26;
            }
        }
    }
    b1 = matrixA[0][0]-matrixB[0][0];
    if(b1<0){
        b1 = 26-(-b1)%26;
    }
    b2 = matrixA[0][1]-matrixB[0][1];
    if(b2<0){
        b2 = 26-(-b2)%26;
    }
    b3 = matrixA[0][2]-matrixB[0][2];
    if(b3<0){
        b3 = 26-(-b3)%26;
    }
    cout<<"The result of B are:("<<b1<<","<<b2<<","<<b3<<")"<<endl;
}