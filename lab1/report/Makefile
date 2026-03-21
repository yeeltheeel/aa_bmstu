ifeq ($(OS), Windows_NT)
	rem = del /s /q
	newdir = mkdir
	ifdir = @if not exist "$(my_dir)" ($(newdir) $(my_dir))
	copyfile = copy
	movefile = move /y
	renamefile = rename /y
	runexec = 
	ghost = gswin64c
	filesize_st = @for %%F in (
	filesize_fn = ) do @echo    %%~zF bytes
else
	rem = rm -rf
	newdir = mkdir -p
	ifdir = @if [ ! -d "$(my_dir)" ]; then $(newdir) $(my_dir); fi
	copyfile = cp
	movefile = mv -f
	renamefile = mv -f
	runexec = ./
	ghost = gs
	filesize_st = @echo    $$(stat -c%s
	filesize_fn = bytes)
endif

parts_tex = report.tex \
			preambula.tex \
			title.tex \
			intro.tex \
			analytic_part.tex \
			design_part.tex \
			tech_part.tex \
			experimental_part.tex \
			conclusion.tex \
			links.tex
parts_ps = $(parts_tex:.tex=.ps)
parts_dvi = $(parts_tex:.tex=.dvi)
parts_pdf = $(parts_tex:.tex=.pdf)

img_dir = images
my_dir = doc#lab_1/doc

tex_flags = -shell-escape -interaction=nonstopmode -halt-on-error
gs_flags = -sDEVICE=pdfwrite -dCompatibilityLevel=1.4 -dPDFSETTINGS=/ebook -dNOPAUSE -dQUIET -dBATCH -dSubsetFonts=true

svg_list = $(wildcard $(img_dir)/*.svg)
pdfs = $(svg_list:.svg=.pdf)

all: report.pdf compress_report
	make show_size
	make clean

compress_report: report.pdf
	$(ifdir)
	$(ghost) $(gs_flags) -sOutputFile=report1.pdf report.pdf
	$(movefile) report1.pdf $(my_dir)
	cd $(my_dir) && $(movefile) report1.pdf report.pdf

report.pdf: $(pdfs) $(parts_tex) 
	pdflatex $(tex_flags) $(parts_tex)
	pdflatex $(tex_flags) $(parts_tex)
	pdflatex $(tex_flags) $(parts_tex)

$(img_dir)/%.pdf: $(img_dir)/%.svg
	inkscape --export-filename=$@ --export-type=pdf $<

show_size:
	@echo report.pdf:
	$(filesize_st)report.pdf$(filesize_fn)
	@echo $(my_dir)/report.pdf:
	$(filesize_st)$(my_dir)/report.pdf$(filesize_fn)

clean:
	cd $(img_dir) && $(rem) *.pdf 
	cd svg-inkscape && $(rem) *.pdf *.pdf_tex
	$(rem) *.aux *.out *.log *.dvi *.ps *.toc