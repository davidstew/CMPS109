# $Id: Makefile,v 1.14 2016-07-20 21:31:50-07 - - $

# By default, gmake sets the variable ${CXX}=g++ on a Linux system.
# To use Sun Studio C++ (CC), set the variable CXX=CC, either on
# the command line when calling make, or in an environment variable.

MKFILE      = Makefile
DEPFILE     = ${MKFILE}.dep
NOINCL      = ci clean spotless
NEEDINCL    = ${filter ${NOINCL}, ${MAKECMDGOALS}}
GMAKE       = ${MAKE} --no-print-directory

WARN        = -Wall -Wextra -Wold-style-cast
COMPILECPP  = g++ -std=gnu++14 -g -O0 ${WARN}
MAKEDEPCPP  = g++ -std=gnu++14 -MM

CPPHEADER   = listmap.h trace.h util.h xless.h xpair.h
TCCSOURCE   = listmap.tcc util.tcc
CPPSOURCE   = trace.cpp util.cpp main.cpp
ALLCPPSRC   = ${CPPHEADER} ${TCCSOURCE} ${CPPSOURCE}
OBJECTS     = ${CPPSOURCE:.cpp=.o}
EXECBIN     = keyvalue
OTHERS      = ${MKFILE} ${DEPFILE}
ALLSOURCES  = ${ALLCPPSRC} ${OTHERS}
LISTING     = Listing.ps

all : ${EXECBIN}

${EXECBIN} : ${OBJECTS}
	${COMPILECPP} -o $@ ${OBJECTS}

%.o : %.cpp
	${COMPILECPP} -c $<

ci : ${ALLSOURCES}
	cid + ${ALLSOURCES}
	- checksource ${ALLSOURCES}
	- cpplint.py.perl ${CPPSOURCE}

lis : ${ALLSOURCES}
	mkpspdf ${LISTING} ${ALLSOURCES}

clean :
	- rm ${OBJECTS} ${DEPFILE} core

spotless : clean
	- rm ${EXECBIN} ${LISTING} ${LISTING:.ps=.pdf}

dep : ${ALLCPPSRC}
	@ echo "# ${DEPFILE} created `LC_TIME=C date`" >${DEPFILE}
	${MAKEDEPCPP} ${CPPSOURCE} >>${DEPFILE}

${DEPFILE} :
	@ touch ${DEPFILE}
	${GMAKE} dep

again :
	${GMAKE} spotless dep ci all lis

ifeq (${NEEDINCL}, )
include ${DEPFILE}
endif
