// dhaka_scene.cpp
// Compile (Linux):
// g++ dhaka_scene.cpp -o dhaka_scene -lGL -lGLU -lglut -lm
//
// Controls:
//  - d : set Day
//  - n : set Night
//  - Space : toggle Day/Night
//  - Arrow keys / WASD : move camera
//  - +/- : zoom in/out
//  - Esc : exit

#include <GL/glut.h>
#include <cmath>
#include <vector>
#include <ctime>

#define PI 3.14159265358979323846f

// Scene state
bool isDay = true;
float timeOfDay = 0.0f; // 0..1 (unused but reserved)
int windowWidth = 1200, windowHeight = 800;

// Camera
float camX = 0.0f, camY = 12.0f, camZ = 40.0f;
float camYaw = 0.0f; // left-right
float camPitch = -12.0f;
float zoom = 1.0f;

// Animation
float metroPos = -60.0f; // move along x
float metroSpeed = 0.12f;
struct Car { float x,z, heading; float offset; };
std::vector<Car> cars;

// Stars positions
std::vector<std::pair<float,float>> stars;

// Utility ------------------------------------------------
void setMaterial(float r, float g, float b, float shin = 32.0f) {
    GLfloat ambient[] = { r*0.15f, g*0.15f, b*0.15f, 1.0f };
    GLfloat diffuse[] = { r, g, b, 1.0f };
    GLfloat spec[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spec);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shin);
}

void drawBox(float sx, float sy, float sz) {
    glPushMatrix();
    glScalef(sx, sy, sz);
    glutSolidCube(1.0);
    glPopMatrix();
}

// Objects ------------------------------------------------
void drawRoad(float length, float width) {
    glDisable(GL_LIGHTING);
    glColor3f(0.15f, 0.15f, 0.15f);
    glPushMatrix();
      glTranslatef(0.0f, 0.02f, 0.0f);
      glBegin(GL_QUADS);
        glVertex3f(-length, 0.0f, -width);
        glVertex3f(length, 0.0f, -width);
        glVertex3f(length, 0.0f, width);
        glVertex3f(-length, 0.0f, width);
      glEnd();
    glPopMatrix();

    // lane markings
    glColor3f(1.0f, 1.0f, 1.0f);
    float dash = 2.0f;
    for (float x=-length; x<length; x+=dash*2.0f) {
      glBegin(GL_QUADS);
        glVertex3f(x, 0.03f, -0.2f);
        glVertex3f(x+dash, 0.03f, -0.2f);
        glVertex3f(x+dash, 0.03f, 0.2f);
        glVertex3f(x, 0.03f, 0.2f);
      glEnd();
    }
    if (isDay) glEnable(GL_LIGHTING);
}

void drawBuilding(float x, float y, float z, float w, float h, float d) {
    glPushMatrix();
      glTranslatef(x, y + h/2.0f, z);
      setMaterial(0.6f, 0.6f, 0.65f, 8.0f);
      drawBox(w, h, d);

      // windows as darker rectangles
      glDisable(GL_LIGHTING);
      glColor3f(0.05f, 0.06f, 0.07f);
      for (float by=-h/2.0f + 0.5f; by<h/2.0f - 0.3f; by+=0.8f) {
        for (float bx=-w/2.0f + 0.4f; bx<w/2.0f - 0.4f; bx+=0.6f) {
          glPushMatrix();
            glTranslatef(bx, by, d/2.0f + 0.505f);
            glScalef(0.4f, 0.5f, 1.0f);
            glutSolidCube(1.0f);
          glPopMatrix();
        }
      }
      if (isDay) glEnable(GL_LIGHTING);
    glPopMatrix();
}

void drawShaplaSculpture() {
    // Stylized shapla at a roundabout
    glPushMatrix();
      setMaterial(0.1f, 0.45f, 0.1f, 16.0f);
      glTranslatef(-6.0f, 0.6f, 0.0f);
      glRotatef(-20, 0,1,0);
      // petal-like ellipses using scaled spheres
      for (int i=0;i<7;i++) {
        glPushMatrix();
          float ang = i * (360.0f/7.0f);
          glRotatef(ang, 0,1,0);
          glTranslatef(0.0f, 0.0f, 1.2f);
          glRotatef(30, 1,0,0);
          glScalef(0.35f, 0.12f, 0.85f);
          glutSolidSphere(1.0, 16, 8);
        glPopMatrix();
      }
      // center
      glPushMatrix();
        setMaterial(0.9f, 0.6f, 0.1f, 32.0f);
        glTranslatef(0.0f, -0.1f, 0.0f);
        glutSolidSphere(0.5, 16, 8);
      glPopMatrix();
    glPopMatrix();
}

