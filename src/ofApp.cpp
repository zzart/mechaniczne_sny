#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(ofColor::white);
    //ofSetWindowTitle("mechaniczne sny");
    //ofSetWindowShape(1920,1200);
    //ofSetFullscreen(true);
    ofSetFrameRate(60);
    //ofSetLogLevel(OF_LOG_VERBOSE);
    ofEnableAntiAliasing();

    initGui();
    showGui = false;
    scene = 0;
    recording = false;
}


//--------------------------------------------------------------
void ofApp::update(){
    if (scene == 1) scene1_update();
    if (scene == 2) scene2_update();
    if (scene == 3) scene3_update();
    if (scene == 4) scene4_update();
    if (scene == 5) scene5_update();
    if (scene == 6) scene6_update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(background);

    if (scene == 1) scene1_draw();
    if (scene == 2) scene2_draw();
    if (scene == 3) scene3_draw();
    if (scene == 4) scene4_draw();
    if (scene == 5) scene5_draw();
    if (scene == 6) scene6_draw();

    if (showGui) gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch (key){
        case '0':
            scene = 0;
            printf("-------------------------> scene %d activated\n" , scene);
            ofBackground(ofColor::black);
            break;

        case '1':
            ofResetElapsedTimeCounter();
            scene = 1;
            printf("------------------------->  scene %d activated\n" , scene);
            scene1_setup();
            break;

        case '2':
            // clear 1
            mesh.clear();
            ofResetElapsedTimeCounter();
            //end clear
            scene = 2;
            printf("------------------------->  scene %d activated\n" , scene);
            printf("Naciśnij dodatkowo spacje jak nic nie rysuje !!!! \n");
            addNoise = false;
            showPreview = false;
            scene2_setup();
            break;

        case '3':
            // clear 2
            line.clear();
            lines.clear();
            drawPoints.clear();
            ofResetElapsedTimeCounter();
            //end clear
            scene = 3;
            ofSetBackgroundAuto(true);
            printf("------------------------->  scene %d activated\n" , scene);
            scene3_setup();
            break;
        case '4':
            scene = 4;
            ofResetElapsedTimeCounter();
            printf("------------------------->  scene %d activated\n" , scene);
            scene4_setup();
            break;
        case '5':
            recording = false;
            ofResetElapsedTimeCounter();
            scene = 5;
            printf("------------------------->  scene %d activated\n" , scene);
            scene5_setup();
            break;
        case '6':
            scene = 6;
            ofResetElapsedTimeCounter();
            printf("------------------------->  scene %d activated\n" , scene);
            scene6_setup();
            break;
        case 'w':
            //exit();  not working
            break;
        case 'b':
            buildPicture();
            break;
        case 'g':
            showGui = !showGui;
            break;
        case 'x':
            addNoise = !addNoise;
            break;
        case 'p':
            showPreview = !showPreview;
            break;
        case 'z':
            ofBackground(background);
            line.clear();
            lines.clear();
            drawPoints.clear();
            break;
        case OF_KEY_RETURN:
            ofSaveScreen("shoot.png");
            break;
		case ' ':
			bLearnBakground = true;
			break;
		case '+':
			s2_threshold = s2_threshold + 1;
			if (s2_threshold > 255) s2_threshold = 255;
			break;
		case '-':
			s2_threshold = s2_threshold -1;
			if (s2_threshold < 0) s2_threshold = 0;
			break;
	}
}

//scene 1--------------------------------------------------------------
void ofApp::scene1_setup(){
    image.setImageType(OF_IMAGE_GRAYSCALE);
    image.load("schlemmer/schlemmer.png");
    image.setImageType(OF_IMAGE_COLOR_ALPHA);
    //image.allocate(ofGetWidth(), ofGetHeight(), OF_IMAGE_COLOR);

    mesh.setMode(OF_PRIMITIVE_POINTS);

    buildPicture();
    alpha = 255;
    beta = 0;
    modulation = 0.0;
    particle_color= 0;
}
void ofApp::scene1_update(){
    timer = ofGetElapsedTimeMillis()/1000;
    if (alpha - timer <= 0 ){
       if (timer == 5 * 60){
            modulation = 0.1;
        }
        if (timer == 6 * 60){
            modulation = 0.2;
        }
        if (timer == 7 * 60){
            modulation = 0.3;
        }
        if (timer == 8 * 60){
            modulation = 0.4;
        }
        if (timer == 9 * 60){
            modulation = 0.5;
        }
        if (timer == 11 * 60){
            modulation = 0.6;
        }
        if (timer == 12 * 60){
            modulation = 0.7;
        }

        for (auto &vert : mesh.getVertices()){
            vert.x += ofRandom(-modulation,modulation);
            vert.y += ofRandom(-modulation,modulation);
        }
    }
}

