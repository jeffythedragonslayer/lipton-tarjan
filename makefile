CXXFLAGS=-g -Wall -std=c++14 -I/usr/local/boost_1_58_0 -c
OBJS := main.o lipton-tarjan.o
.DEFAULT_GOAL = all

-include $(OBJS:.o=.d)

all: lipton-tarjan 

lipton-tarjan: $(OBJS)
	g++ $(OBJS) -o lipton-tarjan

%.o: %.cpp
	g++ -c $(CXXFLAGS) $*.cpp
	@g++ -MM $(CXXFLAGS) $*.cpp > $*.d
	@cp -f $*.d $*.d.tmp
	@sed -e 's/.*://' -e 's/\\$$//' < $*.d.tmp | fmt -1 | \
	  sed -e 's/^ *//' -e 's/$$/:/' >> $*.d
	@rm -f $*.d.tmp 

clean:
	rm -f *.o *.d lipton-tarjan
