GO ?= go
COVERAGE_MIN = 85
COVERAGE_DIR = build/coverage

.PHONY: build test lint bench coverage test-cover clean

build:
	$(GO) build -o lab3_go .

test:
	$(GO) test -v ./...

lint:
	$(GO) vet ./...
	@files=$$(gofmt -l .); if [ -n "$$files" ]; then printf '%s\n' "$$files"; exit 1; fi
	@echo 'go vet и gofmt: проверка пройдена'

bench:
	$(GO) test -run '^$$' -bench . -benchmem ./test

coverage:
	mkdir -p $(COVERAGE_DIR)
	$(GO) test ./... -coverpkg=./... -coverprofile=$(COVERAGE_DIR)/coverage.out
	$(GO) tool cover -html=$(COVERAGE_DIR)/coverage.out -o $(COVERAGE_DIR)/index.html
	$(GO) tool cover -func=$(COVERAGE_DIR)/coverage.out > $(COVERAGE_DIR)/summary.txt
	@awk '/^total:/ { print; found = 1; passed = ($$NF + 0 >= $(COVERAGE_MIN)) } END { exit !(found && passed) }' $(COVERAGE_DIR)/summary.txt

test-cover: coverage

clean:
	rm -rf lab3_go build