void drawMetro(float pos) {
    // elevated columns and rail
    // columns
    glPushMatrix();
      setMaterial(0.4f, 0.4f, 0.42f);
      for (float x=-100.0f; x<100.0f; x+=10.0f) {
        glPushMatrix();
          glTranslatef(x, 0.0f, -12.0f);
          drawBox(0.6f, 6.0f, 0.6f);
        glPopMatrix();
      }

      // beam
      setMaterial(0.2f, 0.2f, 0.25f);
      glPushMatrix();
        glTranslatef(0.0f, 3.5f, -12.0f);
        drawBox(120.0f, 0.6f, 1.8f);
      glPopMatrix();

      // metro train as moving box segments
      int carsCount = 6;
      for (int i=0;i<carsCount;i++) {
        float x = pos + i*5.5f;
        glPushMatrix();
          glTranslatef(x, 4.6f, -12.0f);
          setMaterial(0.8f, 0.05f, 0.05f, 12.0f);
          drawBox(4.6f, 1.6f, 1.4f);
          // windows
          glDisable(GL_LIGHTING);
          glColor3f(0.85f, 0.95f, 1.0f);
          glPushMatrix();
            glTranslatef(0.0f, 0.0f, 1.01f);
            glScalef(3.8f, 0.9f, 1.0f);
            glutSolidCube(0.7f);
          glPopMatrix();
          if (isDay) glEnable(GL_LIGHTING);
        glPopMatrix();
      }
    glPopMatrix();
}

void drawCar(const Car &c, bool headlight=false, bool taillight=false) {
    glPushMatrix();
      glTranslatef(c.x, 0.5f, c.z);
      if (c.heading != 0.0f)
        glRotatef(c.heading, 0,1,0);

      // body
      setMaterial(0.05f + 0.5f*( (int)(c.offset) % 3 )*0.2f, 0.05f, 0.2f, 8.0f);
      drawBox(1.8f, 0.8f, 0.8f);
      // windows
      glDisable(GL_LIGHTING);
      glColor3f(0.15f, 0.2f, 0.25f);
      glPushMatrix();
        glTranslatef(0.0f, 0.18f, 0.0f);
        glScalef(1.2f, 0.5f, 0.6f);
        glutSolidCube(0.6f);
      glPopMatrix();

      // lights
      if (!isDay && headlight) {
        glColor3f(1.0f, 0.9f, 0.7f);
        glBegin(GL_QUADS);
          glVertex3f(0.95f, -0.05f, -0.2f);
          glVertex3f(1.35f, -0.05f, -0.2f);
          glVertex3f(1.35f, 0.2f, -0.2f);
          glVertex3f(0.95f, 0.2f, -0.2f);
        glEnd();
      }
      if (!isDay && taillight) {
        glColor3f(1.0f, 0.0f, 0.0f);
        glBegin(GL_QUADS);
          glVertex3f(-0.95f, -0.05f, -0.2f);
          glVertex3f(-1.25f, -0.05f, -0.2f);
          glVertex3f(-1.25f, 0.2f, -0.2f);
          glVertex3f(-0.95f, 0.2f, -0.2f);
        glEnd();
      }
      if (isDay) glEnable(GL_LIGHTING);
    glPopMatrix();
}

void drawCity() {
    // ground plane
    setMaterial(0.2f, 0.6f, 0.2f);
    glPushMatrix();
      glTranslatef(0, -0.01f, 0);
      glBegin(GL_QUADS);
        glVertex3f(-200.0f, 0.0f, -200.0f);
        glVertex3f( 200.0f, 0.0f, -200.0f);
        glVertex3f( 200.0f, 0.0f,  200.0f);
        glVertex3f(-200.0f, 0.0f,  200.0f);
      glEnd();
    glPopMatrix();

    // Buildings (Motijheel-like cluster)
    drawBuilding(10, 0, 10, 6.0f, 24.0f, 6.0f);
    drawBuilding(18, 0, 12, 8.0f, 30.0f, 7.0f);
    drawBuilding(26, 0, 8, 5.0f, 18.0f, 5.0f);
    drawBuilding(34, 0, 14, 10.0f, 34.0f, 8.0f);
    drawBuilding(6, 0, -8, 7.0f, 20.0f, 7.0f);
    drawBuilding(-8, 0, 14, 6.0f, 28.0f, 5.0f);

    // metro + columns
    drawMetro(metroPos);

    // shapla chattar island & roundabout
    // roundabout
    glPushMatrix();
      setMaterial(0.1f, 0.45f, 0.1f);
      glTranslatef(-6.0f, 0.01f, 0.0f);
      for (int i=0;i<20;i++) {
        glBegin(GL_TRIANGLE_FAN);
          glVertex3f(0.0f, 0.01f, 0.0f);
          for (int a=0;a<=20;a++) {
            float ang = a*(2*PI/20.0f);
            glVertex3f(cos(ang)*3.5f, 0.01f, sin(ang)*3.5f);
          }
        glEnd();
      }
    glPopMatrix();
    drawShaplaSculpture();
}

