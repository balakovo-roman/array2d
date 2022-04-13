#ifndef ARRAY2D_ARRAY2D_ARRAY_2D_H_
#define ARRAY2D_ARRAY2D_ARRAY_2D_H_

#include <sstream>
#include <stdexcept>
#include <vector>

template<typename T, std::size_t R, std::size_t C>
class Array2d;

template<typename T, bool Const>
class Array2dIterator {
  template<typename, std::size_t R, std::size_t C>
  friend
  class Array2d;

  friend class Array2dIterator<T, !Const>;

 public:
  using iterator_category = std::random_access_iterator_tag;
  using value_type = T;
  using difference_type = std::ptrdiff_t;
  using pointer = std::conditional_t<Const, const value_type *, value_type *>;
  using reference = std::conditional_t<Const, const value_type &, value_type &>;

  reference operator*() const noexcept { return *ptr_; }

  auto &operator++() noexcept {
	++ptr_;
	return *this;
  }

  auto operator++(int) noexcept {
	auto result = *this;
	++(*this);
	return result;
  }

  template<bool R>
  bool operator==(const Array2dIterator<T, R> &other) const noexcept { return ptr_ == other.ptr_; }

  template<bool R>
  bool operator!=(const Array2dIterator<T, R> &other) const noexcept { return ptr_ != other.ptr_; }

  friend difference_type operator-(const Array2dIterator &lhs, const Array2dIterator &rhs) noexcept {
	return std::distance(rhs.ptr_, lhs.ptr_);
  }

  friend Array2dIterator operator+(const Array2dIterator &lhs, difference_type n) noexcept {
	return Array2dIterator(lhs.ptr_ + n);
  }

  operator Array2dIterator<T, true>() const noexcept { return Array2dIterator<T, true>{ptr_}; }

 private:
  explicit Array2dIterator(pointer p) noexcept: ptr_(p) {}

  pointer ptr_;
};

template<typename T, std::size_t R, std::size_t C>
class Array2d {

  template<typename It>
  class IndexProxy {
   public:
	explicit IndexProxy(It it) : iter_(it) {}

	typename It::reference operator[](const size_t c) {
	  return *(iter_ + c);
	}

	typename It::reference operator[](const size_t c) const {
	  return *(iter_ + c);
	}

   private:
	It iter_;
  };

 public:

  using value_type = T;
  using iterator = Array2dIterator<T, false>;
  using const_iterator = Array2dIterator<T, true>;
  using reference = value_type &;
  using const_reference = const value_type &;
  using pointer = T *;
  using const_pointer = const T *;

  Array2d() : arr_(R * C) {}
  explicit Array2d(const T &value) : arr_(R * C, value) {}
  Array2d(std::initializer_list<T> list) {
	constexpr auto real_size{R * C};

	if (list.size() > real_size) {
	  std::ostringstream err;
	  err << "Size of initializer_list [" << list.size() << "] greater than size of Array2d [" << real_size << ']';

	  throw std::logic_error{err.str()};
	}

	arr_.assign(list);

	if (arr_.size() != real_size)
	  arr_.insert(arr_.end(), real_size - arr_.size(), T{});
  }

  pointer data() noexcept { return arr_.data(); }
  const_pointer data() const noexcept { return arr_.data(); }

  reference at(const std::size_t r, const std::size_t c) { return arr_.at(r * C + c); }
  const_reference at(const std::size_t r, const std::size_t c) const { return arr_.at(r * C + c); }

  auto operator[](const std::size_t r) noexcept { return IndexProxy<Array2dIterator<T, false>>{begin() + r * C}; }
  auto operator[](const std::size_t r) const noexcept { return IndexProxy<Array2dIterator<T, true>>{begin() + r * C}; }

  bool empty() const noexcept { return C == 0 && R == 0; }

  std::size_t size(const int chunk) const {
	if (chunk == 1) return R;
	if (chunk == 2) return C;
	throw std::out_of_range("Chunk is out of range");
  }

  void fill(const T &value) { std::fill(begin(), end(), value); }

  void swap(Array2d &other) noexcept { arr_.swap(other.arr_); }

  const_iterator begin() const noexcept { return const_iterator{arr_.data()}; }
  const_iterator end() const noexcept { return const_iterator{arr_.data() + arr_.size()}; }

  iterator begin() noexcept { return iterator{arr_.data()}; }
  iterator end() noexcept { return iterator{arr_.data() + arr_.size()}; }

  friend std::ostream &operator<<(std::ostream &os, const Array2d &array_2d) {
	for (auto row = 0; row != R; ++row) {
	  for (auto column = 0; column != C; ++column) {
		os << array_2d[row][column];
		if (column + 1 != C) os << ' ';
	  }
	  if (row + 1 != R) os << '\n';
	}
	return os;
  }

 private:
  std::vector<T> arr_;
};

#endif //ARRAY2D_ARRAY2D_ARRAY_2D_H_
