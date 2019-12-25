from random import getrandbits
#getrandbits(K0) = 获得K0位的随机数
import newsha1 as sha
import os

#initial p,q,a(学号next prime)等变量
p = 26153419880168376362202188987757924012730773103379336702783893911945569765807925598055741718318894668097422282970148143318854063018842330750077600657089762372408043561691429860394698715614135323890419866335662805372776481168782398366213111508120324204925875061027896203507999602050025383990838663255633965623
q = 45227686757726142817811228687677083907344379482222843140857807740988327453019326064607900426630967057699208945203921733522471223684184679683881879177037262786243872392728315927163360907098410601741447298272899449845372785816418721088490488003960338723221940009847907244904470349706168807266187004766056147187
a = 17341127
n = p*q
phi_n = (p - 1)*(q - 1)

#扩展欧几里得除法求逆
def inverse(n, target):
    a = n
    b = target % n
    t0 = 0
    t = 1
    # “//” = integer 式除法
    q = a // b
    r = a % b
    while r > 0:
        temp = (t0 - q*t) % n
        t0 = t
        t = temp
        a = b
        b = r
        q = a // b
        r = a % b
    return t

#python理可以直接对字节对象进行异或，按字节单位逐个异或就不需要先转换成数字类型再异或，防止出错
def xor(a, b):
    #using assert in py(不返回值的判断语句，不符合则停止program)
    assert len(a) == 128
    assert len(b) == 128
    res = bytearray(128)
    for i in range(128):
        res[i] = a[i] ^ b[i]
    return res

def byte_to_int(b):
    #int容器内的from_bytes函数可以把目标byte类型数据转成int，分big和little两种，分别代表高位排序和低位排序
    return int.from_bytes(b, byteorder="big")

def int_to_byte(integer,size = 1024):
    #to_bytes函数将int类型转成字节byte类型，分big和little两种，分别代表高位排序和低位排序
    return integer.to_bytes(size//8, byteorder="big")

def encrypt(plain):
    #创建公钥
    b = inverse(phi_n,a)
    print ("The MESSAGE is:\n", plain)
    #oaep填充
    M = oaeppadding(plain)
    print ("Message after padding is shown as:\n", M)
    #RSA处理
    cipher = pow (M, b, n)
    print ("After RSA process:\n", cipher)
    return cipher

def decrypt(cipher):
    print ("The cipher we get:\n", cipher)
    #RSA逆向解密处理
    M = pow(cipher, a, n)
    print ("Waiting inverse padding text:\n", M)
    #oeap逆向处理，转成utf-8文本
    plain = utf8decode(M)
    print ("The plain is:\n", plain)
    return plain

def oaeppadding(message):
    assert len(message) <= 128
    #转utf-8
    messbytes = int_to_byte(byte_to_int(message.encode("utf-8")))
    #随机数r
    r = getrandbits(1024)
    print ("The random integer 'r' is:\n",r)
    #转byte
    r_byte = int_to_byte(r)
    #哈希后的r与填充后的明文字节异或，生成的密文左部记为X
    X = xor(messbytes, Hash(r_byte))
    #哈希后的X与填充后的r字节异或，生成的密文右部记为Y
    Y = xor(Hash(X), r_byte)

    #进行X和Y的拼接
    res = byte_to_int(X)
    res = res << 1024
    res += byte_to_int(Y)
    return res

def utf8decode(big_int):
    Text = int_to_byte(big_int, 2048)
    #左边取下来记为X，右边取下来记为Y，因为是字节长度 所以2048比特是256字节
    X = Text[0:128]
    Y = Text[128:256]
    #还原出之前的随机数r
    r = xor(Hash(X), Y)
    #再做逆过程还原填充过的文本
    message_bytes = xor(X, Hash(r))
    #输出检验utf-8文本
    print ("utf-8 text of message is:\n",message_bytes)
    #utf-8解码
    message = message_bytes.decode("utf-8")
    
    return message

def Hash(data):
    #hash6次
    data = sha.sha1(data)
    temp = data
    #上面是第一次，给temp变量赋值
    for i in range(5):
        temp = sha.sha1(temp)
        data += temp
    #这里做剩下的五次
    res = int_to_byte(byte_to_int(data))
    assert len(res) == 128
    return res

if __name__ == "__main__":
    print ("============================================================================================")
    print ("Show basis data as follow:")
    print ("Next prime = ",a)
    b = inverse (n,a)
    print ("Its inverse = ",b,"\n")
    print ("p = ",p,"\n")
    print ("q = ",q)
    print ("============================================================================================")
    message = "Sun Yat-sen University"
    decrypt(encrypt(message))
    os.system("pause")