// Sky ----------------------------------------------------
void drawSun() {
    glDisable(GL_LIGHTING);
    glPushMatrix();
      glTranslatef(35.0f, 50.0f, -80.0f);
      glScalef(6.0f,6.0f,6.0f);
      glutSolidSphere(1.0, 20, 20);
    glPopMatrix();
    if (isDay) glEnable(GL_LIGHTING);
}

void drawMoon() {
    glDisable(GL_LIGHTING);
    glPushMatrix();
      glTranslatef(30.0f, 48.0f, -80.0f);
      glScalef(5.0f,5.0f,5.0f);
      glColor3f(0.95f, 0.95f, 1.0f);
      glutSolidSphere(1.0, 20, 20);
    glPopMatrix();
    // subtle crescent (by overlaying darker disc)
    glDisable(GL_LIGHTING);
    glPushMatrix();
      glTranslatef(30.4f, 48.4f, -79.5f);
      glScalef(5.0f,5.0f,5.0f);
      glColor3f(0.05f, 0.05f, 0.08f);
      glutSolidSphere(0.6f, 20, 20);
    glPopMatrix();
    if (!isDay) glEnable(GL_LIGHTING);
}

void drawStars() {
    if (isDay) return;
    glDisable(GL_LIGHTING);
    glPointSize(2.0f);
    glBegin(GL_POINTS);
    glColor3f(1.0f, 1.0f, 1.0f);
    for (auto &s : stars) {
      glVertex3f(s.first, 50.0f, s.second);
    }
    glEnd();
    if (!isDay) glEnable(GL_LIGHTING);
}

// Lighting setup -----------------------------------------
void setupLights() {
    glEnable(GL_LIGHTING);
    glEnable(GL_NORMALIZE);

    if (isDay) {
      // Sun directional light
      GLfloat sunDir[] = { -0.3f, -0.6f, 0.0f, 0.0f }; // directional
      GLfloat sunDiffuse[] = { 0.98f, 0.94f, 0.85f, 1.0f };
      GLfloat sunAmbient[] = { 0.25f, 0.25f, 0.25f, 1.0f };
      GLfloat sunSpec[] = { 0.6f, 0.6f, 0.6f, 1.0f };

      glLightfv(GL_LIGHT0, GL_POSITION, sunDir);
      glLightfv(GL_LIGHT0, GL_DIFFUSE, sunDiffuse);
      glLightfv(GL_LIGHT0, GL_SPECULAR, sunSpec);
      glLightfv(GL_LIGHT0, GL_AMBIENT, sunAmbient);
      glEnable(GL_LIGHT0);

      // Soft fill light
      GLfloat fillPos[] = { 0.0f, 20.0f, 10.0f, 1.0f };
      GLfloat fillColor[] = { 0.25f, 0.28f, 0.35f, 1.0f };
      glLightfv(GL_LIGHT1, GL_POSITION, fillPos);
      glLightfv(GL_LIGHT1, GL_DIFFUSE, fillColor);
      glEnable(GL_LIGHT1);

      // Scene ambient
      GLfloat amb[] = {0.25f,0.25f,0.25f,1.0f};
      glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb);
    } else {
      // Night: dim ambient + moon light
      GLfloat amb[] = {0.05f,0.06f,0.08f,1.0f};
      glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb);

      GLfloat moonDir[] = { 0.3f, -0.5f, 0.2f, 0.0f };
      GLfloat moonDiffuse[] = { 0.45f, 0.47f, 0.6f, 1.0f };
      GLfloat moonSpec[] = { 0.25f, 0.25f, 0.30f, 1.0f };
      glLightfv(GL_LIGHT0, GL_POSITION, moonDir);
      glLightfv(GL_LIGHT0, GL_DIFFUSE, moonDiffuse);
      glLightfv(GL_LIGHT0, GL_SPECULAR, moonSpec);
      glEnable(GL_LIGHT0);

      // small fill
      GLfloat fillPos[] = { 0.0f, 10.0f, 0.0f, 1.0f };
      GLfloat fillColor[] = { 0.05f, 0.05f, 0.06f, 1.0f };
      glLightfv(GL_LIGHT1, GL_POSITION, fillPos);
      glLightfv(GL_LIGHT1, GL_DIFFUSE, fillColor);
      glEnable(GL_LIGHT1);
    }
}

