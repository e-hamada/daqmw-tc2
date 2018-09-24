BASENAME = daqmw-tc-bs

SRCS += $(BASENAME).tex

DIRS += ../byte_order
#DIRS += ../double-array
DIRS += ../draw_graph
DIRS += ../fread
DIRS += ../read_file_decode
DIRS += ../read_file_decode_graph
DIRS += ../read_nc_decode
DIRS += ../read_nc_decode_graph

all: pdf

ps:  $(BASENAME).ps
pdf: $(BASENAME).pdf
dvi: $(BASENAME).dvi

$(BASENAME).tex:
	./create-latex-src.py $(BASENAME).tex $(DIRS)

$(BASENAME).pdf: $(BASENAME).dvi
	dvipdfmx $(BASENAME).dvi

$(BASENAME).ps: $(BASENAME).dvi
	dvips -f -o $(BASENAME).ps $(BASENAME).dvi 

$(BASENAME).dvi: $(SRCS) $(FIGS)
	platex $(BASENAME).tex

clean:
	@rm -f $(BASENAME).tex $(BASENAME).out $(BASENAME).aux $(BASENAME).toc $(BASENAME).log $(BASENAME).dvi $(BASENAME).ps $(BASENAME).pdf
