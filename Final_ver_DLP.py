import numpy
import math
import os
def Greatest_Common_Divisor(a, b):
    #辗转相除法
    while(b):
        temp = b
        b = a % b
        a = temp
    return a

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

def Pollard_rho_Select(n, p, alpha, beta, x, a, b):
    #三个划分，划成mod 3同余的划分
    #按照书上的例题划分：
    # S1 mod 3 == 1
    # S2 mod 3 == 0
    # S3 mod 3 == 2
    #函数返回3元组，对应书中f()的三个参数
    if(x % 3 == 1):
        return beta*x%p, a, (b+1)%n
    elif(x % 3 == 0):
        return x*x%p, 2*a%n, 2*b%n
    elif(x % 3 == 2):
        return alpha*x%p, (a+1)%n, b

def Solve_Congruence_formula(a, b, n):
    #这个函数目的是解a*x = b (mod n)类指数同余式
    d = Greatest_Common_Divisor(a, n)
    if(d == 1):
        return b * Multiplicative_Inverse(n, a), 0
    if(b % d != 0):
        return -1, -1
    a = a//d
    b = b//d
    n = n//d
    x = (b * Multiplicative_Inverse(n, a))%n
    return x, n

def Pollard_rho(p, alpha, beta, n):
    #n = p-1
    x, a, b = Pollard_rho_Select(n, p, alpha, beta, 1, 0, 0)
    x1, a1, b1 = Pollard_rho_Select(n, p, alpha, beta, x, a, b)
    c = -1
    step = -1
    
    while(x != x1):
        x, a, b = Pollard_rho_Select(n, p, alpha, beta, x, a, b)
        x1, a1, b1 = Pollard_rho_Select(n, p, alpha, beta, x1, a1, b1)
        x1, a1, b1 = Pollard_rho_Select(n, p, alpha, beta, x1, a1, b1)
        if(x == x1):
            #解方程：c*(b1-b) = (a-a1)mod n 其中c代表log_(alpha)^beta
            c, step = Solve_Congruence_formula((b1-b)%n, (a-a1)%n, n)
    '''
    if (Greatest_Common_Divisor(b1-b,n)!=1):
        return -1
    else:
        return (a-a1)*Multiplicative_Inverse(n,b1-1)%n
    '''
    
    if (step == 0):
        return c
    else:
        #如果step不是0，则返回的step值是n整除(a,n)的最大公约数的值，所以方程a*x = b mod n的解形式可以是x = x + k*GCD(a,n)
        times = Greatest_Common_Divisor((a-a1),n)
        i = 0
        for i in range(times):
            #遍历c的同余式求可能出现的解
            #
            if(pow(alpha, c, p) == beta):
                return c
            else:
                c = (c+step)%p
        return -1
    

if __name__ == "__main__":
    '''
    GCD test:
    print (Greatest_Common_Divisor(18,27))
    '''
    
    '''
    p = 809
    g = 123
    ya = 89
    yb = 618
    n = 101
    '''
    
    p=5682549022748424631339131913370125786212509227588493537874673173634936008725904358935442101466555561124455782847468955028529037660533553941399408331331403379
    g=2410497055970432881345493397846112198995088771364307195189734031205605186951241875096796459061741781667380437076874705300974836586165283741668656930807264789
    ya=973768284341326272301553751114322685324340805902069613339667142187801529585352406975030927008752571917079716318221077758236884342662829402529734009607531649
    yb=4149822765985031146554298777122732051870868431387323913747785791685310508836836283702926446817000114868007555717546362425841865173929670156568682745060708314
    n = 4309874666
    #n = p-1
    xa = Pollard_rho(p, g, ya, n)#p, g, ya
    xb = Pollard_rho(p, g, yb, n)#p, g, yb
    print("==================Print Answers:=================")
    print("xa = ",xa)
    print("xb = ", xb)
    print("===========Print Answer Checking Test:===========")
    print("ya == g^xa mod p : ",pow(g, xa, p) == ya)
    print("ya == g^xb mod p : ",pow(g, xb, p) == yb)
    print("==================Secret Share:==================")
    print("pow(g, xa, p) == ya : ", pow(g, xa, p))
    print("pow(g, xb, p) == yb : ", pow(g, xb, p))
    shared_secret = pow(ya, xb, p)
    print('Shared Secret (ya^(xb) mod p): ', shared_secret)
    print("Shared Secret == g^(xa*xb) mod p: ",shared_secret==pow(g,xa*xb,p))
    print("Shared Secret == yb^(xa) mod p: ",shared_secret==pow(yb,xa,p))
    os.system("pause")
    
    