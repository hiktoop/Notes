# nim 问题证明

## 基础问题

题目：

> 两个人玩游戏，每人一回合，从 n 堆石子中任选一堆取出至少一个石子，取得最后一个石子的人获胜

首先考虑当只剩两堆石子的时候，如果数量相同，那么后选者总是可以模仿先选者的选取方式而取得胜利；如果数量不同，当选者取出多出的石子便能复刻上述过程

可以发现当两堆石子数量相等时，每堆石子数量的 xor 为 0，即$x \oplus x == 0$

令一次移动之前的每堆石子的数量分别为 $x_1,x_2,...,x_n$

移动之后为                                                   $y_1,y_2,...,y_n$

并令异或和为 $s = x_1 \oplus x_2 \oplus ... \oplus x_n$

                        $t = y_1 \oplus y_2 \oplus ... \oplus y_n$

有

$t = s \oplus s \oplus t $

    $ = s \oplus x_1 \oplus x_2 \oplus ... \oplus x_n \oplus y_1 \oplus y_2 \oplus ... \oplus y_n $

    $= s \oplus (x_1 \oplus y_1) \oplus (x_2 \oplus y_2) \oplus ... \oplus (x_k \oplus y_k) \oplus (x_n \oplus y_n) $

    $= s \oplus x_k \oplus y_k$

当 $s = 0$ 时，t 必不为 0

当 $s \neq 0$ 时，t 可以为 0，因为当 $y_k = s \oplus x_k$ 时 $t = 0$ 

故当玩家在异或和为零的回合是没有选择的，而玩家在异或和不为零的时候总是能够用一次操作使异或和为零而使另一名玩家处于必输的局面
