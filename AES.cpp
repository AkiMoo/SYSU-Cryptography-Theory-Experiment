#include <bitset>
#include <cstddef>
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>

using std::cout;
using std::endl;
using std::string;
using std::vector;

typedef std::bitset<8> byte;
typedef std::bitset<32> word;

extern word RCon[10];
extern byte IV[16];

// AES for 128 bit
class AES {
   private:
    word ext_key[44];  // call KeyExpansion to set this
    // key store according to columns
    byte key[16];  // call setKey to set this // 16 Bytes

   public:
    AES(string s_key = "moxuanqi");
    // set key//original key
    void setKey(string s_key = "moxuanqi");
    // 4 bytes to 1 word
    word toWord(byte b1, byte b2, byte b3, byte b4);
    // RotWord(b0,b1,b2,b3) = (b1,b2,b3,b0)
    word RotWord(word w);
    // input a word, for each byte, substitute by S box
    word SubWord(word w);
    // expand the key
    void KeyExpansion();
    // FieldMult: return multiply result under GF(2^8), mod: x^8 +x^4 +x^3 +x +1
    byte FieldMult(byte lhs, byte rhs);
    // SubBytes //loop step 1
    void SubBytes(byte state[4 * 4]);
    void InvSubBytes(byte state[4 * 4]);
    // ShiftRows //loop step 2
    void ShiftRows(byte state[4 * 4]);
    void InvShiftRows(byte state[4 * 4]);
    // MixColumns //loop step 2
    void MixColumns(byte state[4 * 4]);
    void InvMixColumns(byte state[4 * 4]);
    // AddRoundKey//No. Nr_k round key
    void AddRoundKey(byte state[4 * 4], size_t Nr_k);
    // encrypt
    void Encrypt(byte state[4 * 4]);
    // input a string to Encrypt
    void Decrypt(byte state[4 * 4]);
    vector<byte> EncryptCBC(string txt);
    vector<char> DecryptCBC(vector<byte> txt);
};


// S盒
byte S_Box[16][16] =
{
	{0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76},
	{0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0},
	{0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15},
	{0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75},
	{0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84},
	{0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF},
	{0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8},
	{0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2},
	{0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73},
	{0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB},
	{0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79},
	{0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08},
	{0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A},
	{0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E},
	{0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF},
	{0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16}
};
//
byte Inv_S_Box[16][16] = 
{
	{0x52, 0x09, 0x6A, 0xD5, 0x30, 0x36, 0xA5, 0x38, 0xBF, 0x40, 0xA3, 0x9E, 0x81, 0xF3, 0xD7, 0xFB},
	{0x7C, 0xE3, 0x39, 0x82, 0x9B, 0x2F, 0xFF, 0x87, 0x34, 0x8E, 0x43, 0x44, 0xC4, 0xDE, 0xE9, 0xCB},
	{0x54, 0x7B, 0x94, 0x32, 0xA6, 0xC2, 0x23, 0x3D, 0xEE, 0x4C, 0x95, 0x0B, 0x42, 0xFA, 0xC3, 0x4E},
	{0x08, 0x2E, 0xA1, 0x66, 0x28, 0xD9, 0x24, 0xB2, 0x76, 0x5B, 0xA2, 0x49, 0x6D, 0x8B, 0xD1, 0x25},
	{0x72, 0xF8, 0xF6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xD4, 0xA4, 0x5C, 0xCC, 0x5D, 0x65, 0xB6, 0x92},
	{0x6C, 0x70, 0x48, 0x50, 0xFD, 0xED, 0xB9, 0xDA, 0x5E, 0x15, 0x46, 0x57, 0xA7, 0x8D, 0x9D, 0x84},
	{0x90, 0xD8, 0xAB, 0x00, 0x8C, 0xBC, 0xD3, 0x0A, 0xF7, 0xE4, 0x58, 0x05, 0xB8, 0xB3, 0x45, 0x06},
	{0xD0, 0x2C, 0x1E, 0x8F, 0xCA, 0x3F, 0x0F, 0x02, 0xC1, 0xAF, 0xBD, 0x03, 0x01, 0x13, 0x8A, 0x6B},
	{0x3A, 0x91, 0x11, 0x41, 0x4F, 0x67, 0xDC, 0xEA, 0x97, 0xF2, 0xCF, 0xCE, 0xF0, 0xB4, 0xE6, 0x73},
	{0x96, 0xAC, 0x74, 0x22, 0xE7, 0xAD, 0x35, 0x85, 0xE2, 0xF9, 0x37, 0xE8, 0x1C, 0x75, 0xDF, 0x6E},
	{0x47, 0xF1, 0x1A, 0x71, 0x1D, 0x29, 0xC5, 0x89, 0x6F, 0xB7, 0x62, 0x0E, 0xAA, 0x18, 0xBE, 0x1B},
	{0xFC, 0x56, 0x3E, 0x4B, 0xC6, 0xD2, 0x79, 0x20, 0x9A, 0xDB, 0xC0, 0xFE, 0x78, 0xCD, 0x5A, 0xF4},
	{0x1F, 0xDD, 0xA8, 0x33, 0x88, 0x07, 0xC7, 0x31, 0xB1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xEC, 0x5F},
	{0x60, 0x51, 0x7F, 0xA9, 0x19, 0xB5, 0x4A, 0x0D, 0x2D, 0xE5, 0x7A, 0x9F, 0x93, 0xC9, 0x9C, 0xEF},
	{0xA0, 0xE0, 0x3B, 0x4D, 0xAE, 0x2A, 0xF5, 0xB0, 0xC8, 0xEB, 0xBB, 0x3C, 0x83, 0x53, 0x99, 0x61},
	{0x17, 0x2B, 0x04, 0x7E, 0xBA, 0x77, 0xD6, 0x26, 0xE1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0C, 0x7D}
};


