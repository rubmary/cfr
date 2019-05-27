#include <cstdlib>
#include <ctime>
#include <algorithms/CFR.hpp>
#include <fstream>
#include <iomanip>
using namespace std;

int iterations = 1000000;

int main(int argc, char **argv) {

    ofstream file_strt(argv[1]);
    ofstream file_rgrt(argv[2]);

    KuhnPoker kuhn_poker;
    CFR<State, Action, Properties, InformationSet> cfr({&kuhn_poker});
    vector <vector <double>> regret     = cfr.train(iterations);
    vector <vector <double>> strategy   = cfr.average_strategy();

    int M = strategy.size();
    file_strt << M << endl;
    for (int i = 0; i < M; i++) {
        int N = strategy[i].size();
        file_strt << N << ' ';
        for (int j = 0; j < N; j++)
            file_strt << fixed << setprecision(5) << strategy[i][j] << ' ';
        file_strt << endl;
    }

    int T = regret.size();
    for (int t = 1; t < T; t++){
        file_rgrt << regret[t][0] << ' ';
        file_rgrt << regret[t][1] << ' ';
        file_rgrt << regret[t][0] + regret[t][1] << endl;
    }
}
