import struct
import hashlib
top = 0xffffffff

def sha1(msg):
    #先填充
    msg = pad(msg)
    #初始化h0~h4
    h0 = 0x67452301
    h1 = 0xefcdab89
    h2 = 0x98badcfe
    h3 = 0x10325476
    h4 = 0xc3d2e1f0

    for j in range(len(msg) // 64):
        part = msg[j * 64: (j+1) * 64]

        w = {}
        for i in range(16):
            word = part[i*4: (i+1)*4]
            (w[i],) = struct.unpack(">i", word)
            # 按照大端序，将word(bytes类型)转换为int类型

        for i in range(16, 80):
            # 按照之前已有的字生成新的字
            w[i] = rotl((w[i-3] ^ w[i-8] ^ w[i-14] ^ w[i-16]) & top, 1)

        a = h0
        b = h1
        c = h2
        d = h3
        e = h4

        for i in range(0, 80):
            if 0 <= i <= 19:
                f = (b & c) | ((~ b) & d)
                k = 0x5A827999
            elif 20 <= i <= 39:
                f = b ^ c ^ d
                k = 0x6ED9EBA1
            elif 40 <= i <= 59:
                f = (b & c) | (b & d) | (c & d)
                k = 0x8F1BBCDC
            elif 60 <= i <= 79:
                f = b ^ c ^ d
                k = 0xCA62C1D6
            #移位和变量交换操作
            temp = add([rotl(a, 5), f, e, k, w[i]])
            e = d
            d = c
            c = rotl(b, 30)
            b = a
            a = temp

        h0 = add([h0, a])
        h1 = add([h1, b])
        h2 = add([h2, c])
        h3 = add([h3, d])
        h4 = add([h4, e])

    h_list = [h0, h1, h2, h3, h4]
    ret = bytes()
    #将h0~h4拼接起来
    for h in h_list:
        ret += h.to_bytes(4, "big")

    return ret


def pad(msg):
    sz = len(msg)
    bits_number = sz * 8
    padding = 512 - ((bits_number + 8) % 512) - 64

    return msg + bytes.fromhex("80") + (padding // 8) * bytes.fromhex("00") + struct.pack(">Q", bits_number)


def rotl(i, n):
    # 循环左移
    # 由于python的int没有长度限制，因此不能简单地移位
    # 采用全一的二进制数据和目的循环移位数据串作“与”运算来获取原来的位
    left = top << (32-n)
    right = top >> n
    #“与”运算
    l = i & left
    r = i & right
    newl = r << n
    newr = l >> (32-n)
    return newl + newr

def add(l):
    # 将列表l中的元素模2^32加
    res = 0
    for e in l:
        res = (res + e) & top
        # 通过与全“1”变量相与保证结果的范围在0-2^32-1之内
    return res