void ofApp::scene1_draw(){
    timer = ofGetElapsedTimeMillis()/1000;
    //printf("timer: %d\n" , timer );
    ofSetColor(ofColor::white, alpha - timer);
    image.draw(0,0, ofGetWidth(), ofGetHeight());
    if (timer < 10 * 60){
        ofSetColor((int)particle_color, int(beta));
        beta = beta + 0.10;
    }else if (timer > 11 * 60 ) {
        ofSetColor((int)particle_color, int(beta));
        beta = beta - 1;
    }else {
        ofSetColor((int)particle_color, int(beta));

    }

    //mesh.draw();
    for (auto &vert : mesh.getVertices()){
        ofDrawCircle(vert.x, vert.y,particle_fill);
    }
}

void ofApp::buildPicture(){
    mesh.clear();
    for (int x=0; x<image.getWidth(); x++){
        for (int y=0; y<image.getHeight(); y++){
          //int index = components * (x + image.getWidth() * y);
          //int grayScale = data[ index ];
          //Set red component modulated by u
          ofColor c = image.getColor(x, y);
          float intensity = c.getLightness();
          if (intensity <= s1_threshold) {
            mapw = ofMap(x,1,348, 1, ofGetWidth());
            maph = ofMap(y,1,394, 1, ofGetHeight());
            ofVec3f point(mapw,maph, 0);
            //ofVec3f point(x,y, 0);

            mesh.addVertex(point);
            //printf("added: %d %d\n", maph,mapw);
          }

        }
    }
}
//scene 2--------------------------------------------------------------

void ofApp::scene2_setup(){
    // camera
    VW = 320;
    VH = 240;
	#ifdef _USE_LIVE_VIDEO
        vidGrabber.setVerbose(true);
        vidGrabber.listDevices( );
        vidGrabber.setDeviceID( 1 );
        vidGrabber.initGrabber(VW,VH);
	#elif _USE_VIDEO
        vidPlayer.loadMovie("fingers.mov");
        vidPlayer.play();
    #else
        x=0,y=0;
        gst.setPipeline("tcpclientsrc host=192.168.1.3 port=5000 ! gdpdepay ! rtph264depay ! avdec_h264 ! videoconvert", OF_PIXELS_RGB, true, VW, VH);
        gst.startPipeline();
        //gst.setPixelFormat(OF_PIXEL_FORMAT_BGRA);
        tex.allocate(VW,VH,GL_RGB);
        gst.play();
	#endif


    colorImg.allocate(VW,VH);
	grayImage.allocate(VW,VH);
	grayBg.allocate(VW,VH);
	grayDiff.allocate(VW,VH);
	ofSleepMillis(5000);   //sleep for 5 sec
    bLearnBakground = true; //learn background
    ofEnableAlphaBlending();
    ofBackground(background);
    //ofSetBackgroundAuto(false);
    //ofDisableAntiAliasing();
    ofEnableSmoothing();
    ofSetFrameRate(30); // ??
}

