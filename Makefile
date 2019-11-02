CXX=g++
CXXFLAGS= \
	-Wall \
	-std=c++17 \
	-I src/ \
	-I ./
GAMES=KuhnPoker OCP Dudo Domino
DEPS=$(patsubst %, %.o, $(GAMES)) src/games/Game.hpp
BINARIES=dfs gebr cfr
TARGETS=targets/

all: dfs cfr gebr count

%.o: src/games/%.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS)

%.o: src/algorithms/%.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS)

dfs: $(DEPS) src/dfs.cpp DFS.o
	$(CXX) -o $(TARGETS)$@ $^ $(CXXFLAGS)

cfr: $(DEPS) src/cfr.cpp CFR.o
	$(CXX) -o $(TARGETS)$@ $^ $(CXXFLAGS)

gebr: $(DEPS) src/gebr.cpp GEBR.o
	$(CXX) -o $(TARGETS)$@ $^ $(CXXFLAGS)

count: $(DEPS) src/count.cpp GEBR.o
	$(CXX) -o $(TARGETS)$@ $^ $(CXXFLAGS)

get_boost:
	sudo mkdir -p boost
	sudo wget https://dl.bintray.com/boostorg/release/1.71.0/source/boost_1_71_0.tar.bz2
	sudo tar --bzip2 -xf boost_1_71_0.tar.bz2
	sudo cp -r boost_1_71_0/boost/* boost/
	sudo rm boost_1_71_0.tar.bz2
	sudo rm -d -r boost_1_71_0

.PHONY: clean

clean:
	rm -f $(BINARIES) *.o
	cd targets/
	rm -f $(BINARIES) *.o
