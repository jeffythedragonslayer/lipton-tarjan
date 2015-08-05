CXX=clang++
CXXFLAGS=-g -std=c++14 -I/usr/local/boost_1_58_0 -c
OBJS := main.o lipton-tarjan.o UndirectedGraph.o
.DEFAULT_GOAL = all

-include $(OBJS:.o=.d)

all: lipton-tarjan UndirectedGraph.o

lipton-tarjan: $(OBJS)
	$(CXX) $(OBJS) -o lipton-tarjan

UndirectedGraph.o: UndirectedGraph.cpp
	clang++ -std=c++14 UndirectedGraph.cpp -c

%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) $*.cpp
	@$(CXX) -MM $(CXXFLAGS) $*.cpp > $*.d
	@cp -f $*.d $*.d.tmp
	@sed -e 's/.*://' -e 's/\\$$//' < $*.d.tmp | fmt -1 | \
	  sed -e 's/^ *//' -e 's/$$/:/' >> $*.d
	@rm -f $*.d.tmp 

clean:
	rm -f *.o *.d lipton-tarjan
