CXX=clang++
BOOST=-I/usr/local/boost_1_59_0
CXXFLAGS=-g -O0 -std=c++1z -c
OBJS := main.o lipton-tarjan.o strutil.o
.DEFAULT_GOAL = all

-include $(OBJS:.o=.d)

all: lt graphgen chrobak

lt: $(OBJS)
	$(CXX) $(OBJS) -o lt

graphgen: graphgen.o
	$(CXX) graphgen.o -o graphgen

chrobak: chrobak.o
	$(CXX) chrobak.o -o chrobak

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(BOOST) $*.cpp
	@$(CXX) -MM $(CXXFLAGS) $(BOOST) $*.cpp > $*.d
	@cp -f $*.d $*.d.tmp
	@sed -e 's/.*://' -e 's/\\$$//' < $*.d.tmp | fmt -1 | \
	  sed -e 's/^ *//' -e 's/$$/:/' >> $*.d
	@rm -f $*.d.tmp 

clean:
	rm -f *.o *.d lt
