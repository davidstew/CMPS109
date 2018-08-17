# $Id: Makefile,v 1.7 2017-07-24 15:57:08-07 - - $

MKFILE      = Makefile
DEPFILE     = ${MKFILE}.dep
NOINCL      = ci clean spotless
NEEDINCL    = ${filter ${NOINCL}, ${MAKECMDGOALS}}
GMAKE       = ${MAKE} --no-print-directory
WARNINGS    = -Wall -Wextra -Wold-style-cast
GPP         = g++ -std=gnu++14 -g -O0 -rdynamic ${WARNINGS}

MODULES     = debug graphics interp rgbcolor shape util main
CPPHEADER   = $(wildcard ${MODULES:=.h})
CPPSOURCE   = $(wildcard ${MODULES:=.cpp})
TCCFILES    = $(wildcard ${MODULES:=.tcc})
GENFILES    = colors.cppgen
SOURCES     = $(wildcard ${foreach MOD, ${MODULES}, \
                 ${MOD}.h  ${MOD}.tcc  ${MOD}.cpp})
OTHERS      = ${MKFILE} ${DEPFILE} mk-colors.perl
ALLSOURCES  = ${SOURCES} ${OTHERS}
EXECBIN     = gdraw
OBJECTS     = ${CPPSOURCE:.cpp=.o}
LINKLIBS    = -lGL -lGLU -lglut -lm

LISTING     = Listing.ps
CLASS       = cmps109-wm.w15
PROJECT     = asg3

all : ${EXECBIN}
	- checksource ${ALLSOURCES}

echo :
	- echo ${ALLSOURCES}

${EXECBIN} : ${OBJECTS}
	${GPP} -o $@ ${OBJECTS} ${LINKLIBS}

%.o : %.cpp
	${GPP} -c $<
	- cpplint.py.perl $<

colors.cppgen: mk-colors.perl
	mk-colors.perl >colors.cppgen

ci : ${ALLSOURCES}
	cid + ${ALLSOURCES}
	- checksource ${ALLSOURCES}

lis : ${ALLSOURCES}
	mkpspdf ${LISTING} ${ALLSOURCES} ${DEPFILE}

clean :
	- rm ${OBJECTS} ${DEPFILE} core ${GENFILES}

spotless : clean
	- rm ${EXECBIN} ${LISTING} ${LISTING:.ps=.pdf}


submit : ${ALLSOURCES}
	- checksource ${ALLSOURCES}
	submit ${CLASS} ${PROJECT} ${ALLSOURCES}

dep : ${CPPSOURCE} ${CPPHEADER} ${TCCFILES} ${GENFILES}
	@ echo "# ${DEPFILE} created `LC_TIME=C date`" >${DEPFILE}
	${GPP} -MM ${CPPSOURCE} >>${DEPFILE}

${DEPFILE} :
	@ touch ${DEPFILE}
	${GMAKE} dep

again :
	${GMAKE} spotless dep ci all lis

ifeq (${NEEDINCL}, )
include ${DEPFILE}
endif
