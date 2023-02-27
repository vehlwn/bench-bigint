#include <array>
#include <iostream>

#include <boost/multiprecision/cpp_int.hpp>
#include <boost/range/algorithm/count.hpp>

using BigInt = boost::multiprecision::cpp_int;
using boost::multiprecision::pow, boost::multiprecision::sqrt;

namespace {
BigInt ceil_sqrt(const BigInt &n) {
  BigInt tmp = sqrt(n);
  if (pow(tmp, 2) == n) {
    return tmp;
  } else {
    return tmp + 1;
  }
}

bool is_exact_square(const BigInt &n) {
  constexpr std::array RESIDUES_MOD_100 = {
      0,  1,  4,  9,  16, 21, 24, 25, 29, 36, 41,
      44, 49, 56, 61, 64, 69, 76, 81, 84, 89, 96,
  };
  const auto rem = (n % 100).convert_to<int>();
  if (!boost::count(RESIDUES_MOD_100, rem)) {
    return false;
  }
  return pow(sqrt(n), 2) == n;
}

std::pair<BigInt, BigInt> get_fermat_factor(const BigInt &n) {
  BigInt x = ceil_sqrt(n);
  BigInt y2 = pow(x, 2) - n;
  while (!is_exact_square(y2)) {
    x += 1;
    y2 = pow(x, 2) - n;
  }
  BigInt y = sqrt(y2);
  return {x + y, x - y};
}

} // namespace

int main(int argc, char **argv) {
  BigInt n;
  if (argc >= 2) {
    n = BigInt(argv[1]);
  } else {
    std::cerr << "No input argument!" << std::endl;
    std::exit(1);
  }
  std::cout << "n = " << n << std::endl;
  auto [a, b] = get_fermat_factor(n);
  std::cout << "a = " << a << std::endl;
  std::cout << "b = " << b << std::endl;
}
