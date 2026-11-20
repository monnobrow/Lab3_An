#include "../struct/Queue/queue.h"
#include <gtest/gtest.h>

TEST(Queue, PushPopFifo) {
  Queue q;
  q.QPush("1");
  q.QPush("2");
  q.QPush("3");
  EXPECT_EQ(q.QFront(), "1");
  EXPECT_EQ(q.QBack(), "3");
  EXPECT_EQ(q.QPop(), "1");
  EXPECT_EQ(q.QPop(), "2");
  EXPECT_EQ(q.QFront(), "3");
}

TEST(Queue, TailResetAfterLastPop) {
  Queue q;
  q.QPush("a");
  EXPECT_EQ(q.QPop(), "a");
  EXPECT_THROW(q.QBack(), runtime_error);
  q.QPush("b");
  EXPECT_EQ(q.QFront(), "b");
  EXPECT_EQ(q.QBack(), "b");
}

TEST(Queue, EmptyErrors) {
  Queue q;
  EXPECT_THROW(q.QPop(), runtime_error);
  EXPECT_THROW(q.QFront(), runtime_error);
  EXPECT_THROW(q.QBack(), runtime_error);
}

TEST(Queue, Clear) {
  Queue q;
  q.QPush("x");
  q.QPush("y");
  q.QClear();
  EXPECT_THROW(q.QPop(), runtime_error);
}

TEST(Queue, Serialization) {
  Queue q1;
  q1.QPush("first");
  q1.QPush("second");
  q1.QPush("third");
  q1.Save("test_queue.bin");

  Queue q2;
  q2.Load("test_queue.bin");
  EXPECT_EQ(q2.QFront(), "first");
  EXPECT_EQ(q2.QBack(), "third");
  EXPECT_EQ(q2.QPop(), "first");
  EXPECT_EQ(q2.QPop(), "second");
  EXPECT_EQ(q2.QPop(), "third");

  remove("test_queue.bin");
}