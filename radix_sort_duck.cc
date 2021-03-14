#include <cstdint>
#include <cstring>

constexpr uint8_t kRadixBits = 8;
constexpr size_t kMaxN = 1e8, kRadix = 1 << kRadixBits;
constexpr uint32_t kRadixBitMask = kRadix - 1;
uint32_t buffer[kMaxN], cnt[kRadix] = {0};

#define DIGIT(X, shift_bits) (((X) >> (shift_bits)) & kRadixBitMask)
#define SORT(shift_bits)                                                  \
  {                                                                       \
    memset(cnt, 0, sizeof(cnt));                                          \
    for (uint32_t j = 0; j < n; ++j) ++cnt[DIGIT(buf_p0[j], shift_bits)]; \
    for (uint32_t j = 1; j < kRadix; ++j) cnt[j] += cnt[j - 1];           \
    for (int j = n - 1; j >= 0; --j)                                      \
      buf_p1[--cnt[DIGIT(buf_p0[j], shift_bits)]] = buf_p0[j];            \
    buf_pt = buf_p0, buf_p0 = buf_p1, buf_p1 = buf_pt;                    \
  }

void sort(unsigned* arr, int n) {
  uint32_t *buf_p0 = arr, *buf_p1 = buffer, *buf_pt;
  // for (uint8_t shift_bits = 0; shift_bits < 32; shift_bits += kRadixBits) {
  //   memset(cnt, 0, sizeof(cnt));
  //   for (uint32_t j = 0; j < n; ++j) ++cnt[DIGIT(buf_p0[j], shift_bits)];
  //   for (uint32_t j = 1; j < kRadix; ++j) cnt[j] += cnt[j - 1];
  //   for (int j = n - 1; j >= 0; --j)
  //     buf_p1[--cnt[DIGIT(buf_p0[j], shift_bits)]] = buf_p0[j];
  //   buf_pt = buf_p0, buf_p0 = buf_p1, buf_p1 = buf_pt;
  // }
  SORT(0);
  SORT(8);
  SORT(16);
  SORT(24);
  if (buf_p0 != arr) memcpy(arr, buf_p0, n * sizeof(uint32_t));
  return;
}