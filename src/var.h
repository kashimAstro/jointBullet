#include "ofAppGLFWWindow.h"
#include "ofMain.h"
#include "ofxBullet.h"
#include "ofxAnimatableFloat.h"
#include "ofxAnimatableOfPoint.h"
#include "ofxUI.h"

int NUM_ANIME = 0;
float position;
float fr;
float radius = 0.1;
int sphererad = 0;

ofxAnimatableFloat pos[NUM_ANIM_CURVES];
string curveNames[NUM_ANIM_CURVES];
ofxAnimatableOfPoint pointAnim;
ofxAnimatableFloat ball;
ofxBulletWorldRigid world;
ofxBulletBox* ground;
vector<ofxBulletBaseShape*> shapes;
vector<ofxBulletJoint*> joints;
ofShader cylinderShader,texture, hair;
ofImage img,img1;
ofCamera camera;
ofVec3f mousePos;
vector<float> sizeSpheres;

ofxUISuperCanvas *gui;
void guiEvent(ofxUIEventArgs &e);

float SizeSphere[4];
int floorLine = 500;
bool bDrawDebug;
bool bSpacebar;
bool bShapesNeedErase;
bool textureORhair=true;
float jointLength;
int w,h,fx,width;

