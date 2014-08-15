#include "transform_range.hpp"
#include <iostream>
#include <vector>

int main(const int argc, const char** argv) {
  using namespace std;
  
  vector<int> v{0, 1, 2, 3, 4, 5};
  const auto r = transform_range(v, [](int i){ return double(i) * 1.1; });
  for (auto i : r) {
    cout << i << endl;
  }

  return EXIT_SUCCESS;
}

