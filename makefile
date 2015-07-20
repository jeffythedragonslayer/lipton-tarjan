CXXFLAGS=-g -Wall -std=c++11 -I/usr/local/boost_1_58_0 -c
OBJS := Vec.o MatrixNM.o mathutil.o main.o
.DEFAULT_GOAL = all

-include $(OBJS:.o=.d)

all: $(OBJS)
	g++ $(OBJS) -o chazelle
%.o: %.cpp
	g++ -c $(CXXFLAGS) $*.cpp
	@g++ -MM $(CXXFLAGS) $*.cpp > $*.d
	@cp -f $*.d $*.d.tmp
	@sed -e 's/.*://' -e 's/\\$$//' < $*.d.tmp | fmt -1 | \
	  sed -e 's/^ *//' -e 's/$$/:/' >> $*.d
	@rm -f $*.d.tmp 
clean:
	rm -f chazelle *.o *.d
