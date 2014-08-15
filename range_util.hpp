#ifndef RANGE_UTIL_HPP
#define RANGE_UTIL_HPP
#include <utility>
#include <type_traits>
#include <iterator>

///
/// \todo Figure out whether this guard is as tight as it should be.
///
#if __GNUC__ < 4 || (__GNUC__ == 4 && __GNUC_MINOR__ <= 5)
namespace std {
  template<typename container_T>
  auto begin(container_T&& container) -> decltype(container.begin()) {
    return container.begin();
  }
  template<typename container_T>
  auto end(container_T&& container) -> decltype(container.end()) {
    return container.end();
  }
}
#endif

template<typename iterator_T>
class range_t {
public:
  range_t() : first_m(), last_m() {}
  range_t(const range_t&) = default;
  range_t(range_t&&) = default;
  template<typename first_T, typename last_T>
  range_t(first_T&& first, last_T&& last)
    : first_m(std::forward<first_T>(first))
    , last_m(std::forward<last_T>(last))
  {}
  friend inline void swap(range_t& a, range_t& b) {
    using std::swap;
    swap(a.first_m, b.first_m);
    swap(a.last_m, b.last_m);
  }
  range_t& operator=(range_t x) {
    swap(*this, x);
    return *this;
  }
  iterator_T begin() const { return first_m; }
  iterator_T end() const { return last_m; }
private:
  iterator_T first_m;
  iterator_T last_m;
};

template<typename iterator_T>
range_t<typename std::decay<iterator_T>::type>
range(iterator_T&& first, iterator_T&& last) {
  return range_t<typename std::decay<iterator_T>::type>(std::forward<iterator_T>(first), std::forward<iterator_T>(last));
}

template<typename container_T>
range_t<typename container_T::iterator>
range(container_T& container) {
  return range_t<typename container_T::iterator>(std::begin(container),
						 std::end(container));
}

template<typename container_T>
range_t<typename container_T::const_iterator>
range(const container_T& container) {
  return range_t<typename container_T::const_iterator>(std::begin(container),
						       std::end(container));
}

template<typename iterator_T>
range_t<typename std::decay<iterator_T>::type>
range(std::pair<iterator_T, iterator_T>& pair) {
  return range_t<typename std::decay<iterator_T>::type>(pair.first,
							pair.second);
}

template<typename iterator_T>
range_t<typename std::decay<iterator_T>::type>
range(const std::pair<iterator_T, iterator_T>&& pair) {
  return range_t<typename std::decay<iterator_T>::type>(pair.first,
							pair.second);
}

template<typename iterator_T>
range_t<typename std::decay<iterator_T>::type>
range(std::pair<iterator_T, iterator_T>&& pair) {
  return range_t<typename std::decay<iterator_T>::type>(std::move(pair.first),
							std::move(pair.second));
}



template<typename range_T>
struct range_traits {
  typedef typename std::decay<decltype(begin(std::declval<range_T>()))>::type iterator_type;
  typedef typename std::iterator_traits<iterator_type>::difference_type difference_type;
  typedef typename std::iterator_traits<iterator_type>::value_type value_type;
  typedef typename std::iterator_traits<iterator_type>::pointer pointer;
  typedef typename std::iterator_traits<iterator_type>::reference reference;
  typedef typename std::iterator_traits<iterator_type>::iterator_category iterator_category;
};

template<typename range_T,
	 typename iterator_category_T = typename range_traits<range_T>::iterator_category>
struct default_range_iterator {
  typedef std::iterator<iterator_category_T,
			typename range_traits<range_T>::value_type,
			typename range_traits<range_T>::difference_type,
			typename range_traits<range_T>::pointer,
			typename range_traits<range_T>::reference> type;
};


#endif
