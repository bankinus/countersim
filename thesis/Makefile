src = $(wildcard *.tex)
pdf = $(src:.tex=.pdf)
svg = $(wildcard img/*.svg)
pdf_tex = $(svg:.svg=.pdf_tex)
imgpdf = $(wildcard img/*.pdf) $(svgpdf)

.PHONY: all clean install

all: $(pdf) $(imgpdf)

%.pdf_tex: %.svg
	inkscape -D -z --file=$< --export-pdf=$(@:.pdf_tex=.pdf) --export-latex --export-area-drawing

%.pdf: %.tex $(wildcard src/*.tex) $(wildcard *.bib) $(imgpdf) $(pdf_tex)
	echo $(pdf_tex)
	latexmk -file-line-error -synctex=1 -halt-on-error -pdf $<

fast:
	# enforce tex execution
	pdflatex -synctex=1 $(src)

show: all
	evince $(pdf)

clean:
	latexmk -C $(src)
	rm -f $(wildcard *.out *.nls *.nlo *.bbl *.blg *-blx.bib *.run.xml *.bcf *.synctex.gz *.fdb_latexmk *.fls *.toc)
	rm -f $(wildcard src/*.aux)
	rm -f $(pdf_tex:.pdf_tex=.pdf)
	rm -f $(pdf_tex)

