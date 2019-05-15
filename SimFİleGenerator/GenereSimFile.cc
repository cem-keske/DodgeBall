#include <iostream>
#include <cmath>
#include <cstdlib>
#include <vector>

#define SIDE            800
#define DIM            400
#define nbBall          0

using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 4) {
        cout << "/!\\ Error use syntax : ./G nbCell nbPlayer nbObstacle > out.txt" << endl;
        return 0;
    }

    int nbCell(atoi(argv[1])), nbPlayer(atoi(argv[2])), nbObstacle(atoi(argv[3]));
    std::vector<bool> map(nbCell*nbCell, false);

    if (nbPlayer + nbObstacle >= nbCell*nbCell) {
        cout << "Impossible simulation file : not enough case" << endl;
        return 0;
    }

    cout << "\n# nbCell\n\t" << nbCell << endl;
    cout << "\n# number of players\n\t" << nbPlayer << endl;
    cout << "\n# position of players" << endl;
    int j(0);
    while (j < nbPlayer) {
        int K(rand()%(nbCell*nbCell));
        if (not map[K]) {
            map[K] = true;
            cout << -DIM + K%nbCell* SIDE/nbCell + SIDE/nbCell/2<< "\t" << DIM - K/nbCell*SIDE/nbCell - SIDE/nbCell/2
                 << "\t" << rand()%4+1 << "\t" << rand()%20 << endl;
            ++j;
        }
    }
    cout << "\n# number of obstacles\n\t" << nbObstacle << endl;
    cout << "\n# position of obstacles" << endl;
    int i(0);
    while (i < nbObstacle) {
        int K(rand()%(nbCell*nbCell));
        if (not map[K]) {
            map[K] = true;
            cout << K/nbCell << "\t" << K%nbCell << endl;
            ++i;
        }
    }

    cout << "\n# number of balls\n\t" << nbBall << endl;
    cout << "\n# position of balls" << endl;

    cout << "\n\t#\t And that's all" << endl;

    return 0;
}