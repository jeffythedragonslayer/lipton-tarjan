CXX=clang++
BOOST=-I/usr/local/boost_1_58_0
CXXFLAGS=-g -std=c++14 -c
OBJS := main.o lipton-tarjan.o UndirectedGraph.o
.DEFAULT_GOAL = all

-include $(OBJS:.o=.d)

all: lipton-tarjan UndirectedGraph.o PlanarGraph.o

lipton-tarjan: $(OBJS)
	$(CXX) $(OBJS) -o lipton-tarjan

UndirectedGraph.o: UndirectedGraph.cpp
	$(CXX) $(CXXFLAGS) UndirectedGraph.cpp -c

PlanarGraph.o: PlanarGraph.cpp
	$(CXX) $(CXXFLAGS) PlanarGraph.cpp -c

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(BOOST) $*.cpp
	@$(CXX) -MM $(CXXFLAGS) $(BOOST) $*.cpp > $*.d
	@cp -f $*.d $*.d.tmp
	@sed -e 's/.*://' -e 's/\\$$//' < $*.d.tmp | fmt -1 | \
	  sed -e 's/^ *//' -e 's/$$/:/' >> $*.d
	@rm -f $*.d.tmp 

clean:
	rm -f *.o *.d lipton-tarjan
