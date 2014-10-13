#include "var.h"

class xApp : public ofBaseApp{

	public:
		bool hideInfo = true;
                bool showInfo,paused=true;
		int xX,yY,zZ;
		int rR,gG,bB,aA;
		void exit(){
			delete gui;
		}

		void prepare_gui(){
			gui = new ofxUISuperCanvas("configure",OFX_UI_FONT_SMALL);
			gui->addSpacer();
			gui->addLabel("hide gui press (h)");
		        gui->addSlider("radius", 0.0, 2.0, 1.0);
			gui->addSlider("radius bone", 0, 2.0, 1.0);
			gui->addSpacer();
			gui->addButton("num anime",false);
			gui->addButton("separate",false);
			gui->addButton("add joint",false);
	        	gui->addButton("draw debug",false);
		        gui->addSlider("x", -1000, 1000, 100);
			gui->addSlider("y", -1000, 1000, 100);
			gui->addSlider("z", -1.5, 1.5, 1.0);
			gui->addButton("paused",false);

			gui->addSpacer();
			gui->autoSizeToFitWidgets();

		        ofAddListener(gui->newGUIEvent,this,&xApp::guiEvent);
		}

		void guiEvent(ofxUIEventArgs &e){
			string name = e.getName();
		        int kind = e.getKind();
		        cout << "event-from: " << name << endl;

			if(name == "x"){
				ofxUISlider *slider = e.getSlider();
		                xX=slider->getScaledValue();
			}
			if(name == "y"){
				ofxUISlider *slider = e.getSlider();
		                yY=slider->getScaledValue();
			}
			if(name == "z"){
				ofxUISlider *slider = e.getSlider();
		                zZ=slider->getScaledValue();
			}
			if(name == "paused"){
				ofxUIButton *button = (ofxUIButton *) e.widget; 
				if(button->getValue() == true){
					 paused = ! paused;
				}
			}
			if(name == "draw debug"){
				ofxUIButton *button = (ofxUIButton *) e.widget; 
				if(button->getValue() == true){
					 bDrawDebug = !bDrawDebug;
				}
			}
			if(name == "add joint"){
				ofxUIButton *button = (ofxUIButton *) e.widget; 
				if(button->getValue() == true)
					addJoint();
			}
			if(name == "num anime"){
				ofxUIButton *button = (ofxUIButton *) e.widget; 
				if(button->getValue() == true){
					NUM_ANIME+=1;
					if(NUM_ANIME==NUM_ANIM_CURVES)
						NUM_ANIME=0;
				}
			}
			if(name == "separate"){
				ofxUIButton *button = (ofxUIButton *) e.widget; 
				if(button->getValue() == true)
					bSpacebar = true;
			}

			if(name == "radius"){
				ofxUISlider *slider = e.getSlider();
		                radius=slider->getScaledValue();
			}
			if(name == "radius bone"){
				ofxUISlider *slider = e.getSlider();
		                sphererad=slider->getScaledValue();
			}
		}

		void drawWorm() {
			//line joints 
			ofPushMatrix();
		        ofSetColor(255,255,255);

			cylinderShader.begin();
			cylinderShader.setUniform1f("radius", radius);
		        for(int i = 1; i < joints.size(); i++) {
		           joints[i]->draw();
		        }
			cylinderShader.end();
			ofPopMatrix();
			//

			//bone
			ofPushStyle();

			texture.begin();
			texture.setUniformTexture("texture", img, 1);
			texture.setUniform1f("time", ofGetElapsedTimef());
			for(int i=0;i<shapes.size(); i++) {
			   ofPushMatrix();
			   ofDrawCone(   shapes[i]->getPosition().x, shapes[i]->getPosition().y-0.6, shapes[i]->getPosition().z, sphererad+sizeSpheres[i], 1.2 );		
			   ofPopMatrix();
                           ofDrawSphere( shapes[i]->getPosition(), sphererad+sizeSpheres[i] );
                        }
			texture.end();

			ofPopStyle();
			//
		}

