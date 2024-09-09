#ifndef __GAME_BASE_H_
#define __GAME_BASE_H_

#include "DDHeader.h"

using namespace std;

namespace dd
{

class GameBase
{
public:
    GameBase(void);
    virtual ~GameBase(void);

    virtual void Run();
    virtual void Start();
    virtual void Resume();
    virtual void Pause();
    virtual void Init();
    virtual void Destroy();
    virtual void Tick();
    virtual long GetMSec();

    virtual void Update();
    virtual void Render(float interpolation);

public:
    std::string gameName;

private:
    bool game_running;
    int TICKS_PER_SECOND;
    int SKIP_TICKS;
    int MAX_FRAMESKIP;
    time_t seconds;

    long next_game_tick;
    int loops;
    float interpolation;
};

}

#endif