// Draw scene ---------------------------------------------
void display() {
    glClearColor(isDay ? 0.53f : 0.03f, isDay ? 0.8f : 0.03f, isDay ? 0.92f : 0.08f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0f/zoom, (float)windowWidth / (float)windowHeight, 0.1f, 1000.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // camera transform
    float radYaw = camYaw * PI / 180.0f;
    float radPitch = camPitch * PI / 180.0f;
    float dirX = cos(radPitch) * sin(radYaw);
    float dirY = sin(radPitch);
    float dirZ = -cos(radPitch) * cos(radYaw);
    gluLookAt(camX, camY, camZ, camX + dirX, camY + dirY, camZ + dirZ, 0,1,0);

    setupLights();

    // Sky objects
    if (isDay) {
      drawSun();
    } else {
      drawMoon();
      drawStars();
    }

    // City and road
    drawCity();

    // Road in front
    glPushMatrix();
      glTranslatef(0.0f, 0.0f, 8.0f);
      drawRoad(80.0f, 6.0f);
      // cars (traffic jam)
      for (size_t i=0;i<cars.size(); ++i) {
        bool head = (i==0); // first car headlight on
        bool tail = true;
        drawCar(cars[i], head, tail);
      }
    glPopMatrix();

    // Another road in other direction
    glPushMatrix();
      glTranslatef(0.0f, 0.0f, -8.0f);
      drawRoad(80.0f, 6.0f);
      for (size_t i=0;i<cars.size(); ++i) {
        Car mirrored = cars[i];
        mirrored.z = -mirrored.z;
        mirrored.heading = 180.0f;
        drawCar(mirrored, (i==0), true);
      }
    glPopMatrix();

    glutSwapBuffers();
}

// Animation step -----------------------------------------
void update(int value) {
    // metro movement
    metroPos += metroSpeed;
    if (metroPos > 120.0f) metroPos = -120.0f;

    // cars: they slowly wiggle (traffic jam)
    for (size_t i=0;i<cars.size(); ++i) {
      Car &c = cars[i];
      // small oscillation to simulate inching
      float wiggle = 0.02f * sin((float)glutGet(GLUT_ELAPSED_TIME)/200.0f + i);
      c.x += wiggle;
      c.offset += 0.01f;
    }

    glutPostRedisplay();
    glutTimerFunc(16, update, 0); // ~60 FPS
}

// Input --------------------------------------------------
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
      case 27: exit(0); break;
      case 'd': case 'D': isDay = true; break;
      case 'n': case 'N': isDay = false; break;
      case ' ': isDay = !isDay; break;
      case '+': zoom *= 1.05f; break;
      case '-': zoom /= 1.05f; break;
      case 'w': camZ -= 1.0f; break;
      case 's': camZ += 1.0f; break;
      case 'j': camX -= 1.0f; break;
      case 'r': camX += 1.0f; break;
    }
    glutPostRedisplay();
}

void specialKeys(int key, int x, int y) {
    switch (key) {
      case GLUT_KEY_LEFT: camYaw -= 3.0f; break;
      case GLUT_KEY_RIGHT: camYaw += 3.0f; break;
      case GLUT_KEY_UP: camPitch += 2.0f; break;
      case GLUT_KEY_DOWN: camPitch -= 2.0f; break;
    }
    glutPostRedisplay();
}

void reshape(int w, int h) {
    windowWidth = w; windowHeight = h;
    glViewport(0,0,w,h);
}

// Setup initial cars & stars
void initCarsAndStars() {
    srand((unsigned int)time(NULL));
    cars.clear();
    // create clustered cars (traffic jam) along the road: x positions spaced, z lanes
    float startX = -30.0f;
    for (int i=0;i<18;i++) {
      Car c;
      c.x = startX + i * 2.4f + ( (i%3)==0 ? 0.12f : -0.08f );
      c.z = 1.4f + ((i%3)-1) * 1.2f; // lane offset
      c.heading = 0.0f;
      c.offset = (float) (rand()%10);
      cars.push_back(c);
    }
    // stars
    stars.clear();
    for (int i=0;i<150;i++) {
      float sx = (float)(rand()%200 - 100);
      float sz = (float)(rand()%200 - 100);
      stars.emplace_back(sx, sz);
    }
}

// OpenGL init
void initGL() {
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    GLfloat whiteSpec[] = {0.2f,0.2f,0.2f,1.0f};
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, whiteSpec);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 8.0f);

    initCarsAndStars();
}

// Main ---------------------------------------------------
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("Dhaka - Motijheel (Shapla Chattar) with Metro & Traffic - Day/Night");

    initGL();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    glutTimerFunc(16, update, 0);

    glutMainLoop();
    return 0;
}
