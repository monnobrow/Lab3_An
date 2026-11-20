#include "../struct/Tree/tree.h"
#include <gtest/gtest.h>

void FillExample(Tree &tree) {
  const int values[] = {10, 20, 30, 25, 27, 5, 4};
  for (int v : values) {
    tree.TAdd(v);
  }
}

TEST(Tree, Traversals) {
  Tree tree;
  FillExample(tree);

  testing::internal::CaptureStdout();
  tree.TPrintPryam();
  EXPECT_EQ(testing::internal::GetCapturedStdout(), "20 5 4 10 27 25 30 ");

  testing::internal::CaptureStdout();
  tree.TPrintSim();
  EXPECT_EQ(testing::internal::GetCapturedStdout(), "4 5 10 20 25 27 30 ");

  testing::internal::CaptureStdout();
  tree.TPrintObr();
  EXPECT_EQ(testing::internal::GetCapturedStdout(), "4 10 5 25 30 27 20 ");

  testing::internal::CaptureStdout();
  tree.TPrintBfs();
  EXPECT_EQ(testing::internal::GetCapturedStdout(), "20 5 27 4 10 25 30 ");

  EXPECT_EQ(tree.THeight(), 3);
}

TEST(Tree, RotationRR) {
  Tree tree;
  tree.TAdd(10);
  tree.TAdd(20);
  tree.TAdd(30); // левый поворот вокруг 10
  testing::internal::CaptureStdout();
  tree.TPrintBfs();
  EXPECT_EQ(testing::internal::GetCapturedStdout(), "20 10 30 ");
}

TEST(Tree, DeleteWithRL) {
  Tree tree;
  FillExample(tree);
  tree.TDelete(30);
  tree.TDelete(4);
  tree.TDelete(10);
  tree.TDelete(5); // случай RL, новый корень 25
  testing::internal::CaptureStdout();
  tree.TPrintBfs();
  EXPECT_EQ(testing::internal::GetCapturedStdout(), "25 20 27 ");
}

TEST(Tree, FindAndErrors) {
  Tree tree;
  FillExample(tree);
  EXPECT_TRUE(tree.TFind(27));
  EXPECT_FALSE(tree.TFind(99));
  EXPECT_THROW(tree.TAdd(27), runtime_error);
  EXPECT_THROW(tree.TDelete(99), runtime_error);
}

TEST(Tree, Clear) {
  Tree tree;
  FillExample(tree);
  tree.TClear();
  EXPECT_EQ(tree.THeight(), 0);
  EXPECT_FALSE(tree.TFind(10));
}

TEST(Tree, Serialization) {
  Tree tree1;
  tree1.TAdd(10);
  tree1.TAdd(5);
  tree1.TAdd(20);
  tree1.Save("test_tree.bin");

  Tree tree2;
  tree2.Load("test_tree.bin");
  EXPECT_TRUE(tree2.TFind(10));
  EXPECT_TRUE(tree2.TFind(5));
  EXPECT_TRUE(tree2.TFind(20));
  EXPECT_EQ(tree2.THeight(), 2);

  remove("test_tree.bin");
}