void ofApp::scene2_update(){
    bool bNewFrame = false;

	#ifdef _USE_LIVE_VIDEO
       vidGrabber.update();
	   bNewFrame = vidGrabber.isFrameNew();
    #elif _USE_VIDEO
        vidPlayer.update();
        bNewFrame = vidPlayer.isFrameNew();
    #else
        gst.update();
        bNewFrame = gst.isFrameNew();
        //printf("isNew?  %d \n", bNewFrame);
	#endif

	if (bNewFrame){

		#ifdef _USE_LIVE_VIDEO
            colorImg.setFromPixels(vidGrabber.getPixels(), VW,VH);
        #elif _USE_VIDEO
            colorImg.setFromPixels(vidPlayer.getPixels(), VW,VH);
        #else
            colorImg.setFromPixels(gst.getPixels().getData(), VW, VH);
        #endif

        grayImage = colorImg;
		if (bLearnBakground == true){
			grayBg = grayImage;		// the = sign copys the pixels from grayImage into grayBg (operator overloading)
			bLearnBakground = false;
		}

		// take the abs value of the difference between background and incoming and then threshold:
		grayDiff.absDiff(grayBg, grayImage);
		grayDiff.threshold(s2_threshold);

		// find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
		// also, find holes is set to true so we will get interior contours as well....
		contourFinder.findContours(grayDiff, blob_size, (VW*VH)/3, blob_points, false);	// find holes
	}
}
void ofApp::scene2_draw(){
    //ofSetColor(color, transparency);
    //ofSetLineWidth(width);
    //ofSetLineWidth(ofRandom(1,5));
    //ofSleepMillis(1000);
    //if (addNoise) {
    //    for (auto &vert : line.getVertices()){
    //        vert.x += ofRandom(random - (2 * random) , random);
    //        vert.y += ofRandom(random - (2 * random) , random);
    //    }
    //}
    //line.draw();

    //----------------------------------------
    ofSetColor(s_color, s_transparency);
    ofSetLineWidth(s_width);


    if (addNoise){
        for (auto sline : lines) {
            int rand = ofRandom(1,255);
            if (rand >= s_random) ofDrawLine(sline.a, sline.b);
        }
    }
    else {
        for (auto sline : lines) {
            ofDrawLine(sline.a, sline.b);
        }
    }

    //for (auto cir: circle.getVertices()){
    //    ofNoFill();
    //    ofSetCircleResolution(40);
    //    ofSetColor(c_color, c_transparency);
    //    ofDrawCircle(cir.x, cir.y, c_radius);
    //}

    for (int i = 0; i < contourFinder.nBlobs; i++){
        //contourFinder.blobs[i].draw(360,540);
        //printf("countur: %f %f \n", contourFinder.blobs[i].boundingRect.getCenter().x, contourFinder.blobs[i].boundingRect.getCenter().y);
        //float cx = contourFinder.blobs[i].boundingRect.getCenter().x;
        //float cy = contourFinder.blobs[i].boundingRect.getCenter().y;
        cx = ofMap(contourFinder.blobs[i].boundingRect.getCenter().x, 1,VW, 1,ofGetWidth());
        cy = ofMap(contourFinder.blobs[i].boundingRect.getCenter().y, 1,VH, 1,ofGetHeight());
        ofNoFill();
        ofDrawCircle(cx, cy, 10);
        pointCollecter(cx, cy);

    }

    if (showPreview) displayPreview();
}
void ofApp::displayPreview(){
    //Camera
    //camera.draw(0,0);
    //camera.draw(0,0, ofGetWidth(), ofGetHeight());
	// draw the incoming, the grayscale, the bg and the thresholded difference
	ofSetHexColor(0xffffff);
	colorImg.draw(20,20);
	grayImage.draw(360,20);
	grayBg.draw(20,280);
	grayDiff.draw(360,280);

	// then draw the contours:

	ofFill();
	ofSetHexColor(0x333333);
	ofDrawRectangle(360,540,320,240);
	ofSetHexColor(0xffffff);

	// we could draw the whole contour finder
	//contourFinder.draw(360,540);

	// or, instead we can draw each blob individually from the blobs vector,
	// this is how to get access to them:
    for (int i = 0; i < contourFinder.nBlobs; i++){
        contourFinder.blobs[i].draw(360,540);

		// draw over the centroid if the blob is a hole
		ofSetColor(255);
		if(contourFinder.blobs[i].hole){
			ofDrawBitmapString("hole",
				contourFinder.blobs[i].boundingRect.getCenter().x + 360,
				contourFinder.blobs[i].boundingRect.getCenter().y + 540);
		}
    }

	// finally, a report:
	ofSetHexColor(0xffffff);
	stringstream reportStr;
	reportStr << "bg subtraction and blob detection" << endl
			  << "press ' ' to capture bg" << endl
			  << "threshold " << s2_threshold << " (press: +/-)" << endl
			  << "num blobs found " << contourFinder.nBlobs << ", fps: " << ofGetFrameRate();
	ofDrawBitmapString(reportStr.str(), 20, 600);

    // Debug
    ofDrawBitmapString(ofToString(ofGetFrameRate()), 10,10);

}
void ofApp::pointCollecter(float x, float y ){
    //ofPoint point;
    //point.set(x,y);
    //int r = ofRandom(1,70);
    //if (r == 40) line.addVertex(point);

    for(auto point: drawPoints){
        ofPoint mouse;
        mouse.set(x,y);
        float dist = (mouse - point).length();
        if (dist < 70){
            Line lineTemp;
            lineTemp.a = mouse;
            lineTemp.b = point;
            //lines.push_back(lineTemp);
            if (ofRandom(1,150) > 120) lines.push_back(lineTemp);
        }
        //if (dist < 20){
        //    ofPoint point;
        //    point.set(x,y);
        //    int rand = ofRandom(1,5000);
        //    if (rand == 100) circle.addVertex(point);
        //}
    }

    drawPoints.push_back(ofPoint(x,y));


    if (lines.size() > 4000){
        lines.clear();
        drawPoints.clear();
    }
    ofDrawBitmapString(ofToString(lines.size()), 30,40);
    //printf("poits %d ; lines %d\n", drawPoints.size(), lines.size());
}
//scene 3--------------------------------------------------------------
void ofApp::scene3_setup(){
    circle.clear();
    prev_point.set(0,0);
    ofSleepMillis(5000);   //sleep for 5 sec
    bLearnBakground = true; //learn background
    particle_count =1;

    ofPoint point;
    point.set(ofRandom(0,ofGetWidth()), ofRandom(0, ofGetHeight()));
    circle.addVertex(point);
    ofResetElapsedTimeCounter();

}
void ofApp::scene3_update(){
    scene2_update();
    timer = ofGetElapsedTimeMillis()/1000;
       if ((timer > particle_count) and (particle_count < 40 * 10)){
                ofPoint point;
                point.set(ofRandom(0,ofGetWidth()), ofRandom(0, ofGetHeight()));
                circle.addVertex(point);
                particle_count = particle_count + 10;
        }

	time_passed = ofGetElapsedTimef();
	float phase = 0.1 * ofGetElapsedTimef() * M_TWO_PI;
    float phase1 = 0.01 * ofGetElapsedTimef() * M_TWO_PI;
	float value = sin( phase );
	float value1 = ofNoise(phase);
    float value2 = sin( phase1 );
	kx = ofMap(value, -1, 1, 0, 255);
	ky = ofMap(value1, -1, 1, 0, 255);
    px = ofMap(value1, -1, 1, -ofGetWidth(), ofGetWidth());
    py = ofMap(value1, -1, 1, -ofGetHeight(), ofGetHeight());
    bg = ofMap(value2, -1, 1, 0, 255);
    for (auto &vert : circle.getVertices()){
            vert.x += ofRandom(-2,2);
            vert.y += ofRandom(-2,2);
       }
}

