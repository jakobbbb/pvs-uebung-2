.ONESHELL:
GCC_FLAGS = -fopenmp -Wall

.PHONY: build
build: quicksort

.PHONY: quicksort
quicksort:
	g++ $(GCC_FLAGS) -o quicksort quicksort.cpp

.PHONY: test
test: quicksort
	@echo "Tests not implemented yet." && exit 1

.PHONY: clean
clean:
	rm quicksort

.PHONY: pdf
pdf:
	pandoc pvs.md -o pvs.pdf --from markdown --template ~/.pandoc/eisvogel.latex --listings

FILES=Makefile pvs.pdf pvs.md quicksort.cpp
DIR=pvs-02-gruppeB

.PHONY: tarball
tarball: pdf
	mkdir $(DIR)
	for f in $(FILES); do cp $$f $(DIR); done
	tar zcvf pvs-02-gruppeB-piekarski-wichmann-ruckel.tar.gz $(DIR)
	rm -fr $(DIR)
