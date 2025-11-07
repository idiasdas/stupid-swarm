#include <glad/glad.h>

#include "log.h"
#include "simulations/moving_goal.hpp"

int main()
{
    Log::Init();
    MovingGoalSimulation movingGoalSimulation;
    movingGoalSimulation.Run();
    return 0;
}
