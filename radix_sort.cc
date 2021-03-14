#include <algorithm>
#include <chrono>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>

using namespace std;
using namespace chrono;

ofstream fout("radix_sort_time.csv");

// 伪随机数生成器
inline uint32_t NextInteger(uint32_t x) {
  x ^= x << 13;
  x ^= x >> 17;
  x ^= x << 5;
  return x;
}

// 根据给定的种子和数量生成待排序的数据
void InitSortData(uint32_t arr[], size_t n, uint32_t seed) {
  for (size_t i = 0; i < n; i++) {
    seed = NextInteger(seed);
    arr[i] = seed;
  }
  return;
}

// 检查排序结果；这里只检查了数组是否为升序
bool CheckSortResult(uint32_t arr[], size_t n) {
  bool res = true;
  --n;
  for (size_t i = 0; i < n; ++i) res &= (arr[i] <= arr[i + 1]);
  return res;
}

void RadixSort(uint32_t arr[], size_t n, int radix_bits) {
  uint64_t radix = 1ULL << radix_bits;
  uint32_t* cnt = new uint32_t[radix];
  uint32_t* tmp = new uint32_t[n];
  uint32_t radix_bitmask = radix - 1, digit;
  for (int shift_bits = 0; shift_bits < 32; shift_bits += radix_bits) {
    // 对于每个数位使用计数排序。基数设为2的整数幂，这样可以用位运算代替乘除和取模
    memset(cnt, 0, radix * sizeof(uint32_t));
    for (int j = 0; j < n; ++j) {
      digit = (arr[j] >> shift_bits) & radix_bitmask;
      ++cnt[digit];
    }
    for (uint64_t j = 1; j < radix; ++j) cnt[j] += cnt[j - 1];
    for (int j = n - 1; j >= 0; --j) {
      digit = (arr[j] >> shift_bits) & radix_bitmask;
      tmp[--cnt[digit]] = arr[j];
    }
    memcpy(arr, tmp, n * sizeof(uint32_t));
  }
  delete[] cnt;
  delete[] tmp;
  return;
}

void RadixSortBenchmark(uint32_t arr[], size_t n, uint32_t seed,
                        int radix_bits) {
  InitSortData(arr, n, seed);
  time_point<high_resolution_clock> begin_time = high_resolution_clock::now();
  RadixSort(arr, n, radix_bits);
  time_point<high_resolution_clock> end_time = high_resolution_clock::now();
  uint64_t time_ms = duration_cast<milliseconds>(end_time - begin_time).count();

  cout << "radix-" << (1ULL << radix_bits) << " sort time: " << time_ms
       << "ms\t result: " << CheckSortResult(arr, n) << endl;
  fout << (1ULL << radix_bits) << ',' << time_ms << ','
       << (time_ms / (32 / radix_bits + (32 % radix_bits != 0))) << endl;
  return;
}

int main(void) {
  // 随机数种子和待排序的数据数量，可自行修改
  uint32_t seed = 20210311;
  size_t n = 2e8;
  // cin >> n >> seed;
  uint32_t* arr = new uint32_t[n];
  cout << boolalpha;

  // 和std::sort()对比
  InitSortData(arr, n, seed);
  time_point<high_resolution_clock> begin_time = high_resolution_clock::now();
  sort(arr, arr + n);
  time_point<high_resolution_clock> end_time = high_resolution_clock::now();
  cout << "std::sort time: "
       << duration_cast<milliseconds>(end_time - begin_time).count() << "ms"
       << endl;
  cout << "std::sort result: " << CheckSortResult(arr, n) << endl << endl;

  fout << "radix,total time (ms),time per digit (ms)" << endl;
  // 从2^1到2^30枚举每一个2的整数幂作为基数，统计运行时间
  // 将30改为更小的数可以减少内存消耗；当然得到的数据也会相应变少
  for (int radix_bits = 1; radix_bits <= 30; ++radix_bits)
    RadixSortBenchmark(arr, n, seed, radix_bits);

  delete[] arr;
  return 0;
}