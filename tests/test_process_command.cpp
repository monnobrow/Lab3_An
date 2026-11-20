#include "../Process/ProcessCommand.h"

#include <filesystem>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

#include <gtest/gtest.h>
/*CLI: парсинг, валидация, интеграция всех структур*/
namespace {

class CoutCapture {
public:
  CoutCapture() : previous_(std::cout.rdbuf(output_.rdbuf())) {}
  ~CoutCapture() { std::cout.rdbuf(previous_); }

  std::string str() const { return output_.str(); }

private:
  std::ostringstream output_;
  std::streambuf *previous_;
};

class ProcessCommandTest : public ::testing::Test {
protected:
  void SetUp() override {
  const auto *info = ::testing::UnitTest::GetInstance()->current_test_info();
  filename_ = std::filesystem::temp_directory_path() /
              (std::string("lab3_") + info->test_suite_name() + "_" +
               info->name() + ".data");

  for (char c : std::string("FLSQMTHO")) {
    auto file = filename_;
    file += "_" + std::string(1, c);
    std::filesystem::remove(file);
  }
}

 void TearDown() override {
  for (char c : std::string("FLSQMTHO")) {
    auto file = filename_;
    file += "_" + std::string(1, c);
    std::filesystem::remove(file);
  }
  }
  std::string Run(const std::string &query) {
    CoutCapture capture;

    std::filesystem::path structure_file = filename_;
    structure_file += "_" + query.substr(0, 1);

    ProcessCommand(structure_file.string(), query);
    return capture.str();
  }

