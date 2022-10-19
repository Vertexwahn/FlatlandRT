#
# UNIX Makefile for NASM documentation
#

top_srcdir	= ..
srcdir		= .

prefix		= /usr/local
exec_prefix	= ${prefix}
bindir		= ${exec_prefix}/bin
mandir		= ${datarootdir}/man
docdir		= ${datarootdir}/doc/${PACKAGE}
htmldir		= ${docdir}
infodir		= ${datarootdir}/info
datarootdir	= ${prefix}/share

INSTALL		= /bin/install -c
INSTALL_PROGRAM	= ${INSTALL}
INSTALL_DATA	= ${INSTALL} -m 644

PERL		= perl -I$(srcdir)

PDFOPT		= 

MKDIR		= mkdir
RM_F		= rm -f
RM_RF		= rm -rf
CP_F		= cp -f
CP_UF		= cp -ufv

# Auxiliary files referenced by the HTML files
HTMLAUX		= nasmdoc.css local.css nasmlogw.png

SRCS		= nasmdoc.src inslist.src changes.src version.src
OUT		= html nasmdoc.txt nasmdoc.pdf

.SUFFIXES:
.SUFFIXES: .pfa .ph

all: $(OUT)

inslist.src: inslist.pl ../x86/insns.dat
	$(PERL) $(srcdir)/inslist.pl $(srcdir)/../x86/insns.dat

.PHONY: html
html: $(HTMLAUX)
	$(MKDIR) -p html
	for f in $(HTMLAUX); do $(CP_UF) "$(srcdir)/$$f" html/; done
	$(MAKE) html/nasmdoc0.html

RDSRC = $(PERL) $(srcdir)/rdsrc.pl -I$(srcdir)/

html/nasmdoc0.html: $(SRCS) rdsrc.pl
	$(RM_F) html/*.html
	$(RDSRC) -ohtml html nasmdoc.src

nasmdoc.dip: $(SRCS) rdsrc.pl
	$(RDSRC) dip nasmdoc.src

nasmdoc.txt: $(SRCS) rdsrc.pl
	$(RDSRC) txt nasmdoc.src

version.src: $(top_srcdir)/version.pl $(top_srcdir)/version
	$(PERL) $(top_srcdir)/version.pl docsrc \
		< $(top_srcdir)/version > version.src

nasmdoc.ps: nasmdoc.dip genps.pl afmmetrics.ph ttfmetrics.ph \
	pswidth.ph nasmlogo.eps psfonts.ph head.ps
	$(PERL) $(srcdir)/genps.pl -epsdir "$(srcdir)" \
		-headps $(srcdir)/head.ps nasmdoc.dip \
		> nasmdoc.ps

nasmdoc.pdf: nasmdoc.ps pspdf.pl
	$(PERL) $(srcdir)/pspdf.pl $(PDFOPT) nasmdoc.ps nasmdoc.pdf

clean:
	-$(RM_F) *.rtf *.hpj *.texi *.gid *.ipf *.dip
	-$(RM_F) *.aux *.cp *.fn *.ky *.pg *.log *.toc *.tp *.vr
	-$(RM_F) inslist.src version.src
	-$(RM_F) nasmdoc*.ps

spotless: clean
	-$(RM_RF) html info
	-$(RM_F) *.hlp nasmdoc.txt *.inf *.pdf *.dvi

install: all
	$(MKDIR) -p $(DESTDIR)$(htmldir)
	$(INSTALL_DATA) html/* $(DESTDIR)$(htmldir)
	$(MKDIR) -p $(DESTDIR)$(docdir)
	$(INSTALL_DATA) nasmdoc.pdf nasmdoc.txt $(DESTDIR)$(docdir)
