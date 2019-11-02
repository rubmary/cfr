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

.PHONY: clean

clean:
	cd targets/
	rm -f $(BINARIES) *.o
