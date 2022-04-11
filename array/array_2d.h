#ifndef ARRAY2D_ARRAY2D_ARRAY_2D_H_
#define ARRAY2D_ARRAY2D_ARRAY_2D_H_

#include <stdexcept>
#include <vector>

template<typename T, std::size_t R, std::size_t C>
class Array2d {
 public:
  using value_type = T;
  using iterator = value_type *;
  using const_iterator = const value_type *;

  constexpr Array2d() : arr_(R * C) {}
  constexpr explicit Array2d(const T &value) : arr_(value, R * C) {}
  constexpr Array2d(std::initializer_list<T> list) {
	constexpr auto real_size = R * C;
	if (list.size() > real_size)
	  throw std::logic_error("Size of initializer_list greater than size Array2d");

	arr_.assign(list);

	if (arr_.size() != real_size)
	  arr_.insert(arr_.end(), real_size - arr_.size(), T{});
  }

  constexpr T *data() noexcept { return arr_.data(); }
  constexpr const T *data() const noexcept { return arr_.data(); }

  constexpr T &at(const std::size_t r, const std::size_t c) {
	return arr_.at(r * C + c);
  }

  constexpr const T &at(const std::size_t r, const std::size_t c) const {
	return arr_.at(r * C + c);
  }

  constexpr T &operator()(const std::size_t r, const std::size_t c) {
	return arr_[r * C + c];
  }

  constexpr const T &operator()(const std::size_t r, const std::size_t c) const {
	return arr_[r * C + c];
  }

  constexpr bool empty() const noexcept { return C == 0 || R == 0; }

  constexpr std::size_t size(const int rank) const {
	if (rank == 1) return R;
	if (rank == 2) return C;
	throw std::out_of_range("Rank is out of range!");
  }

  void fill(const T &value) {
	std::fill(std::begin(arr_), std::end(arr_), value);
  }

  void swap(Array2d &other) noexcept {
	arr_.swap(other.arr_);
  }

  const_iterator begin() const { return arr_.data(); }
  const_iterator end() const { return arr_.data() + arr_.size(); }

  iterator begin() { return arr_.data(); }
  iterator end() { return arr_.data() + arr_.size(); }

 private:
  std::vector<T> arr_;
};

#endif //ARRAY2D_ARRAY2D_ARRAY_2D_H_
