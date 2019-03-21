ROOTFLAGS = $(shell root-config --cflags)
ROOTLIBS  = $(shell root-config --glibs) -lMinuit
FASTJETFLAGS = $(./bin/fastjet-config --cxxflags)
#FASTJETLIBS = $(./bin/fastjet-config --glibs) -lMinuit
#FASTJETPLUGINS = $(./bin/fastjet-config --plugins) 

INCLUDE   = ./include
LIB       = ./lib
FASTJET   = fastjet
#PLUGINS   = fastjetplugins
#TOOLS     = fastjettools
#SISCONE   = siscone
#SPHERICAL = siscone_spherical


all: JetAna

JetAna: JetAna.cc JetAna.h
	g++ -Wall -Wextra -O3 -o $@ JetAna.cc $(ROOTFLAGS) $(ROOTLIBS) $(FASTJETFLAGS) -I$(INCLUDE) -L$(LIB) -l$(FASTJET) # -l$(TOOLS) -l$(SISCONE) -l$(SPHERICAL) -l$(PLUGINS)


clean:
	rm -f JetAna *~
