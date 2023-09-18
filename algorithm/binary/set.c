// 枚举sup的所有真子集
int sub = sup;
do {
    sub = (sub - 1) & sup;
} while(sub != sup);

// 枚举{0,1,2,...,n-1}中所有大小为k的子集
int comb = (1 << k) - 1;
while(comb < (1 << n)) {
    int x = comb & -comb, y = comb + x;
    comb = ((comb & ~y) / (x >> 1)) | y;
}

