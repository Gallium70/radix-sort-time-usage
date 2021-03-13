# 基数选取对基数排序时间的影响 演示代码

本项目旨在演示在基数排序中，不同的基数大小对总排序时间的影响。
题目描述见 [LibreOJ #2286.「WC2017」挑战](https://loj.ac/p/2286) 任务一，本项目使用了和其模板程序相同的数据生成方法，即给定一个伪随机数生成器，输入初始种子和数据数量，生成待排序的数据。这样可以避免从硬盘中读取大量数据消耗的时间。

## 编译指令

`g++ radix_sort.cc -o radix_sort -Wall -Wextra -O2 -lm -fpermissive -static -std=c++11`

## 文件内容

- [radix_sort.cc](radix_sort.cc): 源代码
- [radix_sort_time.csv](radix_sort_time.csv): 不同基数下的总排序时间和每轮排序消耗的时间

## 注意事项

程序运行时会申请最多6GB的内存，请确保你的目标平台能够承受这一空间需求，或手动修改[radix_sort.cc](radix_sort.cc)第90行的循环终止条件。
运行结果受众多因素影响，即使在同一平台上也无法确保两次运行结果相同，数据仅供参考。
