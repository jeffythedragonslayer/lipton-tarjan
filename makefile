CXX=clang++
BOOST=-I/usr/local/boost_1_66_1
CXXFLAGS=-g -O0 -std=c++1z -c
OBJS := main.o lipton-tarjan.o strutil.o Partition.o BFSVisitorData.o EmbedStruct.o ScanVisitor.o graphutil.o
.DEFAULT_GOAL = all

-include $(OBJS:.o=.d)

all: lt planargen straightline unittest

lt: $(OBJS)
	$(CXX) $(OBJS) -o lt

planargen: planargen.o
	$(CXX) planargen.o -o planargen

straightline: straightline.o
	$(CXX) straightline.o -o straightline

unittest: unittest.o lipton-tarjan.o strutil.o Partition.o BFSVisitorData.o graphutil.o EmbedStruct.o ScanVisitor.o 
	$(CXX) unittest.o lipton-tarjan.o strutil.o Partition.o BFSVisitorData.o graphutil.o EmbedStruct.o ScanVisitor.o -o unittest

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(BOOST) $*.cpp
	@$(CXX) -MM $(CXXFLAGS) $(BOOST) $*.cpp > $*.d
	@cp -f $*.d $*.d.tmp
	@sed -e 's/.*://' -e 's/\\$$//' < $*.d.tmp | fmt -1 | \
	  sed -e 's/^ *//' -e 's/$$/:/' >> $*.d
	@rm -f $*.d.tmp 

clean:
	rm -f *.o *.d lt planargen straightline unittest
