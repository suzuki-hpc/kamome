#ifndef KAMOME_TYPES_HPP
#define KAMOME_TYPES_HPP

#include <cmath>
#include <cstdint>
#include <type_traits>

#ifndef __is_identifier
#define __is_identifier(x) 1
#endif
#define _has_keyword(__x) !(__is_identifier(__x))

#define KAMOME_ENABLER(cond) std::enable_if_t<cond, std::nullptr_t> = nullptr

namespace kmm {

namespace impl {

template <class> struct is_fixed_point {
  static constexpr bool value = false;
};
template <class C>
inline constexpr bool is_fixed_point_v = is_fixed_point<C>::value;

template <typename val_t, typename mid_t, uint32_t> struct _fixed_t {
  inline static uint8_t bit;
  val_t val;

  constexpr _fixed_t() : val(0) {}
  constexpr _fixed_t(const _fixed_t &in) = default;
  // template <class C, KAMOME_ENABLER(is_fixed_point_v<C>)>
  // constexpr _fixed_t(const C &in)
  //     : val((bit < in.bit) ? in.val >> (in.bit - bit)
  //                          : in.val << (bit - in.bit)) {}

  template <typename T, KAMOME_ENABLER(std::is_integral_v<T>)>
  constexpr _fixed_t(const T in) : val(in << bit) {}
  template <typename T, KAMOME_ENABLER(std::is_floating_point_v<T>)>
  constexpr _fixed_t(const T in) : val(val_t(in * (1 << bit))) {}

  template <typename T, KAMOME_ENABLER(std::is_integral_v<T>)>
  constexpr operator T() const {
    return T(val) >> bit;
  }
  template <typename T, KAMOME_ENABLER(std::is_floating_point_v<T>)>
  constexpr operator T() const {
    return T(val) / (1 << bit);
  }
#if _has_keyword(__fp16)
//  constexpr operator __fp16() const { return __fp16(val) / (1 << bit); }
#endif
#if _has_keyword(__half)
//  constexpr operator __half() const { return __half(val) / (1 << bit); }
#endif

  template <typename T, KAMOME_ENABLER(std::is_arithmetic_v<T>)>
  constexpr _fixed_t operator+(const T in) const {
    return {val_t(val + _fixed_t(in).val), true};
  }
  template <typename T, KAMOME_ENABLER(std::is_arithmetic_v<T>)>
  constexpr _fixed_t operator-(const T in) const {
    return {val_t(val - _fixed_t(in).val), true};
  }

  constexpr _fixed_t operator+(const _fixed_t in) const {
    return {val_t(val + in.val), true};
  }
  constexpr _fixed_t operator-(const _fixed_t in) const {
    return {val_t(val - in.val), true};
  }
  constexpr _fixed_t operator*(const _fixed_t in) const {
    return {val_t(mid_t(val) * mid_t(in.val) >> bit), true};
  }
  constexpr void operator+=(const _fixed_t in) { val += in.val; }
  constexpr void operator-=(const _fixed_t in) { val -= in.val; }
  constexpr void operator*=(const _fixed_t in) {
    val = val_t(mid_t(val) * mid_t(in.val) >> bit);
  }

  val_t sqrt_eval() const { return std::sqrt(val); }
  val_t sqrt_odd() const { return std::sqrt(val * 1.41421356f); }

  static void change_point(const uint8_t _bit) { bit = _bit; }

private:
  constexpr _fixed_t(const val_t in, bool) : val(in) {}
};

template <typename a, typename b, uint32_t c>
struct is_fixed_point<_fixed_t<a, b, c>> {
  static constexpr bool value = true;
};

} // namespace impl

template <uint32_t _> using fixed8_t = impl::_fixed_t<int8_t, int16_t, _>;
template <uint32_t _> using fixed16_t = impl::_fixed_t<int16_t, int32_t, _>;
template <uint32_t _> using fixed32_t = impl::_fixed_t<int32_t, int64_t, _>;
template <uint32_t _> using fixed64_t = impl::_fixed_t<int64_t, int64_t, _>;

} // namespace kmm

#undef KAMOME_ENABLER

#endif 
