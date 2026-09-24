/* WARNING: Do not include any other libraries here,
 * otherwise you will get an error while running test.py
 * You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.
 *
 * Using printf will interfere with our script capturing the execution results.
 * At this point, you can only test correctness with ./btest.
 * After confirming everything is correct in ./btest, remove the printf
 * and run the complete tests with test.py.
 */

 /*
 * bitAnd - x & y using only ~ and |
 * Example: bitAnd(4, 5) = 4
 * Legal ops: ~ |
 * Max ops: 7
 * Difficulty: 1
 */
int bitAnd(int x, int y) {
    /* 德摩根律：x&y = ~(~x | ~y)，"两格都是1" = "不是至少一格为0" */
    return ~(~x | ~y);
}

/*
 * bitXor - x ^ y using only ~ and &
 *   Example: bitXor(4, 5) = 1
 *   Legal ops: ~ &
 *   Max ops: 7
 *   Difficulty: 1
 */
int bitXor(int x, int y) {
    /* "两格不同" = 既不是"都是1"(x&y)，也不是"都是0"(~x&~y) */
    return ~(x & y) & ~(~x & ~y);
}

/*
 * samesign - Determines if two integers have the same sign.
 *   0 is not positive, nor negative
 *   Example: samesign(0, 1) = 0, samesign(0, 0) = 1
 *            samesign(-4, -5) = 1, samesign(-4, 5) = 0
 *   Legal ops: >> << ! ^ && if else &
 *   Max ops: 12
 *   Difficulty: 2
 *
 * Parameters:
 *   x - The first integer.
 *   y - The second integer.
 *
 * Returns:
 *   1 if x and y have the same sign , 0 otherwise.
 */
int samesign(int x, int y) {
    /* 0 单独算一类：都0算同号；恰好一个0算不同号；
       都非0时符号位相同 <=> (x^y) 的第31格是0 */
    if (!x && !y)
        return 1;
    if (!x ^ !y)
        return 0;
    return !((x ^ y) >> 31);
}

/*
 * logtwo - Calculate the base-2 logarithm of a positive integer using bit
 *   shifting. (Think about bitCount)
 *   Note: You may assume that v > 0
 *   Example: logtwo(32) = 5
 *   Legal ops: > < >> << |
 *   Max ops: 25
 *   Difficulty: 4
 */
int logtwo(int v) {
    /* 二分找最高位的1：比较式本身是0/1，<<4/3/2/1 后既当累加值又当移位量 */
    int r = 0;
    int s;
    s = ((v >> 16) > 0) << 4;
    r = r | s;
    v = v >> s;
    s = ((v >> 8) > 0) << 3;
    r = r | s;
    v = v >> s;
    s = ((v >> 4) > 0) << 2;
    r = r | s;
    v = v >> s;
    s = ((v >> 2) > 0) << 1;
    r = r | s;
    v = v >> s;
    s = (v >> 1) > 0;
    r = r | s;
    return r;
}

/*
 *  byteSwap - swaps the nth byte and the mth byte
 *    Examples: byteSwap(0x12345678, 1, 3) = 0x56341278
 *              byteSwap(0xDEADBEEF, 0, 2) = 0xDEEFBEAD
 *    Note: You may assume that 0 <= n <= 3, 0 <= m <= 3
 *    Legal ops: ! ~ & ^ | + << >>
 *    Max ops: 17
 *    Difficulty: 2
 */
int byteSwap(int x, int n, int m) {
    /* 异或翻牌子：d = 两字节的差异格，在 n、m 两个位置异或 d<<(n*8)、d<<(m*8)
       即完成交换（n=m 时 d=0，自动还原） */
    int a = (x >> (n << 3)) & 0xFF;
    int b = (x >> (m << 3)) & 0xFF;
    int d = a ^ b;
    return x ^ (d << (n << 3)) ^ (d << (m << 3));
}

/*
 * reverse - Reverse the bit order of a 32-bit unsigned integer.
 *   Example: reverse(0xFFFF0000) = 0x0000FFFF reverse(0x80000000)=0x1 reverse(0xA0000000)=0x5
 *   Note: You may assume that an unsigned integer is 32 bits long.
 *   Legal ops: << | & - + >> for while ! ~ (You can define unsigned in this function)
 *   Max ops: 30
 *   Difficulty: 3
 */
unsigned reverse(unsigned v) {
    /* 循环逐位搬运：每轮 r 左移腾出最右格、接住 v 的最右格，v 右移露下一格。
       用计数器 m（1,2,4,...,2^32 回绕到0）控制 32 轮，
       循环条件不带比较符——本题 Legal ops 里没有 < > */
    unsigned r = 0;
    unsigned m = 1;
    while (m) {
        r = (r << 1) | (v & 1);
        v = v >> 1;
        m = m << 1;
    }
    return r;
}

/*
 * logicalShift - shift x to the right by n, using a logical shift
 *   Examples: logicalShift(0x87654321,4) = 0x08765432
 *   Note: You can assume that 0 <= n <= 31
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Difficulty: 3
 */
int logicalShift(int x, int n) {
    /* 算术右移后把高 n 格清零。np = n + !n 保证移位量 32-np <= 31 不触发 UB；
       n=0 时再用 !n<<31 把最高格补回，掩码变成全1 */
    int np = n + !n;
    int mask = (1 << (33 + ~np)) + ~0;
    mask = mask | (!n << 31);
    return (x >> n) & mask;
}

/*
 * leftBitCount - returns count of number of consective 1's in left-hand (most) end of word.
 *   Examples: leftBitCount(-1) = 32, leftBitCount(0xFFF0F0F0) = 12,
 *             leftBitCount(0xFE00FF0F) = 7
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 50
 *   Difficulty: 4
 */
