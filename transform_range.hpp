#ifndef TRANSFORM_RANGE_HPP
#define TRANSFORM_RANGE_HPP
#include "range_util.hpp"

template<typename range_T, typename func_T>
class transform_range_t {
private:
  class iterator_t;
  friend class iterator_t;
public:
  typedef typename range_traits<range_T>::iterator_type range_iterator;
  typedef typename std::result_of<func_T(typename range_traits<range_T>::reference)>::type reference;
  typedef typename std::decay<reference>::type value_type;
  
  typedef iterator_t iterator;
  transform_range_t(range_T& range, func_T func)
    : range_m(&range), func_m(std::move(func))
  {}
  transform_range_t(const transform_range_t&) = default;
  transform_range_t(transform_range_t&&) = default;
  friend void swap(transform_range_t& a, transform_range_t& b) {
    using namespace std;
    swap(a.range_m, b.range_m);
    swap(a.func_m, b.func_m);
  }
  transform_range_t& operator=(transform_range_t x) {
    using namespace std;
    swap(*this, x);
    return *this;
  }
  iterator begin() const { return cbegin(); }
  iterator end() const { return cend(); }
  iterator cbegin() const { return iterator(this, range_m->begin()); }
  iterator cend() const { return iterator(this, range_m->end()); }
private:
  range_T* range_m;
  func_T func_m;
};

template<typename range_T, typename func_T>
class transform_range_t<range_T, func_T>::iterator_t
  : public std::iterator<std::input_iterator_tag,
			 typename transform_range_t<range_T, func_T>::value_type,
			 typename transform_range_t<range_T, func_T>::reference>
{
public:
  inline iterator_t(const transform_range_t* range, range_iterator it)
    : range_m(range), it_m(it)
  {}
  inline iterator_t(const iterator_t&) = default;
  inline iterator_t(iterator_t&&) = default;
  inline iterator_t& operator=(iterator_t x) {
    using namespace std;
    swap(*this, x);
    return *this;
  }
  friend inline void swap(iterator_t& a, iterator_t& b) {
    using namespace std;
    swap(a.range_m, b.range_m);
    swap(a.it_m, b.it_m);
  }
  inline iterator_t& operator++() {
    ++it_m;
    return *this;
  }
  inline iterator_t operator++(int) {
    iterator_t x(*this);
    ++*this;
    return x;
  }
  inline typename transform_range_t<range_T, func_T>::reference operator*() const {
    return range_m->func_m(*it_m);
  }
  inline typename transform_range_t<range_T, func_T>::reference* operator->() const {
    return &range_m->func_m(*it_m);
  }
  inline bool operator==(const iterator_t& x) const {
    return range_m == x.range_m && it_m == x.it_m;
  }
  inline bool operator!=(const iterator_t& x) const {
    return ! operator==(x);
  }
private:
  const transform_range_t* range_m;
  range_iterator it_m;
};


template<typename range_T, typename func_T>
transform_range_t<range_T, typename std::decay<func_T>::type>
transform_range(range_T& range, func_T&& func) {
  return transform_range_t<range_T, typename std::decay<func_T>::type>(range, std::forward<func_T>(func));
}

#endif
