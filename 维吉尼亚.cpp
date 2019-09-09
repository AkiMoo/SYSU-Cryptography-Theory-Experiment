#include <iostream>
#include <fstream>
#include <string.h>
#include <stdlib.h>
using namespace std;
void crack(char c[]) 
{
    int keylen=1;   //密钥长度
    int cryptolen=strlen(c);   //密文的长度 		     	
    while(1)
    {
        float IC[keylen]; //重合指数
	    float avgIC=0;  //平均重合指数
	    for(int i=0;i<keylen;i++)    //统计分组字母个数 
    	{		
	        int out[26]={ 0 };   //盛放字母个数的数组
		    for(int j=0;i+j*keylen<cryptolen;j++){
                out[(int)(c[i+j*keylen]-'A')]++;
            }
            float e=0.000f;
            int L=0;
            for(int k=0;k<26;k++){    //子串密文长度 
                L+=out[k];
            }
            L*=(L-1);
	        for(int k=0;k<26;k++){        //分组计算重合指数IC 
	            if(out[k]!=0)
	            e=e+((float)out[k]*(float)(out[k]-1))/(float)L;
            }
	    	IC[i]=e;
        }
	    for(int i=0;i<keylen;i++){
            avgIC+=IC[i];
        }

    	avgIC/=keylen;          //求IC的平均值  
        if (avgIC >= 0.06)  break;    //判断退出条件，重合指数的平均值是否大于0.06 
        else  keylen++;
    }
    cout<<"Length of Key = "<<keylen<<endl; 
    float p[] = {0.082, 0.015, 0.028, 0.043, 0.127, 0.022, 0.02, 0.061, 0.07, 
    0.002, 0.008, 0.04, 0.024, 0.067, 0.075, 0.019, 0.001, 0.06, 0.063, 0.091, 
    0.028, 0.01, 0.023, 0.001, 0.02, 0.001};//字母频率
	
	int key[100];    //存放密钥 
    memset(key,0,sizeof(key));
	for(int i=0;i<keylen;i++)    //统计分组字母个数 
	{	
	    int g=0;   //密文移动g个位置 
	    for(int t=0;t<26;t++)
	    {
	 	    float x=0.000f;    //拟重合指数 	
	        int out[26]={ 0 };   //盛放字母个数的数组
		    for(int j=0;i+j*keylen<cryptolen;j++)
		       out[(int)(c[i+j*keylen]-'A')]++;  
            int L=0;
            for(int k=0;k<26;k++)      //子串密文长度 
                L+=out[k];
            for(int k=0;k<26;k++)
                x=x+p[k]*out[(k+g)%26];
            if(x/L>0.055)
            {
            	key[i]=g;
            	break;
		    }
            else g++;
	    }
    }
    cout<<"The keys are:"; 
    for(int i=0;i<keylen;i++)
    {
        cout<<char ('a'+key[i]);
        //cout << key [i]<<endl;
    }
    cout<<endl;
    int k = 0;
    int support = 0;
    for(int i = 0;i<cryptolen;i++)
    {
        support = c[i]-65-key[k];
        if(support < 0)
        {
            support += 26;
        }
        cout << char ('a'+support);
        k++;
        if(k==6){
            k=0;
        }
    }
    cout<<endl;
}
int main(){
    ifstream readFile("Plain.txt");
    char plains[1000];
    memset(plains,0,sizeof(plains));
    readFile >> plains;
    readFile.close();
    crack(plains);
    return 0;
}