CXX ?= g++
CXXFLAGS ?= -g -Wall -W -Winline -ansi
CXXFLAGS += -I./UnitTest++/src -I./ -I../lib/cJSON -I../inc/lemmalib
LDFLAGS ?=
RM = rm

.SUFFIXES: .o .cpp

name        = LemmaTests
lib         = lib$(name).a
test        = test$(name)
testlibdir  = UnitTest++
testlib     = $(testlibdir)/libUnitTest++.a
testrunner  = Main.cpp

objects = ../src/lemmalib/MessageParser.o \
          ../src/lemmalib/Event.o \
					../lib/cJSON/cJSON.o

testobjects = MessageParserTests.o


all: $(lib)

$(lib): $(objects)
	@echo Creating spark communication library...
	@$(AR) cr $(lib) $(objects)

.cpp.o:
	@$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	-@$(RM) $(objects) $(lib) $(testobjects) 2> /dev/null


############### tests #############

test: $(lib) $(testlib) $(testobjects)
	@$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $(test) $(testlib) \
    $(lib) $(testobjects) $(testrunner)
	@echo running unit tests...
	@./$(test)

$(testlib):
	$(MAKE) -C $(testlibdir)

testclean:
	$(MAKE) -C $(testlibdir) clean
