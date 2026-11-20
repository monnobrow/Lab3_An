#include "../struct/hash/open_table.h"
#include <gtest/gtest.h>

TEST(OpenTable, LinearInsertGetUpdate) {
  OpenTable table;
  EXPECT_TRUE(table.OInsert("name", "Ann"));
  EXPECT_FALSE(table.OInsert("name", "Anna"));
  EXPECT_EQ(table.OGet("name"), "Anna");
  EXPECT_EQ(table.OLength(), 1);
  EXPECT_THROW(table.OGet("nope"), runtime_error);
}

// свёртка при mesto = 11: "1" -> 1, "12" -> 12 % 11 = 1 - коллизия
TEST(OpenTable, DeletedMarkKeepsSearch) {
  OpenTable table(11, false, true);
  table.OInsert("1", "a");
  table.OInsert("12", "b"); // лёг в ячейку 2
  table.ODelete("1");       // ячейка 1 стала "deleted", а не "empty"
  EXPECT_FALSE(table.OContains("1"));
  EXPECT_EQ(table.OGet("12"), "b"); // поиск прошёл через метку
  EXPECT_THROW(table.ODelete("1"), runtime_error);
}

// свёртка длинного ключа и откат к обычному хешу для не-числа
TEST(OpenTable, FoldingKeys) {
  OpenTable table(11, false, true);
  table.OInsert("523456795", "fold");
  table.OInsert("abc", "text");
  EXPECT_EQ(table.OGet("523456795"), "fold");
  EXPECT_EQ(table.OGet("abc"), "text");
}

// все три режима: много ключей, несколько перестроений
TEST(OpenTable, RehashAllModes) {
  const bool modes[3][2] = {{false, false}, {true, false}, {false, true}};
  for (int m = 0; m < 3; ++m) {
    OpenTable table(3, modes[m][0], modes[m][1]);
    for (int i = 0; i < 200; ++i) {
      table.OInsert(to_string(i * 7), to_string(i));
    }
    EXPECT_EQ(table.OLength(), 200);
    for (int i = 0; i < 200; ++i) {
      EXPECT_EQ(table.OGet(to_string(i * 7)), to_string(i));
    }
  }
}

// много удалений: перестройка без роста должна убрать метки
TEST(OpenTable, ManyDeletes) {
  OpenTable table(11);
  for (int i = 0; i < 100; ++i) {
    table.OInsert("k" + to_string(i), "v");
    table.ODelete("k" + to_string(i));
  }
  EXPECT_EQ(table.OLength(), 0);
  table.OInsert("last", "ok");
  EXPECT_EQ(table.OGet("last"), "ok");
}

TEST(OpenTable, Print) {
  OpenTable table(3);
  table.OInsert("a", "1");
  table.ODelete("a");
  table.OInsert("b", "2");
  testing::internal::CaptureStdout();
  table.OPrint();
  string out = testing::internal::GetCapturedStdout();
  EXPECT_NE(out.find("b -> 2"), string::npos);
}

TEST(OpenTable, Serialization) {
  OpenTable table1;
  table1.OInsert("key1", "value1");
  table1.OInsert("key2", "value2");
  table1.OInsert("key3", "value3");
  table1.Save("test_ot.bin");

  OpenTable table2;
  table2.Load("test_ot.bin");
  EXPECT_EQ(table2.OLength(), 3);
  EXPECT_EQ(table2.OGet("key1"), "value1");
  EXPECT_EQ(table2.OGet("key2"), "value2");
  EXPECT_EQ(table2.OGet("key3"), "value3");
  EXPECT_TRUE(table2.OContains("key2"));
  EXPECT_FALSE(table2.OContains("key4"));

  remove("test_ot.bin");
}