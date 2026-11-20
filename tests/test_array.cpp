#include "../struct/Array/array.h"
#include <gtest/gtest.h>
/*  увеличение ёмкости, вставку в середину и
 конец, удаление первого элемента, замену, длину
, очистку и повторное использование объекта.*/
TEST(Array, AddGetResize) {
  Array arr(1);
  arr.MAddEnd("a");
  arr.MAddEnd("b"); // MResize
  arr.MAddEnd("c");
  EXPECT_EQ(arr.MLength(), 3);
  EXPECT_EQ(arr.MGet(0), "a");
  EXPECT_EQ(arr.MGet(2), "c");
}

TEST(Array, InsertDeleteReplace) {
  Array arr;
  arr.MAddEnd("a");
  arr.MAddEnd("c");
  arr.MAddIndex("b", 1);
  arr.MAddIndex("d", 3); // в конец
  EXPECT_EQ(arr.MGet(1), "b");
  EXPECT_EQ(arr.MGet(3), "d");
  EXPECT_EQ(arr.MDeleteIndex(0), "a");
  EXPECT_EQ(arr.MGet(0), "b");
  arr.MReplace("x", 0);
  EXPECT_EQ(arr.MGet(0), "x");
  EXPECT_EQ(arr.MLength(), 3);
}

TEST(Array, Errors) {
  Array arr;
  arr.MAddEnd("a");
  EXPECT_THROW(arr.MGet(1), runtime_error);
  EXPECT_THROW(arr.MGet(-1), runtime_error);
  EXPECT_THROW(arr.MAddIndex("x", 5), runtime_error);
  EXPECT_THROW(arr.MDeleteIndex(3), runtime_error);
  EXPECT_THROW(arr.MReplace("x", 1), runtime_error);
}

TEST(Array, ClearAndReuse) {
  Array arr;
  arr.MAddEnd("a");
  arr.MClear();
  EXPECT_EQ(arr.MLength(), 0);
  arr.MAddEnd("b"); // в ЛР1 здесь был бы выход за границу
  EXPECT_EQ(arr.MGet(0), "b");
}

TEST(Array, Print) {
  Array arr;
  arr.MAddEnd("a");
  arr.MAddEnd("b");
  testing::internal::CaptureStdout();
  arr.MPrint();
  EXPECT_EQ(testing::internal::GetCapturedStdout(), "a b \n");
}

TEST(Array, Serialization) {
  Array arr1;
  arr1.MAddEnd("hello");
  arr1.MAddEnd("world");
  arr1.Save("test_arr.bin");

  Array arr2;
  arr2.Load("test_arr.bin");
  EXPECT_EQ(arr2.MLength(), 2);
  EXPECT_EQ(arr2.MGet(0), "hello");
  EXPECT_EQ(arr2.MGet(1), "world");

  remove("test_arr.bin");
}