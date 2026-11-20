#include "../struct/SingleList/singlList.h"
#include <gtest/gtest.h>
/* вставка чтение поиск удаление по индексу (значению) 
обратная печать заагрузка данные пустой список
выход индекса за границы  и тд */
TEST(SinglList, AddAndGet) {
  SinglList list;
  list.FAddEnd("1");
  list.FAddFirst("0");
  list.FAddAfter("1", "2");
  list.FAddBefore("0", "-1");
  EXPECT_EQ(list.FGet(0), "-1");
  EXPECT_EQ(list.FGet(3), "2");
  EXPECT_TRUE(list.FFind("1"));
  EXPECT_FALSE(list.FFind("9"));
}

TEST(SinglList, Delete) {
  SinglList list;
  list.FAddEnd("a");
  list.FAddEnd("b");
  list.FAddEnd("c");
  EXPECT_EQ(list.FDeleteBefore("b"), "a");
  EXPECT_EQ(list.FDeleteEnd(), "c");
  EXPECT_EQ(list.FDeleteFirst(), "b");
  EXPECT_THROW(list.FDeleteFirst(), runtime_error);
}

TEST(SinglList, Errors) {
  SinglList list;
  list.FAddEnd("x");
  EXPECT_THROW(list.FAddAfter("nope", "y"), runtime_error);
  EXPECT_THROW(list.FDeleteAfter("x"), runtime_error);
  EXPECT_THROW(list.FDeleteBefore("x"), runtime_error);
  EXPECT_THROW(list.FGet(5), runtime_error);
}

TEST(SinglList, IndexAndValue) {
  SinglList list;
  list.FAddIndex("b", 0);
  list.FAddIndex("d", 1);
  list.FAddIndex("c", 1);
  list.FAddIndex("a", 0); // a b c d
  EXPECT_THROW(list.FAddIndex("x", 9), runtime_error);
  EXPECT_THROW(list.FAddIndex("x", -1), runtime_error);
  EXPECT_EQ(list.FDeleteIndex(2), "c");
  EXPECT_EQ(list.FDeleteIndex(0), "a"); // b d
  EXPECT_THROW(list.FDeleteIndex(5), runtime_error);
  list.FAddEnd("e");
  list.FAddEnd("f"); // b d e f
  EXPECT_EQ(list.FDeleteAfter("d"), "e");
  EXPECT_EQ(list.FDeleteBefore("f"), "d");
  EXPECT_EQ(list.FDeleteValue("f"), "f");
  EXPECT_EQ(list.FDeleteValue("b"), "b");
  EXPECT_THROW(list.FDeleteValue("b"), runtime_error);
  EXPECT_THROW(list.FDeleteBefore("b"), runtime_error);
}

TEST(SinglList, DeleteEndAndBeforeErrors) {
  SinglList list;
  list.FAddEnd("1");
  EXPECT_EQ(list.FDeleteEnd(), "1"); // один элемент
  list.FAddEnd("1");
  list.FAddEnd("2");
  list.FAddEnd("3");
  EXPECT_EQ(list.FDeleteEnd(), "3"); // проход до предпоследнего
  EXPECT_THROW(list.FDeleteBefore("9"), runtime_error);
  EXPECT_THROW(list.FAddBefore("9", "x"), runtime_error);
  list.FAddBefore("2", "x");
  EXPECT_EQ(list.FGet(1), "x");
}

TEST(SinglList, PrintAndClear) {
  SinglList list;
  list.FAddEnd("a");
  list.FAddEnd("b");
  testing::internal::CaptureStdout();
  list.FPrint();
  EXPECT_EQ(testing::internal::GetCapturedStdout(), "a\nb\n");
  testing::internal::CaptureStdout();
  list.FPrintBack();
  EXPECT_EQ(testing::internal::GetCapturedStdout(), "b\na\n");
  testing::internal::CaptureStdout();
  list.FPrintAddresses();
  EXPECT_NE(testing::internal::GetCapturedStdout().find("chislo: a"),
            string::npos);
  list.FClear();
  EXPECT_THROW(list.FGet(0), runtime_error);
}

TEST(SinglList, Serialization) {
  SinglList list1;
  list1.FAddEnd("a");
  list1.FAddEnd("b");
  list1.Save("test_list.bin");

  SinglList list2;
  list2.Load("test_list.bin");
  EXPECT_EQ(list2.FGet(0), "a");
  EXPECT_EQ(list2.FGet(1), "b");

  remove("test_list.bin");
}