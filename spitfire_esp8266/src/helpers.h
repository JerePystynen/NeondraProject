template<typename T, size_t N> constexpr int array_sum(const T(&arr)[N]) {
  int total = 0;
  for (size_t i = 0; i < N; i++) {
    total += arr[i];
  }
  return total;
}