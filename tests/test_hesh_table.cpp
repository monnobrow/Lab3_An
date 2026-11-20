#include "../struct/hash/hesh_table.h"
#include <gtest/gtest.h>
/*добавление, чтение, наличие, удаление и 
повторное добавление ключа, коллизии, ошибку 
отсутствующего ключа, 
очистку и сохранение с последующей загрузкой. */
TEST(HashTable, InsertGetUpdate) {
  HashTable table;
  EXPECT_TRUE(table.HInsert("name", "Ann"));
  EXPECT_TRUE(table.HInsert("city", "Msk"));
  EXPECT_FALSE(table.HInsert("name", "Anna")); // обновление, а не новый ключ
  EXPECT_EQ(table.HGet("name"), "Anna");
  EXPECT_EQ(table.HLength(), 2);
}

TEST(HashTable, Delete) {
  HashTable table;
  table.HInsert("a", "1");
  table.HInsert("b", "2");
  table.HDelete("a");
  EXPECT_FALSE(table.HContains("a"));
  EXPECT_TRUE(table.HContains("b"));
  EXPECT_THROW(table.HDelete("a"), runtime_error);
  EXPECT_THROW(table.HGet("a"), runtime_error);
}

// таблица из 1 ячейки: обязательно будут коллизии и несколько HResize
TEST(HashTable, CollisionsAndResize) {
  HashTable table(1);
  for (int i = 0; i < 100; ++i) {
    table.HInsert("key" + to_string(i), to_string(i));
  }
  EXPECT_EQ(table.HLength(), 100);
  for (int i = 0; i < 100; ++i) {
    EXPECT_EQ(table.HGet("key" + to_string(i)), to_string(i));
  }
}

// удаление из середины цепочки (previous != nullptr)
TEST(HashTable, DeleteFromChainMiddle) {
  HashTable table(1);
  table.HInsert("a", "1");
  table.HInsert("b", "2");
  table.HInsert("c", "3");
  table.HDelete("b");
  EXPECT_EQ(table.HGet("a"), "1");
  EXPECT_EQ(table.HGet("c"), "3");
  EXPECT_EQ(table.HLength(), 2);
}

TEST(HashTable, Clear) {
  HashTable table;
  table.HInsert("x", "1");
  table.HClear();
  EXPECT_EQ(table.HLength(), 0);
  EXPECT_FALSE(table.HContains("x"));
  table.HInsert("y", "2"); // после очистки таблица работает
  EXPECT_EQ(table.HGet("y"), "2");
}

TEST(HashTable, Print) {
  HashTable table;
  table.HInsert("k", "v");
  testing::internal::CaptureStdout();
  table.HPrint();
  EXPECT_EQ(testing::internal::GetCapturedStdout(), "k -> v\n");
}

TEST(HashTable, Serialization) {
  HashTable table1;
  table1.HInsert("name", "Alice");
  table1.HInsert("age", "25");
  table1.HInsert("city", "Moscow");
  table1.Save("test_ht.bin");

  HashTable table2;
  table2.Load("test_ht.bin");
  EXPECT_EQ(table2.HLength(), 3);
  EXPECT_EQ(table2.HGet("name"), "Alice");
  EXPECT_EQ(table2.HGet("age"), "25");
  EXPECT_EQ(table2.HGet("city"), "Moscow");
  EXPECT_TRUE(table2.HContains("name"));
  EXPECT_FALSE(table2.HContains("phone"));

  remove("test_ht.bin");
}