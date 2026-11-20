CXX ?= g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
COVERAGE_FLAGS = -std=c++17 -Wall -Wextra -O0 -g --coverage
COVERAGE_MIN = 90
COVERAGE_CSV = build/coverage_report/summary.csv

STRUCT_SOURCES = $(wildcard struct/*/*.cpp)
SOURCES = Process/ProcessCommand.cpp $(STRUCT_SOURCES)
HEADERS = Process/ProcessCommand.h struct/utils.h $(wildcard struct/*/*.h)
TEST_SOURCES = $(wildcard tests/*.cpp)
FORMAT_SOURCES = main.cpp benchmark/benchmark.cpp $(SOURCES) $(HEADERS) $(TEST_SOURCES) $(wildcard tests/*.h)
GTEST_FLAGS = $(shell pkg-config --cflags --libs gtest_main 2>/dev/null)
ifeq ($(strip $(GTEST_FLAGS)),)
GTEST_FLAGS = -lgtest_main -lgtest -pthread
else
GTEST_FLAGS += -pthread
endif
TARGET = build/app

GCOV = gcov
ifeq ($(shell uname -s),Darwin)
GCOV = xcrun llvm-cov gcov
endif

all: $(TARGET)

$(TARGET): main.cpp $(SOURCES) $(HEADERS)
	mkdir -p build
	$(CXX) $(CXXFLAGS) -I. -IProcess -Istruct -Istruct/Array -Istruct/LinkedList -Istruct/SingleList -Istruct/Queue -Istruct/Stack -Istruct/Tree -Istruct/hash main.cpp $(SOURCES) -o $(TARGET)

build/benchmark: benchmark/benchmark.cpp $(STRUCT_SOURCES) $(HEADERS)
	mkdir -p build
	$(CXX) $(CXXFLAGS) -I. -IProcess -Istruct -Istruct/Array -Istruct/LinkedList -Istruct/SingleList -Istruct/Queue -Istruct/Stack -Istruct/Tree -Istruct/hash benchmark/benchmark.cpp $(STRUCT_SOURCES) -o $@

bench: build/benchmark
	./build/benchmark

build/run_tests: $(TEST_SOURCES) $(SOURCES) $(HEADERS)
	mkdir -p build
	$(CXX) $(CXXFLAGS) -I. -IProcess -Istruct -Istruct/Array -Istruct/LinkedList -Istruct/SingleList -Istruct/Queue -Istruct/Stack -Istruct/Tree -Istruct/hash $(TEST_SOURCES) $(SOURCES) $(GTEST_FLAGS) -o $@

test: build/run_tests
	cd build && ./run_tests

build/coverage/run_tests: $(TEST_SOURCES) $(SOURCES) $(HEADERS)
	mkdir -p build/coverage
	$(CXX) $(COVERAGE_FLAGS) -I. -IProcess -Istruct -Istruct/Array -Istruct/LinkedList -Istruct/SingleList -Istruct/Queue -Istruct/Stack -Istruct/Tree -Istruct/hash $(TEST_SOURCES) $(SOURCES) $(GTEST_FLAGS) -o $@

build/coverage/benchmark: benchmark/benchmark.cpp $(STRUCT_SOURCES) $(HEADERS)
	mkdir -p build/coverage
	$(CXX) $(COVERAGE_FLAGS) -I. -IProcess -Istruct -Istruct/Array -Istruct/LinkedList -Istruct/SingleList -Istruct/Queue -Istruct/Stack -Istruct/Tree -Istruct/hash benchmark/benchmark.cpp $(STRUCT_SOURCES) -o $@

gcov_report: build/coverage/run_tests build/coverage/benchmark
	find build/coverage -name '*.gcda' -delete
	cd build/coverage && ./run_tests
	./build/coverage/benchmark > build/coverage/benchmark_output.txt
	mkdir -p build/coverage_report
	gcovr --root . build/coverage --gcov-executable '$(GCOV)' \
	      --filter 'struct/' --filter 'Process/' --filter 'benchmark/' \
	      --exclude-branches-by-pattern '.*' \
	      --html-details build/coverage_report/index.html \
	      --json-summary build/coverage_report/summary.json \
	      --csv build/coverage_report/summary.csv \
	      --fail-under-line $(COVERAGE_MIN) --fail-under-function $(COVERAGE_MIN)
	python3 tools/hide_coverage_branches.py build/coverage_report
	$(MAKE) --no-print-directory check-coverage

check-coverage:
	@awk -F, -v minimum=$(COVERAGE_MIN) '\
	NR == 1 { if ($$2 != "line_total" || $$3 != "line_covered" || $$8 != "function_total" || $$9 != "function_covered") failed = 1 } \
	NR > 1 { rows++; \
		if (($$2 > 0 && 100 * $$3 < minimum * $$2) || ($$8 > 0 && 100 * $$9 < minimum * $$8)) { \
			print "Покрытие ниже порога:", $$1; failed = 1 \
		} \
	} END { \
		if (!rows) { print "Нет статистики покрытия"; failed = 1 } \
		if (!failed) print "В каждом файле: строки и функции не ниже " minimum "%"; \
		exit failed \
	}' '$(COVERAGE_CSV)'

coverage: gcov_report

format:
	clang-format -i $(FORMAT_SOURCES)

format-check:
	clang-format --dry-run --Werror $(FORMAT_SOURCES)

lint:
	cppcheck --std=c++17 --enable=warning,performance,portability  --suppress=missingIncludeSystem -I. -IProcess -Istruct -Istruct/Array -Istruct/hash main.cpp Process/ProcessCommand.cpp benchmark/benchmark.cpp struct/

clean:
	rm -rf build

check:
	cppcheck --std=c++17 --enable=warning,performance,portability --suppress=missingIncludeSystem -I. -IProcess -Istruct -Istruct/Array -Istruct/hash main.cpp Process/ProcessCommand.cpp benchmark/benchmark.cpp struct/
	@echo "Проверка cppcheck завершена."

.PHONY: all bench test gcov_report coverage check-coverage format format-check lint check clean 
