// frame_timer.cpp
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>
#include <queue>
#include <fstream>
#include <float.h>

#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>

float gTotalTimeElapsed = 0.0f;
int gTotalFrames = 0;
GLuint gTimer;

void init_timer()
{
    glGenQueries(1, &gTimer);

    if (!GLEW_ARB_timer_query)
    {
        printf("ERROR: GL_ARB_timer_query not supported on this system!\n");
    }
}

void start_timing()
{
    glBeginQuery(GL_TIME_ELAPSED, gTimer);
}

float stop_timing()
{
    glEndQuery(GL_TIME_ELAPSED);

    GLint available = GL_FALSE;
    while (available == GL_FALSE)
        glGetQueryObjectiv(gTimer, GL_QUERY_RESULT_AVAILABLE, &available);

    GLuint64 result = 0;
    glGetQueryObjectui64v(gTimer, GL_QUERY_RESULT, &result);

    float timeElapsed = result / 1e9f;
    return timeElapsed;
}
