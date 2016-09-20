.PHONY: default clean

graphs = hw2-q1.pdf hw2-q3.pdf

default: $(graphs)

%.pdf: %.dot
	dot -Tpdf $^ > $@

clean:
	-rm -f $(graphs)
