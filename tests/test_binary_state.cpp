#include <cstdio>
#include <fstream>
#include <iterator>
#include <string>

#include <gtest/gtest.h>

#include "struct/Array/array.h"
#include "struct/Stack/stack.h"
#include "struct/Tree/tree.h"

TEST(BinaryState, RawBytesAndType) {
  const std::string file = "lab3_raw_state.bin";
  const std::string value("a\0b\nc", 5);
  Array source;
  source.MAddEnd(value);
  source.Save(file);
  std::ifstream input(file, std::ios::binary);
  const std::string bytes{std::istreambuf_iterator<char>(input),
                          std::istreambuf_iterator<char>()};
  EXPECT_EQ(bytes, std::string("LR3BA\1\0\0\0\5\0\0\0", 13) + value);
  Array restored;
  restored.Load(file);
  EXPECT_EQ(restored.MGet(0), value);
  Stack stack;
  EXPECT_THROW(stack.Load(file), std::runtime_error);
  std::remove(file.c_str());
}

TEST(BinaryState, LegacyAndInvalidData) {
  const std::string file = "lab3_legacy_state.bin";
  {
    std::ofstream out(file);
    out << "1\nold value\n";
  }
  Array arr;
  arr.Load(file);
  ASSERT_EQ(arr.MGet(0), "old value");
  {
    std::ofstream out(file, std::ios::binary);
    out.write("LR3BA\1\0\0\0\5\0\0\0x", 14);
  }
  EXPECT_THROW(arr.Load(file), std::runtime_error);
  EXPECT_EQ(arr.MGet(0), "old value");
  EXPECT_THROW(arr.Save("nonexistent_lab_state_dir/state.bin"), std::runtime_error);
  std::remove(file.c_str());
}

TEST(BinaryState, SignedKeysAndInvalidCounts) {
  const std::string file = "lab3_signed_tree.bin";
  Tree tree;
  tree.TAdd(-20);
  tree.TAdd(0);
  tree.Save(file);
  Tree restored;
  restored.Load(file);
  EXPECT_TRUE(restored.TFind(-20));
  EXPECT_TRUE(restored.TFind(0));
  for (const std::string contents : {"", "-1\n", "invalid\n", "1\n"}) {
    {
      std::ofstream out(file);
      out << contents;
    }
    if (contents.empty())
      EXPECT_NO_THROW(restored.Load(file));
    else
      EXPECT_THROW(restored.Load(file), std::runtime_error);
    EXPECT_TRUE(restored.TFind(-20));
  }
  std::remove(file.c_str());
}
