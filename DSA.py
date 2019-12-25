import hashlib
import random
import numpy
from random import getrandbits
import os
# -*- coding: utf-8 -*-  

def Multiplicative_Inverse(a, b):
    #用扩展欧几里得算法求b在mod a下的乘法逆
    #按照书上给出的Multiplicative Inverse算法实现
    a0 = a
    b0 = b
    t0 = 0
    t = 1
    q = a0 // b0
    r = a0 % b0
    while (r>0):
        temp = (t0 - q*t) % a
        t0 = t
        t = temp
        a0 = b0
        b0 = r
        q = a0 // b0
        r = a0 % b0
    if(b0 != 1):
        return 0
    else:
        return t

def number_gen(p,q,g):
    #取k为256bit的随机数，因为q是256比特
	k = getrandbits(256)
	if(Multiplicative_Inverse(q,k)!=0&k<=q-1):
        #如果随机数k在q下存在逆，并且k<=q-1,就采用这个k
		return k
	else:
        #否则，重新生成
		return number_gen(p,q,g)

if __name__=='__main__':
    #提供的p，q，g先转成int，方便进行数字化处理
    p = int("008746338ba0d0b67ec6dde878f14d624dfe86a4663aaf170208b02b020c09199af1dbaaf0ba5a70d52b3eb715f775b0989ff176d1d6c680042dab48d35d802c598036280c559ffb59ea4c82c01fae4a227847cb715e03511602f7dedd0d0f1556c5e63c9b181a4a7ffb1416a3d6ef69cb9b413746aab5e259c3b12bec3244a7e09f6718ecb721c528020a9ae09568f461512c526593563fad544106fb5fa90922aef456a0f110ccab7628d793ca3c5f6a4c6e22ee05227e1d3c395f99f52418c5dfdee1442d0aa3ca1e9154cae569c25497c3445f2bed1e8de3cada7e2f1825d7341e591234df4053ca5b33c92e3217967ca00211b68c1b0aafa5152b84557a81",16)
    q = int("00bdd727428445e6e4ad64978a4184de1f28ab3f298e79f11147bee060150040e1",16)
    g = int("75fd519cb49bcfd504dc7f8bfb100511dee71335ba07d7bf85e0b602441b121fc1b537f29b941254f85703d2aa41e2cf400e3be7d952ad9a32f461174873034631295c2bc86d5134779358e8f4c6d54e591edc645dbd389088f9a7dfa3927fbeedce1bc57fa1e6dd3908daac26914ead4f40e8acab8b57a303d1970a06393789c56decbb504f7ae3d9f8e2cd1850b9e77d4aa64f49218fd0f6c24ccb5aa6af9d97e4d041b0676dda90dfbba0177df89b730ab1b8db5ef19ebd2fed52a790826fc507647576bebed69e5a8a11ec25f89d445b56b738d2a7f713d74527a4e6ed99af43ebcdb2cfe2ccaee76fd543ea429ec6402c273b030a63e6a686b2d299d8d7",16)
    #选定学号作为私钥，计算公钥beta
    a = 17341126
    beta = pow(g,a,p)
    print("======================Print private key 'a' & Beta======================")
    print("Private key: 17341126(student ID)",a)
    print("Beta = g^a mod p: ",beta)
    #hash，并将结果转成int
    s = "SchoolofDataandComputerScience,Sunyat-senUniversity".encode("utf8")
    print("Message: SchoolofDataandComputerScience,Sunyat-senUniversity")
    out1 = hashlib.sha256(s).hexdigest()
    x = int(out1,16)
    #生成随机数k并生成在q下的逆
    k = number_gen(p,q,g)
    k_1 = Multiplicative_Inverse(q,k)
    
    #计算出gamma和delta
    print("=========================Gamma & Derta=========================")
    gamma = pow(g,k,p) % q
    print("Gamma is: ",gamma)
    derta = (x + a*gamma)*k_1 % q
    print("Delta is: ",derta)

    #计算e1和e2
    print("============================e1 & e2============================")
    e1 = x * Multiplicative_Inverse(q,derta) % q
    e2 = gamma * Multiplicative_Inverse(q,derta) % q
    print("e1 is: ",e1)
    print("e2 is: ",e2)
    print("========================Authorize Check========================")
    #验证，记得二者乘完先mod一个p再mod q！！！！！！
    if((pow(g,e1,p)*pow(beta,e2,p)%p%q)==gamma):
        print ("((g^e1*Beta^e2)mod p) mod q == Gamma",pow(g,e1,p)*pow(beta,e2,p)%p%q,"\nTrue")
    else:
        print ("((g^e1*Beta^e2)mod p) mod q == Gamma",pow(g,e1,p)*pow(beta,e2,p)%p%q,"\nFalse")
    
    os.system("pause")