void ofApp::scene3_draw(){
    ofBackground(bg);
    ofSetColor(kx,20,20,ky);
    ofFill();
    for (auto vert: circle.getVertices()){
        ofDrawRectangle(vert.x , vert.y , 50,50);
        //printf("x:%d y:%d\n", vert.x, vert.y);
    }

    contourFinder.draw(0,0,ofGetWidth(), ofGetHeight());
    // for (int i = 0; i < contourFinder.nBlobs; i++){
    //     float cx = ofMap(contourFinder.blobs[i].boundingRect.getCenter().x, 1,VW, 1,ofGetWidth());
    //     float cy = ofMap(contourFinder.blobs[i].boundingRect.getCenter().y, 1,VH, 1,ofGetHeight());
    //     ofFill();
    //     contourFinder.draw(cx,cy);
    //     curr_point.set(cx,cy);
    //     float dist = (prev_point - curr_point).length();
    //     if(dist > distance){

    //         //ofDrawRectangle(cx,cy, 50,50);

    //         ofPoint prev_point;
    //         prev_point.set(cx,cy);
    //     }
    // }
    if (showPreview) displayPreview();
}
//scene 4--------------------------------------------------------------
void ofApp::scene4_setup(){
    typewriter.load("typewriter-key-1.wav");
    soundLevel = 0;
    recording = true;
    soundStream.setup( this, 0, 1, 44100, 512, 4 );
	ofSetVerticalSync(true);
//courbd
	unicodeFont.setup("cour.ttf", //font file, ttf only
					  1.0,					//lineheight percent
					  1024,					//texture atlas dimension
					  true,					//create mipmaps of the font, useful to scale down the font at smaller sizes
					  8,					//texture atlas element padding, shouldbe >0 if using mipmaps otherwise
					  1.5f					//dpi scaleup, render textures @2x the reso
					  );					//lower res mipmaps wil bleed into each other

	unicodeFont.setCharacterSpacing(0);
	keyRecorded = 0;
    mic_threshold =  0.31;
}
void ofApp::scene4_update(){}
void ofApp::scene4_draw(){
    ofBackground(ofColor::black);
	unicodeFont.setLineHeight(line_height);
	float x = 50;
	float y = 100;

	int numLines = 0;
	bool wordsWereCropped;
	ofSetColor((int)font_color, (int)font_alpha);
	unicodeFont.drawMultiLineColumn( s, font_size, x, y, ofGetWidth()-100, numLines, false, 99999, true, &wordsWereCropped, false);
    //column = unicodeFont.drawMultiLineColumn(	s,			/*string*/
	//											16,			/*size*/
	//											x, y,		/*where*/
	//											MAX( 10, mouseX - x), /*column width*/
	//											numLines,	/*get back the number of lines*/
	//											false,		/* if true, we wont draw (just get bbox back) */
	//											9,			/* max number of lines to draw, crop after that */
	//											true,		/*get the final text formatting (by adding \n's) in the supplied string;
	//														 BE ARWARE that using TRUE in here will modify your supplied string! */
	//											&wordsWereCropped, /* this bool will b set to true if the box was to small to fit all text*/
	//										 	true		/*centered*/
    //);

    // debug
    ofDrawBitmapString(ofToString(soundLevel), 30,40);
    if (ofGetKeyPressed()) {
    //if (soundLevel > mic_threshold) {
        ofDrawBitmapString("Mechaniczne Sny", 30,55);
        typeKey();
        typewriter.play();
    }
}
void ofApp::typeKey(){
	string sample = (string)"Z lepsza kondycja. Bardziej szczesliwie. Wydajniej. Komfortowo. Nie pic zbyt duzo. Regularne cwiczenia gimnastyczne (trzy dni w tygodniu). Poprawic stosunki z rownymi sobie wiekiem kolegami z pracy. Swobodnie. Dobrze sie odzywiac (koniec z obiadami przyrzadzonymi w mikrofalowce i daniami nasaczonymi tluszczem). Cierpliwy, lepszy kierowca. Bezpieczniejszy samochod (usmiechniete dziecko na tylnym siedzeniu). Spac mocno, dobrze (zadnych zlych snow). Zadnych paranoi. Uwazac na wszystkie zwierzeta (nigdy nie splukiwac pajakow w umywalce). Utrzymywac kontakt ze starymi przyjaciolmi (cieszyc sie drinkiem wypitym od czasu do czasu). Czesto sprawdzac swoj kredyt w (moralnym) banku (bankomacie). Przysluga za przysluge. Czuly, ale nie zakochany. Miec dobroczynny, milosierny regulamin. W niedziele obwodnica do supermarketu. (Nie zabijac ciem i nie wylewac wrzatku na mrowki). Jezdzic do myjni z samochodem (takze w niedziele). Nigdy wiecej nie bac sie ciemnosci. Ani cienia w poludnie. Nic mlodzienczo glupiego i desperackiego. Nic dziecinnego. Bez pospiechu. Wolniej i wiecej kalkulowac. Bez szans na ucieczke. Teraz pracujacy na wlasny rachunek. Zatroskany (ale bezsilny). Pelnoprawny i informowany czlonek spoleczenstwa (pragmatyk, ale nie idealista). Nie plakac na widoku publicznym. Mniejsza szansa na chorobe. Posiadac opony dobre do jazdy w czasie deszczu (sprawdzic czy dziecko na tylnym siedzeniu jest zapiete). Cwiczyc dobra pamiec. Nie przestawac plakac na dobrych filmach. Ani calowac sie z jezyczkiem. Nie czuc dluzej pustki i szalenstwa. Niczym kot. Przywiazany sznurem do kija. Ktory zdechl. Zamrozone, zimne gowno (umiejetnosc wysmiewania slabosci). Spokoj. Z lepsza kondycja, zdrowiej i wydajniej. Swinia. W klatce. Na antybiotykach.";

	if (keyRecorded > sample.size()) { return; }

	std::stringstream ss;
	for (size_t i = 0; i <keyRecorded; ++i)
	{
		if (i != 0)
			ss << "";
		ss << sample[i];
	}
	s = ss.str();
	//std::cout << s << endl;
	keyRecorded++;
	ofSleepMillis(100); // sleep so we don't get double strokes
}
void ofApp::audioReceived(float *input, int bufferSize, int nChannels){
    if (recording == true){
        double v = 0;
        for (int i=0; i<bufferSize; i++) {
            v += input[i] * input[i];
            }
            soundLevel = sqrt( v / bufferSize );
    }
}
//scene 5--------------------------------------------------------------
void ofApp::scene5_setup(){}
void ofApp::scene5_update(){}
void ofApp::scene5_draw(){}
//scene 6--------------------------------------------------------------
void ofApp::scene6_setup(){}
void ofApp::scene6_update(){}
void ofApp::scene6_draw(){}

