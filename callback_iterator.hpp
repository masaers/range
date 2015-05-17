#ifndef COM_MASAERS_CALLBACK_ITERATOR_HPP
#define COM_MASAERS_CALLBACK_ITERATOR_HPP
#include <utility>

namespace com_masaers {
  template<typename Func>
  class callback_iterator_t {
  public:
    template<typename... Args> callback_iterator_t(Args&&... args)
      : func_m(std::forward<Args>(args)...)
    {}
    callback_iterator_t(const callback_iterator_t&) = default;
    callback_iterator_t(callback_iterator_t&&) = default;
    callback_iterator_t& operator=(callback_iterator_t x) {
      swap(*this, x);
      return *this;
    }
    template<typename Arg>
    const callback_iterator_t& operator=(Arg&& arg) const {
      func_m(std::forward<Arg>(arg));
      return *this;
    }
    friend void swap(callback_iterator_t& a, callback_iterator_t& b) {
      using std::swap;
      swap(a.func_m, b.func_m);
    }
    const callback_iterator_t& operator++() const { return *this; }
    const callback_iterator_t& operator++(int) const { return *this; }
    const callback_iterator_t& operator*() const { return *this; }
  protected:
    Func func_m;
  }; // callback_iterator_t

  template<typename Func>
  callback_iterator_t<Func> callback_iterator(Func&& func) {
    return callback_iterator_t<Func>(std::forward<Func>(func));
  }
    
} // com_masaers

/******************************************************************************/
#endif
