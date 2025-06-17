#include <glad/glad.h>

#include "log.h"
#include "renderer/opengl-context.h"

void event_manager(Event& event) { LOG_INFO(event.to_string()); }

int main()
{
    Log::init();
    OpenGLContext context("Stupid Swarm", 1280, 720, event_manager);
}
