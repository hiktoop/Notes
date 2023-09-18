# heap sort

```c
int h[10005], cnt = 0;
void up() {
    int cur = cnt;
    while(cur > 1) {
        int p = cur / 2;
        if(h[p] < h[cur]) {
            swap(h[p], h[cur]);
            cur = p;
        }
        else break;
    }
}
void down() {
    int cur = 1;
    while(cur < cnt) {
        int l = cur * 2;
        int r = l + 1;
        if(l > cnt) break;
        if(r <= cnt && h[l] < h[r]) l = r;
        if(h[cur] < h[l]) {
            swap(h[cur], h[l]);
            cur = l;
        }
        else break;
    }
}
void push(int t) {
    h[++cnt] = t;
    up();
}
int pop() {
    int t = h[1];
    swap(h[1], h[cnt--]);
    down();
    return t;
}
```
