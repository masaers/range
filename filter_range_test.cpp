#include "filter_range.hpp"
#include <iostream>
#include <vector>

int main(const int argc, const char** argv) {
  using namespace std;
  vector<int> v;
  for (int i = 0; i < 10; ++i) {
    v.push_back(i);
  }
  auto v2 = make_filter_range(v, [](int i){ return i % 2 == 0; });
  for (auto& i : v2) {
    cout << i << ' ';
  }
  cout << endl;
  
  return EXIT_SUCCESS;
}

