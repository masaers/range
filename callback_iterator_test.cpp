#include "callback_iterator.hpp"
#include <iostream>

int main(const int argc, const char** argv) {
  using namespace std;
  using namespace com_masaers;
  
  auto it = callback_iterator([&](int i) { cout << i % 5 << endl; });
  for (int i = 0; i < 40; ++i) {
    *it++ = i;
  }
  
  return EXIT_SUCCESS;
}
