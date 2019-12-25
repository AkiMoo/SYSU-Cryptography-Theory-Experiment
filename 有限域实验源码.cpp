#include <iostream>
#include <string.h>
#include <bitset>
#include <vector>
#include <string>
using namespace std;
#define Max 127
int binaryChange(long num);
int binary[100];
int binaryChange(long num){
	int cnt = 0;
	for (int i = 0;i < 100;i++){
		if (num != 1){
			if (num % 2 == 0){
				binary[i] = 0;
				num = num / 2;
				cnt++;
			}
			else{
				binary[i] = 1;
				num = num / 2;
				cnt++;
			}
		}
		else{
			binary[i] = 1;
			cnt++;
			break;
		}
	}
	return cnt;
}
int degree(const bitset<2 * Max> & a)//最高项次数
{
    for (int i = 2 * Max - 1; i >= 0; i--){
        if (a[i] || i == 0){
            return i;
        }
    }
}
bool judge(const bitset<2 * Max> &a)
{
    bitset<Max> temp;
    bitset<2 * Max> sup(temp.set().to_string() + temp.to_string());//set()把所有位都置一，to_string返回对应的字符串//
    sup &= a;
    return sup.any();//返回是否存在1的判断，有则ture无则false
}
bitset<Max> add(const bitset<Max> &bit1, const bitset<Max> &bit2)
{
    bitset<Max> answer = bit1 ^ bit2;//^是异或运算
    return answer;
}
bitset<Max> mod(const bitset<2 * Max> &target, const bitset<Max> &irr)
{
    bitset<2 * Max> x, y;
    bitset<2 * Max> exirr(irr.to_string());
    x = target;
    y ^= bitset<2 * Max>(x.to_string().substr(Max));
    while (judge(x))
    {
        y.reset();
        y ^= bitset<2 * Max>(x.to_string().substr(Max));//substr截取M位的字符串
        for (int i = Max; i < 2 * Max; i++){
            if (x[i]){
                y ^= exirr << (i - Max);
            }            
        }
        x = y;
    }
    return bitset<Max>(y.to_string().substr(Max));
}


bitset<Max> mul(const bitset<Max> &bit1, const bitset<Max> &bit2, const bitset<Max> &irr)
{
    bitset<2 * Max> x;
    bitset<2 * Max> temp(bit1.to_string());
    for (int i = 0; i < Max; i++)
    {
        if (bit2[i]){
            x ^= temp << i;
        }
    }
    return mod(x, irr);
}
bitset<Max> square(const bitset<Max> &a, const bitset<Max> &irr)
{
    bitset<2 * Max> x;
    for (int i = 0; i < Max; i++){
        x[i * 2] = a[i];
    }
    return mod(x, irr);
}
bitset<Max> inverse(const bitset<Max> &a, const bitset<Max + 1> &p)
{
    bitset<2 * Max> b, c, u, v, temp;
    bitset<Max> r(p.to_string().substr(1));
    int j = 0;
    b[0] = 1;
    u = bitset<2 * Max>(a.to_string());
    v = bitset<2 * Max>(p.to_string());
    while (degree(u)){
        j = degree(u) - degree(v);
        if (j < 0){
            j = -j;
            temp = u;
            u = v;
            v = temp;
            temp = b;
            b = c;
            c = temp;
        }
        u = u ^ (v << j);
        b = b ^ (c << j);
    }
    return mod(b, r);
}
int main()
{
    bitset<Max + 1> p("11");
    p[127] = 1;
    bitset<Max> r("11");
    bitset<Max> stuid("00010111001101000001000100100110");
    bitset<Max> cal = stuid;
    bitset<Max> temp("1");
    int count = binaryChange(20190911);
	for (int i = 0;i < count;i++) {
		if (binary[i] == 0){
			temp = temp;
			cal = square(cal,r);
		}
		else{
			temp = mul(temp,cal,r);
			cal = square(cal,r);	
		}
	}
    
    cout << "According to the request, change the student id as 4bits for each number:" << endl << stuid << endl << endl;
    cout << "Calculate student id add student id:" << endl << add(stuid,stuid) << endl << endl;
    cout << "Calculate student id's square:" << endl << square(stuid,r) << endl << endl;
    cout << "Calculate the inverse of student id:" << endl << inverse(stuid, p) << endl << endl;
    cout << "To check the answer of inverse, using student id multiply its inverse:" << endl << mul(inverse(stuid, p), stuid, r) << endl << endl;
    cout << "The \"17341126\" to the power of \"20190911\"is:" << endl;
    cout << temp << endl;
    system("pause");

    /*此处开始是时间测试代码：使用的时候把原来的main函数注释掉，把这部分显示出来就可以了。
    平方，乘法，求逆都做1000次，具体运算的步骤的和方法在txt文件中给出了。
    bitset<Max + 1> p("11");
    p[127] = 1;
    bitset<Max> r("11");
    bitset<Max> stuid("00010111001101000001000100100110");
    bitset<Max> cal = stuid;
    bitset<Max> temp("1");
    bitset<Max> ans("1101000000000010011011101100000110111000000110001001101000001101");这是用网上随机生成字符串的程序做的

    /*求逆部分开始
    ans = inverse(stuid,p);
    ans ^= temp;
    for(int i=0;i<999;i++){
        ans = inverse(ans,p);
        ans ^= temp;
    }
    cout<<ans<<endl;
    求逆部分结束*/

    /*平方部分开始
    for(int i=0;i<1000;i++){
        stuid = square(stuid,r);
    }
    cout<<stuid<<endl;
    平方部分结束*/

    /*乘法部分开始
    for(int i=0;i<1000;i++){
        ans = mul(ans,stuid,r);
    }
    cout<<stuid<<endl;
    乘法部分结束*/
     
    //测试部分结束。
}