		void setup() {
			prepare_gui();
			ofDisableArbTex();
			img.loadImage("img1.jpg");
			img1.loadImage("img.jpg");


			w=ofGetScreenWidth();
			h=ofGetScreenHeight();
			SizeSphere[0]=0.4;
			SizeSphere[1]=0.6;
			SizeSphere[2]=0.2;
			SizeSphere[3]=0.9;
			sizeSpheres.push_back(SizeSphere[(int)round(ofRandom(0, 3))]); 
			////////////////////////
		        width = 10;
		        ofxAnimatableFloat f;
		        f.animateFromTo( 10, w );
		        f.setDuration(2);
		        f.setRepeatType( LOOP_BACK_AND_FORTH );
		        for ( int i = 0; i < NUM_ANIM_CURVES; i++ ){
		                pos[i] = f;
		                AnimCurve curve = (AnimCurve) ( EASE_IN_EASE_OUT + i );
		                pos[i].setCurve( curve );
		                curveNames[i] = ofxAnimatable::getCurveName( curve );
		        }
		
		        ball.reset(300);
		        ball.setCurve(EASE_IN);
		        ball.setRepeatType(LOOP_BACK_AND_FORTH);
		        ball.setDuration(0.55);
		        ball.animateTo( floorLine );
        		pointAnim.setPosition( ofPoint(100,100) );
		        pointAnim.setRepeatType(PLAY_ONCE);
		        pointAnim.setCurve(EASE_IN_EASE_OUT);
			////////////////////////
			cylinderShader.setGeometryInputType(GL_LINES);
		        cylinderShader.setGeometryOutputType(GL_TRIANGLE_STRIP);
		        cylinderShader.setGeometryOutputCount((5 + 1) * (4 + 1) * 2);
		        cylinderShader.load("shaders/vert.glsl", "shaders/frag.glsl", "shaders/geom_needle.glsl");
		        printf("Maximum number of output vertices support is: %i\n", cylinderShader.getGeometryMaxOutputCount());

                        texture.load("shaders/material.vert","shaders/material.frag");

		        ofSetFrameRate(60);
		        ofSetVerticalSync(true);

		        camera.setPosition(ofVec3f(0, -6.f, 20.f));
		        camera.lookAt(ofVec3f(0, 0, 0), ofVec3f(0, -1, 0));

		        world.setup();
		        world.setCamera(&camera);
		        ground = new ofxBulletBox();
		        ground->create( world.world, ofVec3f(0., 5.5, 0.), 0., 150., 1.f, 150.f );
		        ground->setProperties(.25, .95);
		        ground->add();

		        jointLength = .30f;

		        shapes.push_back( new ofxBulletSphere() );
		        ((ofxBulletSphere*)shapes[0])->create( world.world, ofVec3f(0, 2, 0), 0.4f, .5f );
		        shapes[0]->add();
			
        		joints.push_back( new ofxBulletJoint() );
		        joints[joints.size()-1]->create( world.world, shapes[0], ofVec3f(0, 0, 0));
		        joints[joints.size()-1]->add();

		        bDrawDebug       = false;
		        bSpacebar        = false;
		        bShapesNeedErase = false;
			//ofHideCursor();
		}