  std::filesystem::path filename_;
};

TEST_F(ProcessCommandTest, ParsesInputAndRejectsInvalidCommands) {
  EXPECT_THROW(ProcessCommand(filename_.string(), ""), std::invalid_argument);
  EXPECT_THROW(ProcessCommand(filename_.string(), "   "),
               std::invalid_argument);
  EXPECT_THROW(ProcessCommand(filename_.string(), "XWhatever"),
               std::invalid_argument);
  EXPECT_THROW(ProcessCommand(filename_.string(), "SPush"),
               std::invalid_argument);
  EXPECT_THROW(ProcessCommand(filename_.string(), "SPush value extra"),
               std::invalid_argument);
  EXPECT_THROW(ProcessCommand(filename_.string(), "TAdd 12abc"),
               std::invalid_argument);
  EXPECT_THROW(ProcessCommand(filename_.string(), "TAdd"),
               std::invalid_argument);
}

TEST_F(ProcessCommandTest, StackQueueAndArrayCommands) {
  EXPECT_EQ(Run("SPush \"hello world\""), "-> hello world\n");
  EXPECT_EQ(Run("STop"), "-> hello world\n");
  EXPECT_EQ(Run("SIsEmptyStack"), "-> FALSE\n");
  EXPECT_EQ(Run("SPop"), "-> hello world\n");
  EXPECT_EQ(Run("SIsEmptyStack"), "-> TRUE\n");
  EXPECT_THROW(Run("SPop"), std::runtime_error);
  EXPECT_THROW(Run("SUnknown"), std::invalid_argument);
  EXPECT_THROW(Run("STop extra"), std::invalid_argument);
  EXPECT_NO_THROW(Run("SClear"));
  EXPECT_NO_THROW(Run("SPrint"));
  EXPECT_NO_THROW(Run("SPrintAddresses"));

  EXPECT_EQ(Run("QPush first"), "-> first\n");
  EXPECT_EQ(Run("QPush second"), "-> second\n");
  EXPECT_EQ(Run("QFront"), "-> first\n");
  EXPECT_EQ(Run("QBack"), "-> second\n");
  EXPECT_EQ(Run("QPop"), "-> first\n");
  EXPECT_NO_THROW(Run("QPrintQueue"));
  EXPECT_NO_THROW(Run("QPrintAddresses"));
  EXPECT_NO_THROW(Run("QClear"));
  EXPECT_THROW(Run("QPop"), std::runtime_error);
  EXPECT_THROW(Run("QUnknown"), std::invalid_argument);
  EXPECT_THROW(Run("QBack extra"), std::invalid_argument);

  EXPECT_EQ(Run("MAddEnd a"), "-> a\n");
  EXPECT_EQ(Run("MAddEnd b"), "-> b\n");
  EXPECT_EQ(Run("MLength"), "-> 2\n");
  EXPECT_EQ(Run("MGet 1"), "-> b\n");
  EXPECT_EQ(Run("MAddIndex middle 1"), "-> middle\n");
  EXPECT_EQ(Run("MReplace updated 0"), "-> updated\n");
  EXPECT_EQ(Run("MDeleteIndex 2"), "-> b\n");
  EXPECT_NO_THROW(Run("MPrint"));
  EXPECT_NO_THROW(Run("MPrintAddresses"));
  EXPECT_NO_THROW(Run("MClear"));
  EXPECT_THROW(Run("MGet 0"), std::runtime_error);
  EXPECT_THROW(Run("MUnknown"), std::invalid_argument);
  EXPECT_THROW(Run("MLength extra"), std::invalid_argument);
}

TEST_F(ProcessCommandTest, SinglyAndDoublyLinkedListCommands) {
  EXPECT_EQ(Run("FAddFirst b"), "-> b\n");
  EXPECT_EQ(Run("FAddEnd d"), "-> d\n");
  EXPECT_EQ(Run("FAddAfter b c"), "-> c\n");
  EXPECT_EQ(Run("FAddBefore b a"), "-> a\n");
  EXPECT_EQ(Run("FAddIndex e 4"), "-> e\n");
  EXPECT_EQ(Run("FGet 2"), "-> c\n");
  EXPECT_EQ(Run("FFind c"), "-> TRUE\n");
  EXPECT_EQ(Run("FFind missing"), "-> FALSE\n");
  EXPECT_NO_THROW(Run("FPrint"));
  EXPECT_NO_THROW(Run("FPrintBack"));
  EXPECT_NO_THROW(Run("FPrintAddresses"));
  EXPECT_EQ(Run("FDeleteAfter b"), "-> c\n");
  EXPECT_EQ(Run("FDeleteBefore b"), "-> a\n");
  EXPECT_EQ(Run("FDeleteIndex 1"), "-> d\n");
  EXPECT_EQ(Run("FDeleteValue e"), "-> e\n");
  EXPECT_EQ(Run("FDeleteFirst"), "-> b\n");
  EXPECT_THROW(Run("FDeleteEnd"), std::runtime_error);
  EXPECT_NO_THROW(Run("FClear"));
  EXPECT_THROW(Run("FGet 0"), std::runtime_error);
  EXPECT_THROW(Run("FAddAfter absent value"), std::runtime_error);
  EXPECT_THROW(Run("FUnknown"), std::invalid_argument);
  EXPECT_THROW(Run("FPrint extra"), std::invalid_argument);

  EXPECT_EQ(Run("LAddFirst two"), "-> two\n");
  EXPECT_EQ(Run("LAddEnd four"), "-> four\n");
  EXPECT_EQ(Run("LAddAfter two three"), "-> three\n");
  EXPECT_EQ(Run("LAddBefore two one"), "-> one\n");
  EXPECT_EQ(Run("LAddIndex five 4"), "-> five\n");
  EXPECT_EQ(Run("LGet 2"), "-> three\n");
  EXPECT_EQ(Run("LFind three"), "-> TRUE\n");
  EXPECT_EQ(Run("LFind missing"), "-> FALSE\n");
  EXPECT_NO_THROW(Run("LPrint"));
  EXPECT_NO_THROW(Run("LPrintBack"));
  EXPECT_NO_THROW(Run("LPrintAddresses"));
  EXPECT_EQ(Run("LDeleteAfter two"), "-> three\n");
  EXPECT_EQ(Run("LDeleteBefore two"), "-> one\n");
  EXPECT_EQ(Run("LDeleteIndex 1"), "-> four\n");
  EXPECT_EQ(Run("LDeleteValue five"), "-> five\n");
  EXPECT_EQ(Run("LDeleteFirst"), "-> two\n");
  EXPECT_THROW(Run("LDeleteEnd"), std::runtime_error);
  EXPECT_NO_THROW(Run("LClear"));
  EXPECT_THROW(Run("LGet 0"), std::runtime_error);
  EXPECT_THROW(Run("LAddBefore absent value"), std::runtime_error);
  EXPECT_THROW(Run("LUnknown"), std::invalid_argument);
  EXPECT_THROW(Run("LPrint extra"), std::invalid_argument);
}

TEST_F(ProcessCommandTest, TreeAndHashTableCommands) {
  EXPECT_EQ(Run("TAdd 10"), "-> 10\n");
  EXPECT_EQ(Run("TAdd 5"), "-> 5\n");
  EXPECT_EQ(Run("TAdd 20"), "-> 20\n");
  EXPECT_EQ(Run("TFind 5"), "-> TRUE\n");
  EXPECT_EQ(Run("TFind 99"), "-> FALSE\n");
  EXPECT_EQ(Run("THeight"), "-> 2\n");
  EXPECT_NO_THROW(Run("TPrintPryam"));
  EXPECT_NO_THROW(Run("TPrintSim"));
  EXPECT_NO_THROW(Run("TPrintObr"));
  EXPECT_NO_THROW(Run("TPrintBfs"));
  EXPECT_NO_THROW(Run("TPrintTree"));
  EXPECT_NO_THROW(Run("TPrintAddresses"));
  EXPECT_EQ(Run("TDelete 5"), "-> 5\n");
  EXPECT_THROW(Run("TDelete 99"), std::runtime_error);
  EXPECT_THROW(Run("TAdd 10"), std::runtime_error);
  EXPECT_NO_THROW(Run("TClear"));
  EXPECT_THROW(Run("TUnknown"), std::invalid_argument);
  EXPECT_THROW(Run("THeight extra"), std::invalid_argument);

  EXPECT_EQ(Run("HInsert key value"), "-> key: value\n");
  EXPECT_EQ(Run("HInsert other \"value with spaces\""),
            "-> other: value with spaces\n");
  EXPECT_EQ(Run("HGet key"), "-> value\n");
  EXPECT_EQ(Run("HContains other"), "-> TRUE\n");
  EXPECT_EQ(Run("HContains absent"), "-> FALSE\n");
  EXPECT_EQ(Run("HLength"), "-> 2\n");
  EXPECT_NO_THROW(Run("HPrint"));
  EXPECT_NO_THROW(Run("HPrintAddresses"));
  EXPECT_EQ(Run("HDelete key"), "-> key\n");
  EXPECT_THROW(Run("HGet key"), std::runtime_error);
  EXPECT_THROW(Run("HDelete key"), std::runtime_error);
  EXPECT_NO_THROW(Run("HClear"));
  EXPECT_THROW(Run("HGet"), std::invalid_argument);
  EXPECT_THROW(Run("HUnknown"), std::invalid_argument);
  EXPECT_THROW(Run("HLength extra"), std::invalid_argument);

  EXPECT_EQ(Run("OInsert first one"), "-> first: one\n");
  EXPECT_EQ(Run("OInsert second two"), "-> second: two\n");
  EXPECT_EQ(Run("OGet second"), "-> two\n");
  EXPECT_EQ(Run("OContains first"), "-> TRUE\n");
  EXPECT_EQ(Run("OContains absent"), "-> FALSE\n");
  EXPECT_EQ(Run("OLength"), "-> 2\n");
  EXPECT_NO_THROW(Run("OPrint"));
  EXPECT_EQ(Run("ODelete first"), "-> first\n");
  EXPECT_THROW(Run("OGet first"), std::runtime_error);
  EXPECT_THROW(Run("ODelete first"), std::runtime_error);
  EXPECT_THROW(Run("OGet"), std::invalid_argument);
  EXPECT_THROW(Run("OUnknown"), std::invalid_argument);
  EXPECT_THROW(Run("OLength extra"), std::invalid_argument);
}

TEST(ProcessCommand, PrintsHelp) {
  CoutCapture capture;
  PrintHelp();
  EXPECT_NE(capture.str().find("--query"), std::string::npos);
  EXPECT_NE(capture.str().find("MAddEnd"), std::string::npos);
}

} // namespace
