#ifndef FILTER_RANGE_HPP
#define FILTER_RANGE_HPP
#include "range_util.hpp"


template<typename range_T, typename pred_T>
class filter_range {
private:
  class iterator_t;
  friend class iterator_t;
public:
  typedef typename range_traits<range_T>::iterator_type range_iterator;
  typedef iterator_t iterator;
  inline filter_range(range_T& range, pred_T pred)
    : range_m(&range), pred_m(pred)
  {}
  inline iterator cbegin() const {
    return iterator(*this, range_m->begin());
  }
  inline iterator cend() const {
    return iterator(*this, range_m->end());
  }
  inline iterator begin() const { return cbegin(); }
  inline iterator end() const { return cend(); }
private:
  inline bool valid_iterator_b(const range_iterator& it) const {
    return it == range_m->end() || pred_m(*it);
  }
  range_T* range_m;
  pred_T pred_m;
}; // filter_range


template<typename range_T, typename pred_T>
class filter_range<range_T, pred_T>::iterator_t
  : public default_range_iterator<range_T>::type
{
private:
  typedef typename default_range_iterator<range_T>::type base_type;
public:
  inline iterator_t(const filter_range& range, range_iterator at)
    : range_m(&range), at_m(at)
  { validate(); }
  inline iterator_t(const iterator_t&) = default;
  inline iterator_t& operator=(const iterator_t& x) {
    range_m = x.range_m;
    at_m = x.at_m;
    return *this;
  }
  inline iterator_t& operator++() {
    ++at_m;
    validate();
    return *this;
  }
  inline iterator_t operator++(int) {
    iterator_t result(*this);
    ++*this;
    return result;
  }
  inline typename base_type::reference operator*() const { return at_m.operator*(); }
  inline typename base_type::pointer operator->() const { return at_m.operator->(); }
  inline bool operator==(const iterator_t& x) const {
    return range_m == x.range_m && at_m == x.at_m;
  }
  inline bool operator!=(const iterator_t& x) const { return ! operator==(x); }
private:
  inline void validate() {
    while (! range_m->valid_iterator_b(at_m)) {
      ++at_m;
    }
  }
  const filter_range* range_m;
  range_iterator at_m;
}; // filter_range::iterator_t


template<typename range_T, typename pred_T>
filter_range<range_T, typename std::decay<pred_T>::type>
make_filter_range(range_T& range, pred_T&& pred) {
  return filter_range<range_T, typename std::decay<pred_T>::type>(range, std::forward<pred_T>(pred));
}

#endif
