#include "DDHeader.h"
#include "GameBase.h"
#include "Game2D.h"

using namespace std;
using namespace dd;

int main(int argc, char* argv[])
{
    printf("Engine Initialized...");
    Game2D *game2d = new Game2D();
    game2d->Run();

    cin.get();
	return 0;
}

 