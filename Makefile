CXX=g++
CXXFLAGS= \
	-Wall \
	-std=c++17 \
	-I src/ \
	-I ./
GAMES=KuhnPoker OCP Dudo Domino
DEPS=$(patsubst %, %.o, $(GAMES))
BINARIES=dfs gebr cfr

%.o: src/games/%.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS)

%.o: src/algorithms/%.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS)

dfs: $(DEPS) src/dfs.cpp DFS.o
	$(CXX) -o $@ $^ $(CXXFLAGS)

cfr: $(DEPS) src/cfr.cpp CFR.o
	$(CXX) -o $@ $^ $(CXXFLAGS)

gebr: $(DEPS) src/gebr.cpp GEBR.o
	$(CXX) -o $@ $^ $(CXXFLAGS)

.PHONY: clean

clean:
	rm -f $(BINARIES) *.o
