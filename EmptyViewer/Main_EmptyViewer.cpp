#define _CRT_SECURE_NO_WARNINGS
#include <GL/glew.h>
#include <GL/glut.h>
#include <stdio.h>
#include <vector>
#include <string>
#include <cmath>

struct Vector3 { float x, y, z; };
struct Triangle { unsigned int indices[3]; };

extern std::vector<Vector3> gPositions;
extern std::vector<Vector3> gNormals;
extern std::vector<Triangle> gTriangles;
extern GLuint gVAO;

extern float gTotalTimeElapsed;
extern int gTotalFrames;

void load_mesh(std::string filename);
void upload_mesh_to_gpu();
void init_timer();
void start_timing();
float stop_timing();

bool useImmediateMode = true;

Vector3 normalize(const Vector3& v) {
    float len = std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    return { v.x / len, v.y / len, v.z / len };
}

void draw_bunny_immediate() {
    glBegin(GL_TRIANGLES);
    for (size_t i = 0; i < gTriangles.size(); ++i) {
        unsigned int i0 = gTriangles[i].indices[0];
        unsigned int i1 = gTriangles[i].indices[1];
        unsigned int i2 = gTriangles[i].indices[2];

        Vector3 n0 = gNormals[i0], n1 = gNormals[i1], n2 = gNormals[i2];
        Vector3 v0 = gPositions[i0], v1 = gPositions[i1], v2 = gPositions[i2];

        glNormal3f(n0.x, n0.y, n0.z);
        glVertex3f(v0.x, v0.y, v0.z);
        glNormal3f(n1.x, n1.y, n1.z);
        glVertex3f(v1.x, v1.y, v1.z);
        glNormal3f(n2.x, n2.y, n2.z);
        glVertex3f(v2.x, v2.y, v2.z);
    }
    glEnd();
}

void draw_bunny_vao() {
    glBindVertexArray(gVAO);
    glDrawElements(GL_TRIANGLES, (GLsizei)(gTriangles.size() * 3), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void draw_bunny() {
    if (useImmediateMode)
        draw_bunny_immediate();
    else
        draw_bunny_vao();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, 1280, 1280);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-0.1, 0.1, -0.1, 0.1, 0.1, 1000.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 0.0,
        0.0, 0.0, -1.0,
        0.0, 1.0, 0.0);

    GLfloat ambientLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);

    Vector3 lightDir = { -1.0f, -1.0f, -1.0f };
    lightDir = normalize(lightDir);
    GLfloat lightPosition[] = { lightDir.x, lightDir.y, lightDir.z, 0.0f };
    GLfloat lightAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    GLfloat lightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat lightSpecular[] = { 0.0f, 0.0f, 0.0f, 1.0f };

    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);

    GLfloat ka[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat kd[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat ks[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ka);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, kd);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, ks);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0f);

    glTranslatef(0.1f, -1.0f, -1.5f);
    glScalef(10.0f, 10.0f, 10.0f);

    start_timing();
    draw_bunny();
    float elapsed = stop_timing();

    gTotalTimeElapsed += elapsed;
    gTotalFrames++;
    float fps = gTotalFrames / gTotalTimeElapsed;
    char title[128];
    sprintf(title, "OpenGL Bunny: %.2f FPS", fps);
    glutSetWindowTitle(title);

    glutSwapBuffers();
    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
    if (key == '1') useImmediateMode = true;
    else if (key == '2') useImmediateMode = false;
}

void init_opengl() {
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glShadeModel(GL_SMOOTH);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1280, 1280);
    glutCreateWindow("OpenGL");

    glewInit();
    init_opengl();
    load_mesh("bunny.obj");
    upload_mesh_to_gpu();
    init_timer();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}
