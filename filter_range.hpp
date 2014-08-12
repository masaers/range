#ifndef FILTER_RANGE_HPP
#define FILTER_RANGE_HPP
#include "range_util.hpp"

template<typename range_T, typename pred_T>
class filter_range {
public:
  typedef typename range_traits<range_T>::iterator_type range_iterator;
  class iterator_t : public default_range_iterator<range_T>::type {
    typedef typename default_range_iterator<range_T>::type base_type;
  public:
    iterator_t(const filter_range& range, range_iterator at)
      : range_m(&range), at_m(at)
    { validate(); }
    iterator_t(const iterator_t&) = default;
    iterator_t& operator=(const iterator_t& x) {
      range_m = x.range_m;
      at_m = x.at_m;
      return *this;
    }
    iterator_t& operator++() {
      ++at_m;
      validate();
      return *this;
    }
    iterator_t operator++(int) {
      iterator_t result(*this);
      ++*this;
      return result;
    }
    typename base_type::reference operator*() const { return at_m.operator*(); }
    typename base_type::pointer operator->() const { return at_m.operator->(); }
    bool operator==(const iterator_t& x) const {
      return range_m == x.range_m && at_m == x.at_m;
    }
    bool operator!=(const iterator_t& x) const { return ! operator==(x); }
  private:
    inline void validate() {
      while (! valid()) {
	++at_m;
      }
    }
    inline bool valid() const {
      return at_m == range_m->range_m->end() || range_m->pred_m(*at_m);
    }
    const filter_range* range_m;
    range_iterator at_m;
  };
  typedef iterator_t iterator;
  filter_range(range_T& range, pred_T pred)
    : range_m(&range), pred_m(pred)
  {}
  iterator cbegin() const {
    return iterator(*this, range_m->begin());
  }
  iterator cend() const {
    return iterator(*this, range_m->end());
  }
  iterator begin() const { return cbegin(); }
  iterator end() const { return cend(); }
private:
  range_T* range_m;
  pred_T pred_m;
};

template<typename range_T, typename pred_T>
filter_range<range_T, typename std::decay<pred_T>::type>
make_filter_range(range_T& range, pred_T&& pred) {
  return filter_range<range_T, typename std::decay<pred_T>::type>(range, std::forward<pred_T>(pred));
}

#endif