//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------
void ofApp::keyReleased(int key){
}
//--------------------------------------------------------------

//--------------------------------------------------------------
//--------------------------------------------------------------

void ofApp::exit(){
    gui.saveToFile("settings.xml");
}
//--------------------------------------------------------------
//--------------------------------------------------------------
void ofApp::initGui(){
    //Gui
    gui.setup("Parameters", "settigns.xml");
    // scene 1
    gui.add( alpha.setup ("alpha", 255, 0, 255));
    gui.add( beta.setup ("beta", 255, 0, 255));
    gui.add( s1_threshold.setup ("s1_threshold", 250, 0, 255));
    gui.add( modulation.setup ("modulation", 0.0, 0, 10));
    gui.add( particle_color.setup ("particle_color", 0, 0, 255));
    gui.add( particle_fill.setup ("particle_fill", 1, 0, 255));
    // scene 2
    gui.add( background.setup ("background", 255, 0, 255));
    gui.add( transparency.setup ("transparency", 50, 0, 255));
    gui.add( width.setup ("width", 0, 0, 20));
    gui.add( color.setup("color",
                ofColor::black,
                ofColor(0,0,0),
                ofColor::white));
    gui.add( random.setup ("random", 1, 1, 20));
    gui.add( s_transparency.setup ("stransparency", 50, 0, 255));
    gui.add( s_width.setup ("swidth", 0, 0, 20));
    gui.add( s_color.setup("scolor",
                ofColor::black,
                ofColor(0,0,0),
                ofColor::white));
    gui.add( s_random.setup ("srandom", 50, 1, 255));
    gui.add( c_transparency.setup ("c_transparency", 50, 0, 255));
    gui.add( c_radius.setup ("c_radius", 10, 0, 70));
    gui.add( c_color.setup("color",
                ofColor::black,
                ofColor(0,0,0),
                ofColor::white));
    gui.add( s2_threshold.setup ("s2_threshold", 80, 0, 255));
    gui.add( blob_size.setup ("blob_size", 20, 0, 400));
    gui.add( blob_points.setup ("blob_points", 2, 0, 40));
    gui.setPosition(40, ofGetHeight()/2);

    //globalGroup.setup("Global");
    //globalGroup.add(Scale.setup("Scale", 1, 0.0,1));
    //globalGroup.add(Rotate.setup("Rotate", 0,-180,180));
    //globalGroup.add(Background.setup("Background", 255, 0,255));
    //gui.add(&globalGroup);
    // scene 3
    gui.add( distance.setup ("distance", 1000, 0, 10000));
    // scene 4
	gui.add(line_height.setup("line_height", 10, 0, 200));
	gui.add(mic_threshold.setup("mic_threshold", 0.01, 0.0, 1));
	gui.add(font_size.setup("font_size", 2, 0, 200));
	gui.add(font_color.setup("font_color", 0, 0, 255));
	gui.add(font_alpha.setup("font_alpha", 0, 0, 255));

    gui.loadFromFile("settings.xml");
}