word RCon[10] = {0x01000000, 0x02000000, 0x04000000, 0x08000000, 0x10000000,
                 0x20000000, 0x40000000, 0x80000000, 0x1b000000, 0x36000000};
//第一个明文块与一个叫初始化向量IV的数据块异或
byte IV[16] = {0x31, 0x32, 0x33, 0x0, 0x0, 0x0, 0x0, 0x0,
               0x0,  0x0,  0x0,  0x0, 0x0, 0x0, 0x0, 0x0};


AES::AES(string s_key) {
    setKey(s_key);
}

// key 一列一列的存
void AES::setKey(string s_key) {
    for (int i = 0; i < 16 && i < s_key.size(); i++) {  // clear
        key[i] = byte(int(s_key[i]));
    }
}

/**
 * 将4个 byte 转换为一个 double_word.
 */
word AES::toWord(byte b0, byte b1, byte b2, byte b3) {
    word out(0);
    //b和out按位或然后赋给out
    //ulong是10进制转2进制然后取后八位（byte）
    out |= b3.to_ulong();
    out |= b2.to_ulong() << 8;
    out |= b1.to_ulong() << 16;
    out |= b0.to_ulong() << 24;
    return out;
}
/**
 *  利用或运算的方式 按字节循环左移一位
 *  即把[a0, a1, a2, a3]变成[a1, a2, a3, a0]
 */
word AES::RotWord(word w) {
    //左移8位按位或右移24位
    return (w << 8) | (w >> 24);
}

/**
 *  对输入word中的每一个字节进行S-盒变换
 */
word AES::SubWord(word w) {
    word out(0);
    for (int i = 0; i < 32; i += 8) {  // 4 bytes
        int row = w[i + 7] * 8 + w[i + 6] * 4 + w[i + 5] * 2 + w[i + 4];
        int col = w[i + 3] * 8 + w[i + 2] * 4 + w[i + 1] * 2 + w[i];
        byte val = S_Box[row][col];
        for (int j = 0; j < 8; j++) {
            out[i + j] = val[j];
        }
    }
    return out;
}

/**
 *  密钥扩展函数 - 对128位密钥进行扩展得到 w[4*(Nr+1)]
 */
//扩展密钥
void AES::KeyExpansion() {
    for (int i = 0; i < 4; i++) {
        ext_key[i] = toWord(key[4 * i], key[4 * i + 1], key[4 * i + 2], key[4 * i + 3]);
    }
    for (int i = 4; i < 44; i++) {
        word temp = ext_key[i - 1];
        
        if (i % 4 == 0) {
            temp = SubWord(RotWord(temp)) ^ RCon[i / 4 - 1];
        }
        ext_key[i] = ext_key[i - 4] ^ temp;
    }

}

