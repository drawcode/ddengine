#include "DDHeader.h"
#include "GameBase.h"
namespace dd
{

GameBase::GameBase(void)
{   
    Init();
}

GameBase::~GameBase(void)
{
    Destroy();
}

void GameBase::Init()
{
    TICKS_PER_SECOND = 60;
    SKIP_TICKS = 1000 / TICKS_PER_SECOND;
    MAX_FRAMESKIP = 5;

    next_game_tick = GetMSec();

    interpolation;

    game_running = true;
}

long GameBase::GetMSec()
{
#ifdef WIN32
    //printf("\r\n%d", GetTickCount());
    return GetTickCount();
#else
    timeval tim;
    gettimeofday(&tim, NULL);
    long t1=tim.tv_sec+(tim.tv_usec*1000000);
    return t1;

#endif
}

void GameBase::Destroy()
{
}

void GameBase::Run()
{
    while(game_running) {        
        Tick();
        interpolation = float(GetMSec() + SKIP_TICKS - next_game_tick) / float( SKIP_TICKS );
        Render(interpolation);
    }
}

void GameBase::Resume()
{
    Start();
}

void GameBase::Start()
{
    game_running = true;
}

void GameBase::Pause()
{
    game_running = false;
}

void GameBase::Update()
{
    // state and physics
    printf("\r\n");
    printf("Update");
}

void GameBase::Render(float interpolation)
{
    // render back buffer and display
    //printf("\r\n");
    //printf("Render: ", interpolation);
}

void GameBase::Tick()
{
    //printf("\r\n");
    //printf("Tick: ", interpolation);
    loops = 0;
    while( GetMSec() > next_game_tick && loops < MAX_FRAMESKIP) {
        Update();
        next_game_tick += SKIP_TICKS;
        loops++;
    }
}

}
