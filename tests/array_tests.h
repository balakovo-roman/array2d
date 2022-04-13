#ifndef MAIN_TESTS_ARRAY_TESTS_H_
#define MAIN_TESTS_ARRAY_TESTS_H_

#include <gtest/gtest.h>

#include <array_2d.h>

TEST(array2d, good_constructor) {
  // Arrange
  Array2d<int, 2, 3> a1;
  Array2d<int, 4, 4> a2(42);
  Array2d<int, 2, 2> a3{1, 2, 3, 4};
  Array2d<int, 3, 3> a4{1, 2, 3, 4, 5};

  std::ostringstream result;

  // Act
  result << a1;
  // Assert
  ASSERT_EQ(result.str(), std::string{"0 0 0\n0 0 0"});

  // Act
  result.str({});
  result << a2;
  // Assert
  ASSERT_EQ(result.str(), std::string{"42 42 42 42\n42 42 42 42\n42 42 42 42\n42 42 42 42"});

  // Act
  result.str({});
  result << a3;
  // Assert
  ASSERT_EQ(result.str(), std::string{"1 2\n3 4"});

  // Act
  result.str({});
  result << a3;
  // Assert
  ASSERT_EQ(result.str(), std::string{"1 2\n3 4"});

  // Act
  result.str({});
  result << a4;
  // Assert
  ASSERT_EQ(result.str(), std::string{"1 2 3\n4 5 0\n0 0 0"});
}

TEST(array2d, bad_constructor) {
  // Assert
  ASSERT_THROW({ (Array2d<int, 2, 2>{1, 2, 3, 4, 5, 6}); }, std::logic_error);

  try {
	// Act
	Array2d<int, 2, 2> a{1, 2, 3, 4, 5, 6};
  } catch (const std::logic_error &ex) {
	// Assert
	ASSERT_EQ(std::string{ex.what()}, std::string{"Size of initializer_list [6] greater than size of Array2d [4]"});
  }
}

TEST(array2d, size) {
  // Arrange
  constexpr std::size_t kRows = 2;
  constexpr std::size_t kColumns = 3;

  Array2d<int, kRows, kColumns> a{1, 2, 0, 4, 5, -6};

  // Assert
  ASSERT_EQ(a.size(1), kRows);
  ASSERT_EQ(a.size(2), kColumns);
  ASSERT_THROW({ a.size(3); }, std::out_of_range);
  try {
	// Act
	a.size(3);
  } catch (const std::out_of_range &ex) {
	// Assert
	ASSERT_EQ(std::string{ex.what()}, std::string{"Chunk is out of range"});
  }
}

TEST(array2d, empty) {
  // Arrange
  Array2d<int, 5, 23> a{1, 2, 0, 4, 5, -6};

  // Assert
  ASSERT_EQ(a.empty(), false);
  ASSERT_EQ(a.size(1), 5);
  ASSERT_EQ(a.size(2), 23);
  ASSERT_EQ((Array2d<std::string, 5, 0>().empty()), false);
  ASSERT_EQ((Array2d<std::string, 0, 10>().empty()), false);
  ASSERT_EQ((Array2d<std::string, 0, 0>().empty()), true);
}

TEST(array2d, fill) {
  // Arrange
  Array2d<int, 3, 1> a;
  Array2d<int, 3, 2> a2{1, 2, 3, 4};
  std::ostringstream result;

  // Act
  a.fill(-1);
  result << a;

  // Assert
  ASSERT_EQ(result.str(), std::string{"-1\n-1\n-1"});

  // Act
  a2.fill(-1);
  result.str({});
  result << a2;

  // Assert
  ASSERT_EQ(result.str(), std::string{"-1 -1\n-1 -1\n-1 -1"});
}

TEST(array2d, swap) {
  // Arrange
  Array2d<int, 2, 2> a{1, 2, 3, 4};
  Array2d<int, 2, 2> a2(-1);
  std::ostringstream result;

  // Act
  a.swap(a2);

  // Assert
  result << a;
  ASSERT_EQ(result.str(), std::string{"-1 -1\n-1 -1"});

  result.str({});
  result << a2;
  ASSERT_EQ(result.str(), std::string{"1 2\n3 4"});
}

TEST(array2d, index) {
  // Arrange
  Array2d<int, 2, 2> a{1, 2, 3, 4};

  // Assert
  ASSERT_EQ(a[1][1], 4);

  // Act
  auto &item = a[0][1];
  item = -99;

  // Assert
  std::ostringstream result;
  result << a;
  ASSERT_EQ(result.str(), std::string{"1 -99\n3 4"});
}

TEST(array2d, iterators) {
  // Arrange
  Array2d<int, 3, 3> a{1, 2, 3, 4, 5, 6, 7, 8, 9};
  std::ostringstream result;

  // Act
  std::copy(std::begin(a), std::end(a), std::ostream_iterator<int>(result, " "));

  // Assert
  ASSERT_EQ(result.str(), std::string{"1 2 3 4 5 6 7 8 9 "});
}

TEST(array2d, const_iterators) {
  // Arrange
  Array2d<int, 3, 3> a{1, 2, 3, 4, 5, 6, 7, 8, 9};
  std::ostringstream result;

  // Act
  std::copy(std::cbegin(a), std::cend(a), std::ostream_iterator<int>(result, " "));

  // Assert
  ASSERT_EQ(result.str(), std::string{"1 2 3 4 5 6 7 8 9 "});

  // Act
  auto get_by_const_iterator = [](decltype(std::cbegin(a)) it){
	return *it;
  };

  auto not_const_iterator = a.begin() + 4;

  // Assert
  ASSERT_EQ(get_by_const_iterator(not_const_iterator), 5);
}

#endif //MAIN_TESTS_ARRAY_TESTS_H_
