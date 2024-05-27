SOURCES := test.c
OUT_BIN := test.out

$(OUT_BIN): $(SOURCES)
	clang -fpass-plugin=`echo ./SkeletonPass.*` $(SOURCES) -o $(OUT_BIN)
