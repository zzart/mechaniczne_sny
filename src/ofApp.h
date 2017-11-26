#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxOpenCv.h"
#include "ofxFontStash.h"
#include <vector>
#include <string>

//#define _USE_VIDEO		// uncomment this to use Raspberry PI camera
#define _USE_LIVE_VIDEO		// uncomment this to use a live camera
//#define _USE_NETWORK_VIDEO		// uncomment this to use a live camera
class Line {
    public:
        ofPoint a;
        ofPoint b;
};

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void exit();
		void clear_resource();
		void displayPreview();
		void initGui();
		void pointCollecter(float x, float y);

		void scene1_setup();
		void scene2_setup();
		void scene3_setup();
		void scene4_setup();
		void scene5_setup();
		void scene6_setup();

		void scene1_draw();
		void scene2_draw();
		void scene3_draw();
		void scene4_draw();
		void scene5_draw();
		void scene6_draw();

		void scene1_update();
		void scene2_update();
		void scene3_update();
		void scene4_update();
		void scene5_update();
		void scene6_update();

		void keyPressed(int key);
		void keyReleased(int key);
		//void mouseMoved(int x, int y );
		//void mouseDragged(int x, int y, int button);
		//void mousePressed(int x, int y, int button);
		//void mouseReleased(int x, int y, int button);
		//void windowResized(int w, int h);
		//void dragEvent(ofDragInfo dragInfo);
		//void gotMessage(ofMessage msg);
    ofxPanel gui;

    // Scene 1  -------------------------------------------------------
		void buildPicture();
		ofImage image;
        //ofPolyline s1_line;
        ofMesh mesh;
        float timer;

        int mapw;
        int maph;
        ofxIntSlider alpha;
        ofxFloatSlider beta;
        ofxIntSlider s1_threshold;
        ofxIntSlider background;
        ofxIntSlider particle_fill;
        ofxIntSlider particle_color;
        ofxFloatSlider modulation;
    // Scene 2  -------------------------------------------------------
        // camera & openCV
        #ifdef _USE_LIVE_VIDEO
		  ofVideoGrabber 		vidGrabber;
		#elif _USE_VIDEO
		  ofVideoPlayer 		vidPlayer;
		#else
            ofGstVideoUtils gst;
            ofTexture tex;
            int x,y;
		#endif
        int VW;
        int VH;

        ofxCvColorImage			colorImg;

        ofxCvGrayscaleImage 	grayImage;
		ofxCvGrayscaleImage 	grayBg;
		ofxCvGrayscaleImage 	grayDiff;

        ofxCvContourFinder 	contourFinder;
        float cx,cy;
		bool bLearnBakground;
		//ofEasyCam cam;

        // drawing
		ofPolyline line;
		ofPolyline circle;
		bool showGui;
		bool addNoise;
		bool showPreview;
		int scene = 0;
		vector < ofPoint > drawPoints;
		vector < Line > lines;

    // GUI ---------------------
        ofxIntSlider transparency;
        ofxFloatSlider width;
        ofxColorSlider color;
        ofxIntSlider random;
        ofxIntSlider s_transparency;
        ofxFloatSlider s_width;
        ofxColorSlider s_color;
        ofxIntSlider s_random;
        ofxFloatSlider c_radius;
        ofxColorSlider c_color;
        ofxIntSlider c_transparency;
        ofxIntSlider s2_threshold;
        ofxIntSlider blob_size;
        ofxIntSlider blob_points;
    // Scene 3  -------------------------------------------------------

		float time_passed;
		int kx;
		int ky;
		int px;
		int py;
		int bg;
		int particle_count;
        ofPoint curr_point;
        ofPoint prev_point;
        ofxIntSlider distance;
    // Scene 4  -------------------------------------------------------
		//void drawPoint(float x, float y);
		void typeKey();
		bool recording;

        ofSoundPlayer typewriter;
		ofxFontStash font;
		ofxFontStash unicodeFont;
		ofxIntSlider line_height;
		ofxIntSlider font_size;
		ofxIntSlider font_color;
		ofxIntSlider font_alpha;
		ofxFloatSlider mic_threshold;
		int keyRecorded;
		//std::string s[300];
		std::string s;
		std::vector <char> keyArr;

		void audioReceived( float *input, int bufferSize, int nChannels );
		ofSoundStream soundStream;
		float soundLevel;

};