byte AES::FieldMult(byte lhs, byte rhs) {
    byte out(0);
    for (int i = 0; i < 8; i++) {
        if ((lhs & (byte(1) << i)) != 0) {
            out ^= rhs;
        }
        if ((rhs & (byte(1) << 7)) != 0) {  // meet the highest bit
            rhs <<= 1;//右移一位
            rhs ^= byte(0x1b);  // mod x^8 +x^4 +x^3 +x +1
        } else {
            rhs <<= 1;
        }
    }
    return out;
}
//根据S盒进行置换
void AES::SubBytes(byte state[4 * 4]) {
    for (int i = 0; i < 16; i++) {
        int row =
            state[i][7] * 8 + state[i][6] * 4 + state[i][5] * 2 + state[i][4];
        int col =
            state[i][3] * 8 + state[i][2] * 4 + state[i][1] * 2 + state[i][0];
        state[i] = S_Box[row][col];

    }
}
// 逆S盒变换
void AES::InvSubBytes(byte state[4 * 4])
{
    for (int i = 0; i < 16; i++) {
        int row =
            state[i][7] * 8 + state[i][6] * 4 + state[i][5] * 2 + state[i][4];
        int col =
            state[i][3] * 8 + state[i][2] * 4 + state[i][1] * 2 + state[i][0];
        state[i] = Inv_S_Box[row][col];
    }
}



void AES::ShiftRows(byte state[4 * 4]) {

    byte exchange = state[1];
    for (int i = 0; i < 3; i++) {  // row 2
        state[4 * i + 1] = state[4 * i + 5];
    }
    state[13] = exchange;          // row 2 shift end
    for (int i = 0; i < 2; i++) {  // row 3
        exchange = state[4 * i + 2];
        state[4 * i + 2] = state[4 * i + 10];
        state[4 * i + 10] = exchange;
    }  // row 3 shift end
    // row4 left shift 3, or right shift 1
    exchange = state[15];
    for (int i = 3; i > 0; i--) {
        state[4 * i + 3] = state[4 * i - 1];
    }
    state[3] = exchange;
}
/**
 *  逆行变换 - 以字节为单位循环右移
 */ 
void AES::InvShiftRows(byte state[4 * 4])
{
	// 第二行循环右移一位
	byte exchange = state[7];
	for(int i=3; i>0; --i)
		state[i+4] = state[i+3];
	state[4] = exchange;
	// 第三行循环右移两位
	for(int i=0; i<2; ++i)
	{
		exchange = state[i+8];
		state[i+8] = state[i+10];
		state[i+10] = exchange;
	}
	// 第四行循环右移三位
	exchange = state[12];
	for(int i=0; i<3; ++i)
		state[i+12] = state[i+13];
	state[15] = exchange;
}



void AES::MixColumns(byte state[4 * 4]) {
    byte t[4];
    for (int i = 0; i < 4; i++) {  // i col
        for (int j = 0; j < 4; j++) {
            t[j] = state[4 * i + j];
        }
        state[4 * i] =
            FieldMult(0x2, t[0]) ^ FieldMult(0x3, t[1]) ^ t[2] ^ t[3];
        state[4 * i + 1] =
            FieldMult(0x2, t[1]) ^ FieldMult(0x3, t[2]) ^ t[3] ^ t[0];
        state[4 * i + 2] =
            FieldMult(0x2, t[2]) ^ FieldMult(0x3, t[3]) ^ t[0] ^ t[1];
        state[4 * i + 3] =
            FieldMult(0x2, t[3]) ^ FieldMult(0x3, t[0]) ^ t[1] ^ t[2];
    }
}
void AES::InvMixColumns(byte state[4 * 4])
{
	byte t[4];
	for(int i=0; i<4; ++i)
	{
		for(int j=0; j<4; ++j)
			t[j] = state[i+j*4];

		state[i] = FieldMult(0x0e, t[0]) ^ FieldMult(0x0b, t[1]) ^ FieldMult(0x0d, t[2]) ^ FieldMult(0x09, t[3]);
		state[i+4] = FieldMult(0x09, t[0]) ^ FieldMult(0x0e, t[1]) ^ FieldMult(0x0b, t[2]) ^ FieldMult(0x0d, t[3]);
		state[i+8] = FieldMult(0x0d, t[0]) ^ FieldMult(0x09, t[1]) ^ FieldMult(0x0e, t[2]) ^ FieldMult(0x0b, t[3]);
		state[i+12] = FieldMult(0x0b, t[0]) ^ FieldMult(0x0d, t[1]) ^ FieldMult(0x09, t[2]) ^ FieldMult(0x0e, t[3]);
	}
    
}
/**
 *  轮密钥加变换 - 将每一列与扩展密钥进行异或
 */
