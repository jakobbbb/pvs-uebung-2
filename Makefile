.ONESHELL:
GCC_FLAGS = -fopenmp -Wall

ASSIGNMENT_GROUP=B
ASSIGNMENT_NUMBER=02
ASSIGNMENT_TITLE=pvs$(ASSIGNMENT_NUMBER)-grp$(ASSIGNMENT_GROUP)

.PHONY: build
build: quicksort

.PHONY: debug
debug: GCC_FLAGS += -g
debug: build

.PHONY: quicksort
quicksort:
	g++ $(GCC_FLAGS) -o quicksort quicksort.cpp test.cpp

.PHONY: test
test: quicksort
	./quicksort test

.PHONY: clean
clean:
	rm quicksort

.PHONY: codeformat
codeformat:
	clang-format -i *{c,h}pp

PDF_FILENAME=$(ASSIGNMENT_TITLE).pdf
.PHONY: pdf
pdf:
	pandoc pvs.md -o $(PDF_FILENAME) --from markdown --template ~/.pandoc/eisvogel.latex --listings

FILES=Makefile pvs.md quicksort.cpp $(PDF_FILENAME)

ASSIGNMENT_DIR=$(ASSIGNMENT_TITLE)
TARBALL_NAME=$(ASSIGNMENT_TITLE)-piekarski-wichmann-ruckel.tar.gz
.PHONY: tarball
tarball: pdf
	[ -z "$(TARBALL_NAME)" ] || rm $(TARBALL_NAME)
	mkdir $(ASSIGNMENT_DIR)
	for f in $(FILES); do cp $$f $(ASSIGNMENT_DIR); done
	tar zcvf $(TARBALL_NAME) $(ASSIGNMENT_DIR)
	rm -fr $(ASSIGNMENT_DIR)
