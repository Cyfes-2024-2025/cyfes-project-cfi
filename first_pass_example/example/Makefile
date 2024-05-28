SOURCES := test.c
OUT_BIN := test.out

.PHONY: $(OUT_BIN)
$(OUT_BIN): $(SOURCES)
	clang -fpass-plugin=$(shell ls ./SkeletonPass.*) $(SOURCES) -o $(OUT_BIN)
