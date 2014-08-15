#include "zip_range.hpp"
#include <iostream>
#include <vector>

int main(const int argc, const char** argv) {
  using namespace std;
  
  vector<int> v1{1, 2, 3};
  vector<double> v2{0.1, 0.2, 0.3};
  for (auto i : make_zip_range(v1, v2)) {
    cout << get<0>(i) << ' ' << get<1>(i) << endl;
  }
  
  return EXIT_SUCCESS;
}

