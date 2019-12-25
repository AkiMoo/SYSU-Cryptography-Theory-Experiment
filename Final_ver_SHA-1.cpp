#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <string>
#include <fstream>
using namespace std;
class SHA1 {
    private:
        const uint32_t ori_H[5] = {0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476, 0xC3D2E1F0};
        size_t buffer_count = 0;  //缓冲大小
        uint32_t* buffer = NULL;  //填补完后文本
        void SHA1_pad(string str);//填充函数
        uint32_t K(int t);
        uint32_t ROTL(int n, uint32_t Word);
        uint32_t f(int t, uint32_t B, uint32_t C, uint32_t D);

    public:
        uint32_t ans_H[5] = {0, 0, 0, 0, 0};
        string Hash(string str);
};
//K变量的定义
uint32_t SHA1::K(int t) {
    if (0 <= t && t <= 19) {
        return 0x5A827999;
    } else if (20 <= t && t <= 39) {
        return 0x6ED9EBA1;
    } else if (40 <= t && t <= 59) {
        return 0x8F1BBCDC;
    } else if (60 <= t && t <= 79) {
        return 0xCA62C1D6;
    } else {
        cout << "K: t out of range! valid range: [0, 79]" << endl;
        return 0;
    }
}
//循环移位函数，溢出的位用原来的字右移溢出后做或运算
uint32_t SHA1::ROTL(int n, uint32_t Word) {
    return (Word << n) | (Word >> (32 - n));
}
//四种f(B,C,D)结果
uint32_t SHA1::f(int t, uint32_t B, uint32_t C, uint32_t D) {
    if (0 <= t && t <= 19) {
        return (B & C) | ((~B) & D);
    }
    else if (20 <= t && t <= 39) {
        return B ^ C ^ D;
    }
    else if (40 <= t && t <= 59) {
        return (B & C) | (B & D) | (C & D);
    }
    else{
        return B ^ C ^ D;
    }
}
//填充函数
void SHA1::SHA1_pad(string str) {
    buffer_count = (str.size() + 1 + 8) / 4 + (((str.size() + 1 + 8) % 4 != 0) ? 1 : 0);
    if (buffer_count % 16 != 0) {
        buffer_count = (buffer_count / 16) * 16 + 16;
    }
    buffer = new uint32_t[buffer_count]; 
    memset(buffer, 0, buffer_count * 4);
    uint64_t l = str.size() * 8;  
    int d = (447 - l) % 512;
    if (d < 0) {
        d += 512;
    }
    int i = 0;
    for (; i < str.size() / 4; i++) { 
        buffer[i] =
        (uint32_t(str[4 * i]) << 24) | (uint32_t(str[4 * i + 1]) << 16) | (uint32_t(str[4 * i + 2]) << 8) | uint32_t(str[4 * i + 3]);
    }
    int j = 0;
    for (; j < str.size() % 4; j++) {
        buffer[i] |= uint32_t(str[4 * i + j]) << ((3 - j) * 8);
    }
    buffer[i] |= (uint32_t(1) << 7) << ((3 - j) * 8); //填充一个1
    for (i += 1; i < buffer_count - 2; i++) {  
        buffer[i] = 0;//填充0补满
    }
    buffer[buffer_count - 2] = uint32_t(l >> 32);
    buffer[buffer_count - 1] = uint32_t(l & 0xFFFF);
}

string SHA1::Hash(string str) {
    if (buffer != NULL) {  
        delete buffer;
        buffer = NULL;
    }
    SHA1_pad(str);
    int n = buffer_count * 32 / 512;
    for (int i = 0; i < 5; i++) {
        ans_H[i] = ori_H[i];
    }
    uint32_t Word[80];
    for (int i = 0; i < n; i++) { 
        for (int t = 0; t < 16; t++) {
            Word[t] = buffer[i * 16 + t];
        }
        for (int t = 16; t < 80; t++) { 
            Word[t] = ROTL(1, Word[t - 3] ^ Word[t - 8] ^ Word[t - 14] ^ Word[t - 16]);
        }
        uint32_t A = ans_H[0];
        uint32_t B = ans_H[1];
        uint32_t C = ans_H[2];
        uint32_t D = ans_H[3];
        uint32_t E = ans_H[4];
        for (int t = 0; t < 80; t++) {
            uint32_t temp = ROTL(5, A) + f(t, B, C, D) + E + Word[t] + K(t);
            E = D;
            D = C;
            C = ROTL(30, B);
            B = A;
            A = temp;
        }
        ans_H[0] += A;
        ans_H[1] += B;
        ans_H[2] += C;
        ans_H[3] += D;
        ans_H[4] += E;
    }
    
    char arr_char[40];
    for (int i = 0; i < 5; i++) { 
        sprintf(arr_char + 8 * i * sizeof(char), "%08x", ans_H[i]);//将H0H1H2H3H4拼接起来，十六进制格式。
    }
    string out_str(arr_char);
    return out_str;
}

string readFileIntoString(char * filename)
{
    ifstream ifile(filename);
    //将文件读入到ostringstream对象buf中
    ostringstream buf;
    char ch;
    while(buf&&ifile.get(ch))
    buf.put(ch);
    //返回与流对象buf关联的字符串
    return buf.str();
}


bool Single_Test(string plain_text, string expected_ans) {
    SHA1 sha_1;
    string ans;
    cout << left << setw(14) << "plain text:" << plain_text << endl;
    ans = sha_1.Hash(plain_text);
    if (ans != expected_ans) {
        cout << setw(14) << "[error]" << ans << "   is wrong!" << endl;
        cout << setw(14) << "expected:" << expected_ans << endl;
        return false;
    } else {
        cout << setw(14) << "[Pass]" << ans << "   is right!" << endl;
        return true;
    }
}
void Run(string txt){
    SHA1 amis;
    string result;
    result = amis.Hash(txt);
    cout<< result<<endl;
}
bool Sha1_Test() {
    if (Single_Test("17341126", "629354e95dffd78d9b117337c6c73a300c54146e") == false) {
        return false;
    }
    return true;
}

int main() {
    string plain;
    char* readFile="Plain.txt";
    plain = readFileIntoString(readFile);
    cout << "\n=== Test for Sha-1 Hash Algorithm, stu_id=17341126 ==="<<endl<<endl;
    cout << (Sha1_Test() == true ? "Test Pass" : "Test Failed") << endl;

    cout<<"\n=== Before Hash string is: ==="<<endl<<plain<<endl;
    cout<<"\n=== After Hash string is: ==="<<endl;
    Run(plain);
    system("pause");
    return 0;
}