# Makefile
# Project semester BA2
# Macro definitions

CXX   = g++ 
CXXFLAGS = -Wall -std=c++11
CXXFILES = projet.cc simulation.cc player.cc ball.cc map.cc tools.cc  gui.cc
OFILES = projet.o simulation.o player.o ball.o map.o tools.o gui.o
LINKING = `pkg-config --cflags gtkmm-3.0`
LDLIBS = `pkg-config --libs gtkmm-3.0`

# Definition of the first rule
all:projet

projet: $(OFILES)
		$(CXX) $(CXXFLAGS) $(LINKING) $(OFILES) -o projet $(LDLIBS)

# Definition of special rules



#depend:
#	@echo " *** UPDATING DEPENDENCIES ***"
#	@(sed '/^# DO NOT DELETE THIS LINE/q' Makefile && \
#	  $(CXX) -MM $(CXXFLAGS) $(LINKING) $(CXXFILES) | \
#	  egrep -v "/usr/include" \
#	 ) >Makefile.new
#	@mv Makefile.new Makefile

clean:
	@echo " *** CLEANING .O FILES AND EXECUTABLE ***"
	@/bin/rm -f *.o *.x *.c~ *.h~ projet

#
# -Automatically generated dependency rules-
#
# DO NOT DELETE THIS LINE
projet.o: projet.cc define.h simulation.h tools.h player.h map.h ball.h gui.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)
simulation.o: simulation.cc simulation.h tools.h player.h map.h ball.h \
 error.h define.h
player.o: player.cc player.h tools.h
ball.o: ball.cc ball.h tools.h
map.o: map.cc map.h tools.h define.h
tools.o: tools.cc tools.h
gui.o: gui.cc gui.h simulation.h tools.h player.h map.h ball.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)