		void update() {
			float dt = 1.0f / ofGetFrameRate();
			for ( int i = 0; i < NUM_ANIM_CURVES; i++ ) pos[i].update( dt );
		        ball.update( dt );
		        pointAnim.update( dt );
		        if ( !pointAnim.isOrWillBeAnimating() ){ pointAnim.animateToAfterDelay( ofPoint( ofRandom(0, ofGetWidth()), ofRandom(0, ofGetHeight())), 0.5); }
		        float a = 0.5 + 0.5 * sin( 0.1 * ofGetFrameNum());
		        float b = 0.5 + 0.5 * sin( 0.1 * ofGetFrameNum() + 400);
		        pos[QUADRATIC_BEZIER_PARAM].setQuadraticBezierParams( a, b);
		        float steep = 0.5 + 0.5 * sin( 0.1 * ofGetFrameNum());
		        pos[EXPONENTIAL_SIGMOID_PARAM].setDoubleExpSigmoidParam( steep );

		        world.update();
		        ofSetWindowTitle(ofToString(ofGetFrameRate(), 0));
			////////////////////////////////
			if(paused)
		        	mousePos = camera.screenToWorld( ofVec3f( pos[NUM_ANIME].val()+xX, ball.val()-yY,0-zZ) );
			else
	                        mousePos = camera.screenToWorld( ofVec3f( xX, yY, zZ) );
            		////////////////////////////////
		        joints[0]->updatePivotPos( mousePos, 2.f );
		        if(bSpacebar) {
		                for (int i = 1; i < joints.size(); i++) {
		                        delete joints[i];
		                }
		                joints.erase(joints.begin()+1, joints.end());
		                bSpacebar = false;
		                bShapesNeedErase = true;
		        }
		         
		        for (int i = 1; i < joints.size(); i++) {
		                if(i == 1) {
		                        joints[i]->updatePivotPos( shapes[i-1]->getPosition(), jointLength*2.f );
		                } else {
		                        joints[i]->updatePivotPos( shapes[i-1]->getPosition(), jointLength );
		                }
		        }
		}

		void draw() {
		        ofBackgroundGradient( ofColor(210), ofColor(10));

		        camera.begin();
			glEnable( GL_DEPTH_TEST );
        		glBlendFunc(GL_ONE, GL_ZERO);

        		ofSetLineWidth(1.f);
		        if(bDrawDebug) { world.drawDebug(); }
	                /*
                        ofPushMatrix();
			ofRotateY(ofGetFrameNum());
		        ofSetColor(100, 100, 100);
			ground->draw();
			ofPopMatrix();
                        */
			drawWorm();
		        glDisable(GL_DEPTH_TEST);
		        camera.end();

                        if(hideInfo) ofDrawBitmapStringHighlight(curveNames[NUM_ANIME] + "  vel: " + ofToString( pos[NUM_ANIME].getCurrentSpeed(), 2),220,20);
			
		}

		void keyPressed(int key) {
		        switch (key) {
				case 'h':
					gui->toggleVisible();
					hideInfo=!hideInfo;
					break;
				case 'f':
					ofToggleFullscreen();
					break;
		                default:
		                        break;
		        }
		}

		void addJoint() {
		        if(bShapesNeedErase) {
		                for (int i = 1; i < shapes.size(); i++) {
        	                	delete shapes[i];
	                	}
		                shapes.erase( shapes.begin()+1, shapes.end() );
		                bShapesNeedErase = false;
		                sizeSpheres.erase(sizeSpheres.begin()+1, sizeSpheres.end());
        		}

		        float rsize = ofRandom(.15, .3);
	
		        ofVec3f diff;
		        if(shapes.size() < 2) {
		                diff = mousePos - shapes[0]->getPosition();
		        } else {
		                diff = shapes[shapes.size()-2]->getPosition()- shapes[shapes.size()-1]->getPosition();
		        }
		        diff.normalize();
		        if(shapes.size() < 2) {
		                diff *= -(jointLength*2.f);
		        } else {
		                diff *= -jointLength;
		        }
		        diff += shapes[shapes.size()-1]->getPosition();
		        shapes.push_back( new ofxBulletSphere() );
		        ((ofxBulletSphere*)shapes[shapes.size()-1])->create( world.world, diff, .8, rsize );
		        shapes[shapes.size()-1]->add();

			sizeSpheres.push_back(SizeSphere[(int)round(ofRandom(0, 3))]); 

		        joints.push_back( new ofxBulletJoint() );
		        joints[joints.size()-1]->create(world.world, shapes[shapes.size()-1], shapes[shapes.size()-2]->getPosition());
		        joints[joints.size()-1]->add();
		}

};

int main(){
       ofAppGLFWWindow window;
       ofSetupOpenGL(&window, 1024,768, OF_WINDOW);
       ofRunApp( new xApp());
}
