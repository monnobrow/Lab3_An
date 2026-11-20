#include "../struct/Stack/stack.h"
#include <gtest/gtest.h>
/*последовательное добавление и извлечение 
нескольких значений, проверку порядка LIFO/FIFO, 
просмотр края без удаления, повторное использование 
после очистки и 
ошибку попытки извлечь значение из пустой структуры.*/
TEST(Stack, PushPopLifo) {
  Stack stack;
  EXPECT_TRUE(stack.SIsEmptyStack());
  stack.SPush("a");
  stack.SPush("hello world");
  EXPECT_FALSE(stack.SIsEmptyStack());
  EXPECT_EQ(stack.STop(), "hello world");
  EXPECT_EQ(stack.SPop(), "hello world");
  EXPECT_EQ(stack.SPop(), "a");
  EXPECT_TRUE(stack.SIsEmptyStack());
}

TEST(Stack, EmptyErrors) {
  Stack stack;
  EXPECT_THROW(stack.SPop(), runtime_error);
  EXPECT_THROW(stack.STop(), runtime_error);
}

TEST(Stack, Clear) {
  Stack stack;
  stack.SPush("1");
  stack.SPush("2");
  stack.SClear();
  EXPECT_TRUE(stack.SIsEmptyStack());
}

TEST(Stack, Serialization) {
  Stack stack1;
  stack1.SPush("bottom");
  stack1.SPush("middle");
  stack1.SPush("top");
  stack1.Save("test_stack.bin");

  Stack stack2;
  stack2.Load("test_stack.bin");
  EXPECT_EQ(stack2.STop(), "top");
  EXPECT_EQ(stack2.SPop(), "top");
  EXPECT_EQ(stack2.SPop(), "middle");
  EXPECT_EQ(stack2.SPop(), "bottom");
  EXPECT_TRUE(stack2.SIsEmptyStack());

  remove("test_stack.bin");
}