int leftBitCount(int x) {
    /* 对 x 直接二分：x>>k 等于 -1（符号扩展全1） <=> x 的最高 k 格全是1。
       是则答案加上 k 并把 x 左移 k 格（低位移入0，天然是计数终止符）；
       依次检查 16/8/4/2，最后剩 2 格时逐格检查（16+8+4+2+1+1 = 32 位全覆盖） */
    int ans = 0;
    int t;
    t = !(~(x >> 16));
    ans = ans + (t << 4);
    x = x << (t << 4);
    t = !(~(x >> 24));
    ans = ans + (t << 3);
    x = x << (t << 3);
    t = !(~(x >> 28));
    ans = ans + (t << 2);
    x = x << (t << 2);
    t = !(~(x >> 30));
    ans = ans + (t << 1);
    x = x << (t << 1);
    t = !(~(x >> 31));
    ans = ans + t;
    x = x << t;
    t = !(~(x >> 31));
    ans = ans + t;
    return ans;
}

/*
 * float_i2f - Return bit-level equivalent of expression (float) x
 *   Result is returned as unsigned int, but it is to be interpreted as
 *   the bit-level representation of a single-precision floating point values.
 *   Legal ops: if else while for & | ~ + - >> << < > ! ==
 *   Max ops: 30
 *   Difficulty: 4
 */
unsigned float_i2f(int x) {
    /* 手工模拟 (float)x：取绝对值→左移到 1 顶格定指数→取 23 格尾数→
       剩余 8 格向偶数舍入→进位连锁时指数+1→拼位模式 */
    unsigned sign = 0;
    unsigned a;
    int e = 31;
    unsigned frac, round;
    if (x == 0)
        return 0;
    if (x < 0) {
        sign = 0x80000000u;
        a = ~x;
        a = a + 1;
    } else {
        a = x;
    }
    while (!(a & 0x80000000u)) {
        a = a << 1;
        e = e - 1;
    }
    frac = (a >> 8) & 0x7FFFFF;
    round = a & 0xFF;
    if (round > 0x80)
        frac = frac + 1;
    else if (round == 0x80)
        if (frac & 1)
            frac = frac + 1;
    if (frac == 0x800000u) {
        frac = 0;
        e = e + 1;
    }
    return sign | ((e + 127) << 23) | frac;
}

/*
 * floatScale2 - Return bit-level equivalent of expression 2*f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   When argument is NaN, return argument
 *   Legal ops: & >> << | if > < >= <= ! ~ else + ==
 *   Max ops: 30
 *   Difficulty: 4
 */
unsigned floatScale2(unsigned uf) {
    /* exp=255（Inf/NaN）原样返回；exp=0 尾数左移一格（顶进阶码自动规格化）；
       规格化数阶码+1（加到255自然变Inf） */
    unsigned exp = (uf >> 23) & 0xFF;
    unsigned frac = uf & 0x7FFFFF;
    unsigned sign = uf & 0x80000000;
    if (exp == 255)
        return uf;
    if (exp == 0)
        return sign | (frac << 1);
    return sign | ((exp + 1) << 23) | frac;
}

/*
 * float64_f2i - Convert a 64-bit IEEE 754 floating-point number to a 32-bit signed integer.
 *   The conversion rounds towards zero.
 *   Note: Assumes IEEE 754 representation and standard two's complement integer format.
 *   Parameters:
 *     uf1 - The lower 32 bits of the 64-bit floating-point number.
 *     uf2 - The higher 32 bits of the 64-bit floating-point number.
 *   Returns:
 *     The converted integer value, or 0x80000000 on overflow, or 0 on underflow.
 *   Legal ops: >> << | & ~ ! + - > < >= <= if else
 *   Max ops: 60
 *   Difficulty: 3
 */
int float64_f2i(unsigned uf1, unsigned uf2) {
    /* e<0（含非规格化）绝对值<1 → 0；e>=31（含 Inf/NaN）→ 0x80000000；
       否则整数部分 = 隐含1 << e 加上尾数的前 e 格（向零舍入即丢弃小数） */
    int sign = uf2 >> 31;
    int exp = (uf2 >> 20) & 0x7FF;
    int e = exp - 1023;
    int result;
    if (e < 0)
        return 0;
    if (e >= 31)
        return 0x80000000;
    if (e <= 20) {
        result = ((uf2 & 0xFFFFF) >> (20 - e)) | (1 << e);
    } else {
        result = ((uf2 & 0xFFFFF) << (e - 20)) | (uf1 >> (52 - e)) | (1 << e);
    }
    if (sign)
        result = ~result + 1;
    return result;
}

/*
 * floatPower2 - Return bit-level equivalent of the expression 2.0^x
 *   (2.0 raised to the power x) for any 32-bit integer x.
 *
 *   The unsigned value that is returned should have the identical bit
 *   representation as the single-precision floating-point number 2.0^x.
 *   If the result is too small to be represented as a denorm, return
 *   0. If too large, return +INF.
 *
 *   Legal ops: < > <= >= << >> + - & | ~ ! if else &&
 *   Max ops: 30
 *   Difficulty: 4
 */
unsigned floatPower2(int x) {
    /* 2^x 尾数恒为 1.000...，按 x 所在区间直接拼位模式：
       x>127 上溢→+Inf；[-126,127] 规格化 (x+127)<<23；
       [-149,-127] 非规格化 frac = 1<<(x+149)；更小 → 0 */
    if (x > 127)
        return 0x7F800000;
    if (x >= -126)
        return (x + 127) << 23;
    if (x >= -149)
        return 1 << (x + 149);
    return 0;
}
