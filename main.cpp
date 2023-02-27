#include <array>
#include <atomic>
#include <cstddef>
#include <iostream>
#include <mutex>
#include <thread>

#include <boost/lexical_cast.hpp>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/range/algorithm/count.hpp>
#include <boost/range/algorithm/for_each.hpp>

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
  BigInt y2;
  while (true) {
    y2 = pow(x, 2) - n;
    if (is_exact_square(y2)) {
      break;
    }
    x += 1;
  }
  BigInt y = sqrt(y2);
  return {x + y, x - y};
}

std::pair<BigInt, BigInt> get_fermat_factor_parallel(const BigInt &n,
                                                     const std::size_t world) {
  std::vector<std::thread> pool;
  pool.reserve(world);

  std::atomic_bool done = false;
  std::mutex result_mutex;
  std::pair<BigInt, BigInt> result;
  const auto thread_func = [&](BigInt start_x) {
    BigInt x = std::move(start_x);
    BigInt y2;
    while (!done) {
      y2 = pow(x, 2) - n;
      if (is_exact_square(y2)) {
        done = true;
        const std::lock_guard lock(result_mutex);
        BigInt y = sqrt(y2);
        result = {x + y, x - y};
        break;
      }
      x += world;
    }
  };
  const BigInt start_x = ceil_sqrt(n);
  for (std::size_t i = 0; i < world; i++) {
    pool.emplace_back(thread_func, start_x + i);
  }
  boost::for_each(pool, [](auto &t) {
    if (t.joinable())
      t.join();
  });
  return result;
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
  std::size_t world = 0;
  if (argc >= 3) {
    world = boost::lexical_cast<std::size_t>(argv[2]);
  }
  std::cout << "n = " << n << std::endl;
  std::pair<BigInt, BigInt> result;
  if (world <= 1) {
    result = get_fermat_factor(n);
  } else {
    std::clog << "world = " << world << std::endl;
    result = get_fermat_factor_parallel(n, world);
  }
  const auto [a, b] = result;
  std::cout << "a = " << a << std::endl;
  std::cout << "b = " << b << std::endl;
}