void AES::AddRoundKey(byte state[4 * 4], size_t Nr_k) {
    for (int i = 0; i < 4; i++) {  // i col
        unsigned long n0 = (ext_key[Nr_k * 4 + i] >> 24).to_ulong();
        state[4 * i] ^= byte(n0);
        unsigned long n1 = ((ext_key[Nr_k * 4 + i] << 8) >> 24).to_ulong();
        state[4 * i + 1] ^= byte(n1);
        unsigned long n2 = ((ext_key[Nr_k * 4 + i] << 16) >> 24).to_ulong();
        state[4 * i + 2] ^= byte(n2);
        unsigned long n3 = ((ext_key[Nr_k * 4 + i] << 24) >> 24).to_ulong();
        state[4 * i + 3] ^= byte(n3);
    }
}



void AES::Encrypt(byte state[4 * 4]) {
    //size_t Nr_k = 0;
    KeyExpansion();
    AddRoundKey(state, 0);
    for (int Nr_k = 1; Nr_k < 10; Nr_k++) {
        
        SubBytes(state);
        ShiftRows(state);
        MixColumns(state);
        AddRoundKey(state, Nr_k);
    }
    SubBytes(state);
    ShiftRows(state);
    AddRoundKey(state, 10);
}

void AES::Decrypt(byte state[4 * 4])
{
    KeyExpansion();
	AddRoundKey(state, 10);
	for(int round=10-1; round>0; --round)
	{
		InvShiftRows(state);
		InvSubBytes(state);
		AddRoundKey(state,round);
		InvMixColumns(state);
	}
	InvShiftRows(state);
	InvSubBytes(state);
	AddRoundKey(state,0);
}

vector<byte> AES::EncryptCBC(string txt) {
    vector<byte> out;
    // string s;
    int pad = 16 - txt.size() % 16;
    for (int i = 0; i < pad; i++) {
        txt += char(pad);
    }
    byte to_xor[4 * 4];
    memcpy(to_xor, IV, sizeof(IV));
    for (int i = 0; i < txt.size(); i += 16) {
        byte state[4 * 4];
        for (int j = 0; j < 16; j++) {
            state[j] = byte(int(txt.at(i + j)));  // Xor:input 16 bytes
            state[j] ^= to_xor[j];
        }
        Encrypt(state);
        memcpy(to_xor, state, sizeof(state));
        for (int j = 0; j < 16; j++) {
            out.push_back(state[j].to_ulong());
        }
    }
    return out;
}

vector<char> AES::DecryptCBC(vector<byte> txt) {
    vector<char> out;
    // string s;
    byte to_xor[4 * 4];
    byte sup[4 * 4];
    memcpy(to_xor, IV, sizeof(IV));
    for (int i = 0; i < txt.size(); i += 16) {
        byte state[4 * 4];
        for (int j = 0; j < 16; j++) {
            state[j] = txt.at(i + j);  // Xor:input 16 bytes
            sup[j] = txt.at(i + j);
        }
        Decrypt(state);
        for (int j = 0; j < 16; j++) {
            state[j] ^= to_xor[j];
        }
        memcpy(to_xor, sup, sizeof(sup));
        for (int j = 0; j < 16; j++) {
            out.push_back((state[j].to_ulong()));
            //cout<<out.at(i);
        }
    }
    return out;
}

int main() {
    AES aes;
    AES ddd;
    byte b1 = 0x1;
    byte b2 = 0x2;
    byte b3 = 0x4;
    byte b4 = 0x8;
    
    string key("moxuanqi");
    string txt("ilearnedhowtocalculatetheamountofpaperneededforaroomwheniwasatschoolyoumultiplythesquarefootageofthewallsbythecubiccontentsofthefloorandceilingcombinedanddoubleityouthenallowhalfthetotalforopeningssuchaswindowsanddoorsthenyouallowtheotherhalfformatchingthepatternthenyoudoublethewholethingagaintogiveamarginoferrorandthenyouorderthepaper");
    cout << "plain text: " << txt << endl;
    cout << "key:        " << key << endl;
    aes.setKey(key);
    vector<byte> out = aes.EncryptCBC(txt);
    
    cout << "after Encrypt: " << endl;
    for (int i = 0; i < out.size(); i++) {
        if (out.at(i).to_ulong() < 16) {
            cout << 0;
        }
        cout << std::hex << out.at(i).to_ulong();
    }
    cout<<endl;
    cout<<"Decrypt:"<<endl;
    vector<char> out2 = ddd.DecryptCBC(out);
    for (int i = 0; i < out2.size(); i++) {
        cout  << (char)out2.at(i);
    }
    return 0;
}