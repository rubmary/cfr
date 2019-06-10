CXX=g++
CXXFLAGS= \
	-Wall \
	-std=c++17 \
	-I src/ \
	-I ./
PATHGAME=src/games/
GAMES=KuhnPoker OCP Dudo Domino
SRC=$(foreach game, $(GAMES), $(PATHGAME)$(game)/$(game).cpp)
OBJECTS=$(patsubst %, %.o, $(GAMES))
BINARIES=dfs gebr cfr

%.o: $(SRC)
	$(CXX) -c $(PATHGAME)$*/$*.cpp -o $@ $(CXXFLAGS)

dfs: $(OBJECTS) src/dfs.cpp
	$(CXX) -o $@ $^ $(CXXFLAGS)

.PHONY: clean

clean:
	rm -f $(BINARIES) *.o
