#include "../struct/LinkedList/doublelist.h"
#include <gtest/gtest.h>
/*прямой и обратный вывод, вставку до и после 
значения, индексы у краёв, удаление head и tail, слу
чай единственного узла и переход после очистки. */
TEST(DoubleList, AddEverywhere) {
  DoubleList list;
  list.LAddEnd("2");
  list.LAddFirst("0");
  list.LAddBefore("2", "1");
  list.LAddAfter("2", "3");
  list.LAddIndex("4", 4); // index == длина - в конец
  list.LAddIndex("-1", 0);
  EXPECT_EQ(list.LGet(0), "-1");
  EXPECT_EQ(list.LGet(2), "1");
  EXPECT_EQ(list.LGet(5), "4");
  EXPECT_TRUE(list.LFind("3"));
  EXPECT_FALSE(list.LFind("9"));
}

TEST(DoubleList, DeleteEverywhere) {
  DoubleList list;
  for (string s : {"a", "b", "c", "d", "e"}) {
    list.LAddEnd(s);
  }
  EXPECT_EQ(list.LDeleteFirst(), "a");
  EXPECT_EQ(list.LDeleteEnd(), "e");
  EXPECT_EQ(list.LDeleteAfter("b"), "c");
  EXPECT_EQ(list.LDeleteBefore("d"), "b");
  EXPECT_EQ(list.LDeleteValue("d"), "d");
  EXPECT_THROW(list.LDeleteFirst(), runtime_error);
  EXPECT_THROW(list.LDeleteEnd(), runtime_error);
}

// после удалений head и tail должны оставаться правильными
TEST(DoubleList, HeadTailConsistency) {
  DoubleList list;
  list.LAddEnd("x");
  EXPECT_EQ(list.LDeleteIndex(0), "x");
  list.LAddEnd("y"); // tail обнулился, вставка в пустой работает
  list.LAddFirst("w");
  testing::internal::CaptureStdout();
  list.LPrintBack();
  EXPECT_EQ(testing::internal::GetCapturedStdout(), "y\nw\n");
}

TEST(DoubleList, Errors) {
  DoubleList list;
  list.LAddEnd("x");
  EXPECT_THROW(list.LAddAfter("nope", "y"), runtime_error);
  EXPECT_THROW(list.LAddBefore("nope", "y"), runtime_error);
  EXPECT_THROW(list.LAddIndex("y", 5), runtime_error);
  EXPECT_THROW(list.LAddIndex("y", -1), runtime_error);
  EXPECT_THROW(list.LDeleteAfter("x"), runtime_error);
  EXPECT_THROW(list.LDeleteBefore("x"), runtime_error);
  EXPECT_THROW(list.LDeleteIndex(3), runtime_error);
  EXPECT_THROW(list.LDeleteValue("nope"), runtime_error);
  EXPECT_THROW(list.LGet(1), runtime_error);
}

TEST(DoubleList, PrintAndClear) {
  DoubleList list;
  list.LAddEnd("a");
  list.LAddEnd("b");
  testing::internal::CaptureStdout();
  list.LPrint();
  EXPECT_EQ(testing::internal::GetCapturedStdout(), "a\nb\n");
  list.LClear();
  EXPECT_THROW(list.LGet(0), runtime_error);
}

TEST(DoubleList, Serialization) {
  DoubleList list1;
  list1.LAddEnd("a");
  list1.LAddEnd("b");
  list1.LAddEnd("c");
  list1.Save("test_dlist.bin");

  DoubleList list2;
  list2.Load("test_dlist.bin");
  EXPECT_EQ(list2.LGet(0), "a");
  EXPECT_EQ(list2.LGet(1), "b");
  EXPECT_EQ(list2.LGet(2), "c");
  EXPECT_TRUE(list2.LFind("b"));
  EXPECT_FALSE(list2.LFind("z"));

  remove("test_dlist.bin");
}