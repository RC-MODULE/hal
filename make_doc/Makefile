#include ../../setenv.mk
SHELL := cmd
.PHONY: chm pdf html

all: pdf

chm: 
	doxygen
	utf2win.bat
	taskkill /FI "windowtitle eq hal"
	-cd html && hhc index.hhp
	copy html\*.chm .
    
html:
	doxygen

pdf: 
	doxygen
	cd latex &&	call make.bat	
	copy .\latex\refman.pdf .\hal.pdf
	
clean: 
	-rm -f -r -v html xml latex hal.chm hal.pdf *.html .* 
	
open:
	www.bat
	
	#http://rc-module.github.io/hal
	#pdf
	#cp ./latex/refman.pdf ./html/hal.pdf
	#copy hal.pdf  html\hal.pdf	
	#hal.html ipp-hal.html 
	#copy ipp-hal.html html\ipp-hal.html
	#copy hal.html html\hal.html

github: clean pdf chm
	doxygen
	copy hal.chm html\hal.chm /Y
	copy hal.pdf html\hal.pdf	/Y
	7za a html\hal.zip hal.chm &&\
	cd html &&\
	git init . && git checkout --orphan gh-pages &&\
	git add * &&\
	git commit -m "documentation-for-gh-pages" &&\
	git remote add origin https://github.com/RC-MODULE/hal.git  &&\
	git push -u -f origin gh-pages	

	
github0: clean pdf chm
	copy hal.chm html\hal.chm /Y
	copy hal.pdf html\hal.pdf	/Y
	7za a html\hal.zip hal.chm &&\
	cd html &&\
	git init . && git checkout --orphan gh-pages &&\
	git add * &&\
	git commit -m "documentation-for-gh-pages" &&\
	git remote add origin https://github.com/RC-MODULE/hal.git  &&\
	git push -u -f origin gh-pages	

github-html: 
	cd html &&\
	git init . && git checkout --orphan gh-pages &&\
	git add * &&\
	git commit -m "documentation-for-gh-pages" &&\
	git remote add origin https://github.com/RC-MODULE/hal.git  &&\
	git push -u -f origin gh-pages	
	

ipp:
	doxygen DoxyIPP
	
ipp-hal.html: 
	lua ipp-hal.lua
	xsltproc -o ipp-hal.html ipp-hal.xsl ipp-hal.xml
	
hal.html:	
	lua summary.lua
	xsltproc -o hal.html hal.xsl hal.xml
	
git-summary: hal.html ipp-hal.html 
	copy ipp-hal.html html\ipp-hal.html
	copy hal.html html\hal.html
	cd html &&\
	git add * &&\
	git commit -m "documentation-for-gh-pages" &&\
	git push -u -f origin gh-pages	
	
	