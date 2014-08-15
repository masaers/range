#ifndef ZIP_RANGE_HPP
#define ZIP_RANGE_HPP
#include "range_util.hpp"
#include <tuple>


///
/// \namespace Mainly contains functions to provide SFINAE
/// functionality needed by <code>zip_iterator</code>s.
///
namespace zip_iterator_helper {
  //
  // The below section should be in a functional header somewhere
  // else, in a different module.
  //
  template<int...> struct seq {};
  template<int N, int... S> struct gen_seq : gen_seq<N-1, N-1, S...> {};
  template<int... S> struct gen_seq<0, S...> { typedef seq<S...> type; };
  template<typename... _T>
  inline auto make_seq() -> typename gen_seq<sizeof...(_T)>::type {
    return typename gen_seq<sizeof...(_T)>::type();
  }
  template<typename... _T>
  inline auto make_seq(const std::tuple<_T...>&) -> typename gen_seq<sizeof...(_T)>::type {
    return typename gen_seq<sizeof...(_T)>::type();
  }
  

  //
  // Recursive increment functionality for <code>zip_iterators</code>.
  //
  // Base case
  template<int I, typename... it_T>
  inline typename std::enable_if<(I == sizeof...(it_T)), void>::type
  increment_iterator_tuple(std::tuple<it_T...>& its) {}
  // Inductive case
  template<int I, typename... it_T>
  inline typename std::enable_if<(I != sizeof...(it_T)), void>::type
  increment_iterator_tuple(std::tuple<it_T...>& its) {
    ++std::get<I>(its);
    increment_iterator_tuple<I + 1>(its);
  }
  

  //
  // Dereference a tuple of iterators to a tuple of references.
  //
  template<typename it_tuple_T, int... it_S>
  inline auto
  dereference_iterator_tuple(it_tuple_T&& it_tuple, seq<it_S...>)
    -> decltype(std::forward_as_tuple((*std::get<it_S>(it_tuple))...)) {
    return std::forward_as_tuple((*std::get<it_S>(it_tuple))...);
  }


  //
  // Recursive inequality function for <code>zip_iterators</code>.
  //
  // Base case
  template<int I, typename... it_T>
  inline typename std::enable_if<(I == sizeof...(it_T)), bool>::type
  iterator_tuple_inequality(const std::tuple<it_T...>& a,
			    const std::tuple<it_T...>& b) {
    return true;
  }
  // Inductive case
  template<int I, typename... it_T>
  inline typename std::enable_if<(I != sizeof...(it_T)), bool>::type
  iterator_tuple_inequality(const std::tuple<it_T...>& a,
			    const std::tuple<it_T...>& b) {
    return std::get<I>(a) != std::get<I>(b)
      &&   iterator_tuple_inequality<I + 1>(a, b);
  }
  
} // namespace zip_iterator_helper


///
/// The zip iterator type
///
/// \todo Implement swap and change the assignemnt operator to fit the
/// copy-swap idiom.
///
template<typename... it_T>
class zip_iterator_t : public std::iterator<std::input_iterator_tag, std::tuple<typename std::iterator_traits<it_T>::reference...> > {
  typedef std::tuple<it_T...> its_type;
public:
  typedef std::tuple<typename std::iterator_traits<it_T>::reference...> reference;
  typedef std::tuple<typename std::iterator_traits<it_T>::value_type...> value_type;
  ///
  /// \name Constructors and destructors
  /// \{
  // ------------------------------------------------------------------------ //
  ///
  /// Construction from existing iterators
  ///
  zip_iterator_t(it_T&&... it) : its_m(std::forward<it_T>(it)...) {}
  ///
  /// Copy constructor
  ///
  zip_iterator_t(const zip_iterator_t&) = default;
  ///
  /// Move constructor
  ///
  zip_iterator_t(zip_iterator_t&&) = default;
  // ------------------------------------------------------------------------ //
  /// \}

  ///
  /// \name Overloaded operators
  /// \{
  // ------------------------------------------------------------------------ //
  ///
  /// Assignment operator
  ///
  zip_iterator_t& operator=(const zip_iterator_t& x) {
    its_m = x.its_m;
    return *this;
  }
  ///
  /// Preincrement operator
  ///
  zip_iterator_t& operator++() {
    zip_iterator_helper::increment_iterator_tuple<0>(its_m);
    return *this;
  }
  ///
  /// Postincrement operator
  ///
  zip_iterator_t operator++(int) {
    zip_iterator_t x(*this);
    ++*this;
    return x;
  }
  ///
  /// Equality operator
  ///
  bool operator==(const zip_iterator_t& x) const {
    return its_m == x.its_m;
  }
  ///
  /// Inequality operator
  ///
  /// Note that this behaves <q>unexpected</q>, as <em>all</em>
  /// iterators have to be different from each other for the
  /// <code>zip_iteratos</code> to be different. This is neccessary to
  /// have loops work as expected, without one iterator going passed
  /// the end because another has elemetns left.
  ///
  bool operator!=(const zip_iterator_t& x) const {
    return zip_iterator_helper::iterator_tuple_inequality<0>(its_m, x.its_m);
  }
  decltype(zip_iterator_helper::dereference_iterator_tuple(std::declval<its_type>(), zip_iterator_helper::make_seq<it_T...>()))
  operator*() {
    return zip_iterator_helper::dereference_iterator_tuple(its_m, zip_iterator_helper::make_seq<it_T...>());
  }
  // ------------------------------------------------------------------------ //
  /// \}
private:
  its_type its_m;
}; // zip_iterator_t



///
/// Convenience function for constructing a zip_iterator.
///
template<typename... it_T>
zip_iterator_t<typename std::decay<it_T>::type...>
make_zip_iterator(///
		  /// The iterators to zip up.
		  ///
		  it_T&&... it) {
  return zip_iterator_t<typename std::decay<it_T>::type...>(std::forward<it_T>(it)...);
}


///
/// Convenience function for constructing a zipped range.
///
template<typename... ranges_T>
auto make_zip_range(///
		    /// The ranges to zip up.
		    ///
		    ranges_T&&... ranges)
  -> decltype(range(make_zip_iterator(std::begin(ranges)...),
		    make_zip_iterator(std::end(ranges)...))) {
  return range(make_zip_iterator(std::begin(ranges)...),
	       make_zip_iterator(std::end(ranges)...));
}

#endif
