////////////////////////////////////////
// tester.cpp
////////////////////////////////////////

#include "tester.h"

#define WINDOWTITLE	"Quaternion Rotation -- Selected Box: %d -- Mode: %s"

#define PARTICLES_PER_SIDE 25
#define WEIGHT 10.0f
#define DRAG 12.0f
#define K_S 205.0f
#define K_D 0.20f
#define LENGTH 2.0f

////////////////////////////////////////////////////////////////////////////////

static Tester *TESTER;

int t = 0;
bool pause = false;

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	TESTER = new Tester(argc,argv);
	glutMainLoop();
	return 0;
}

////////////////////////////////////////////////////////////////////////////////

// These are really HACKS to make glut call member functions instead of static functions
static void display()									{TESTER->Draw();}
static void idle()										{TESTER->Update();}
static void resize(int x,int y)							{TESTER->Resize(x,y);}
static void keyboard(unsigned char key,int x,int y)		{TESTER->Keyboard(key,x,y);}
static void mousebutton(int btn,int state,int x,int y)	{TESTER->MouseButton(btn,state,x,y);}
static void mousemotion(int x, int y)					{TESTER->MouseMotion(x,y);}

////////////////////////////////////////////////////////////////////////////////

Tester::Tester(int argc,char **argv) {
	WinX=1280;
	WinY=960;
	LeftDown=MiddleDown=RightDown=false;
	MouseX=MouseY=0;
    

	// Create the window
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize( WinX, WinY );
	glutInitWindowPosition( 0, 0 );
	WindowHandle = glutCreateWindow( WINDOWTITLE );
	glutSetWindowTitle( WINDOWTITLE );
	glutSetWindow( WindowHandle );
    glEnable(GL_DEPTH_TEST);
    

	// Background color
	glClearColor(0.5, 0.5, 0.5, 1.0f);

	// Callbacks
	glutDisplayFunc( display );
	glutIdleFunc( idle );
	glutKeyboardFunc( keyboard );
	glutMouseFunc( mousebutton );
	glutMotionFunc( mousemotion );
	glutPassiveMotionFunc( mousemotion );
	glutReshapeFunc( resize );

    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    
    // Initialize components
    GLfloat light_diffuse1[] = {0.75f,0.75f,0.75f,1.0f};
    GLfloat light_position1[] = {2.0, 2.0f, -3.0f,1.0f};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse1);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position1);
    
    GLfloat light_diffuse2[] = {0.5f,0.12f,0.12f,1.0f};
    GLfloat light_position2[] = {-2.0, 2.0f, 3.0f,1.0f};
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse2);
    glLightfv(GL_LIGHT1, GL_POSITION, light_position2);
    
	Cam.SetAspect(float(WinX)/float(WinY));
    
    /*
    if (argc > 1) {
        this->skeleton = new Skeleton(argv[1], argv[2], "skin.vert", "skin.frag");
    } else {
        this->skeleton = new Skeleton(
                    "/Users/aantic/Documents/cse169/skeletons/wasp.skel",
                    "/Users/aantic/Documents/cse169/skins/wasp.skin",
                    "/Users/aantic/Documents/cse169/shaders/skin.vert",
                    "/Users/aantic/Documents/cse169/shaders/skin.frag");
        this->animation = Animation::FromFile("/Users/aantic/Documents/cse169/animations/wasp_walk.anim");
    }
    */
    
    /*
    this->cloth = new Cloth(   PARTICLES_PER_SIDE
                            , WEIGHT
                            , DRAG
                            , K_S
                            , K_D
                            , LENGTH
                            );
     */
    rotsim = new RotationSim();
}

////////////////////////////////////////////////////////////////////////////////

Tester::~Tester() {
	glFinish();
	glutDestroyWindow(WindowHandle);
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Update() {
	// Update the components in the world
    int curtime = glutGet(GLUT_ELAPSED_TIME);
    char formatted_title[128];
//    sprintf(formatted_title, WINDOWTITLE, cloth->getWindSpeed(), glm::degrees(cloth->getWindAngle()), (float) (curtime - t) / 1000.0f);
    if (rotsim->getMode() == SLERP)
        sprintf(formatted_title, WINDOWTITLE, rotsim->getSelected(), "SLERP");
    else
        sprintf(formatted_title, WINDOWTITLE, rotsim->getSelected(), "CATMULL_ROM");
    glutSetWindowTitle(formatted_title);
    
	Cam.Update();
    
    
//    std::vector<float> pose = animation->GetPose(t);
//    skeleton->Update(pose);
    if (!pause)
        rotsim->Update(0.02);
        
    
    t = curtime;

	// Tell glut to re-display the scene
	glutSetWindow(WindowHandle);
	glutPostRedisplay();
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Reset() {
	Cam.Reset();
	Cam.SetAspect(float(WinX)/float(WinY));

}

////////////////////////////////////////////////////////////////////////////////

void Tester::Draw() {

	// Begin drawing scene
	glViewport(0, 0, WinX, WinY);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    

	// Draw components
	Cam.Draw();		// Sets up projection & viewing matrices
    rotsim->Draw();

	// Finish drawing scene
	glFinish();
	glutSwapBuffers();
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Quit() {
	glFinish();
    delete rotsim;
	glutDestroyWindow(WindowHandle);
	exit(0);
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Resize(int x,int y) {
	WinX = x;
	WinY = y;
	Cam.SetAspect(float(WinX)/float(WinY));
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Keyboard(int key,int x,int y) {
    float rad = glm::radians(1.0f);
	switch(key) {
		case 0x1b:		// Escape
			Quit();
			break;
		case 'r':
			Reset();
			break;
        case 'w':
//            cloth->ChangeWindSpeed(0.1f);
            break;
        case 's':
//            cloth->ChangeWindSpeed(-0.1f);
            break;
        case 'a':
//            cloth->ChangeWindAngle(glm::radians(1.0f));
            rotsim->prevKey();
            break;
        case 'd':
//            cloth->ChangeWindAngle(glm::radians(-1.0f));
            rotsim->nextKey();
            break;
        case 'p':
            pause = !pause;
            break;
        case 'i': // move cloth up
//            cloth->MoveCloth(glm::vec3(0.0f, 3.0f, 0.0f));
            rotsim->rotateBy(0.0f, rad, 0.0f);
            break;
        case 'k': // move cloth down
//            cloth->MoveCloth(glm::vec3(0.0f, -3.0f, 0.0f));
            rotsim->rotateBy(0.0f, -rad, 0.0f);
            break;
        case 'j': // move cloth left
//            cloth->MoveCloth(glm::vec3(-3.0f, 0.0f, 0.0f));
            rotsim->rotateBy(-rad, 0, 0.0f);
            break;
        case 'l': // move cloth right
//            cloth->MoveCloth(glm::vec3(3.0f, 0.0f, 0.0f));
            rotsim->rotateBy(rad, 0.0f, 0.0f);
            break;
        case 'o': // move cloth in
//            cloth->MoveCloth(glm::vec3(0.0f, 0.0f, -3.0f));
            rotsim->rotateBy(0.0f, 0.0f, -rad);
            break;
        case 'm': // move cloth out
//            cloth->MoveCloth(glm::vec3(0.0f, 0.0f, 3.0f));
            rotsim->rotateBy(0.0f, 0.0f, rad);
            break;
        case '.':
            pause = false;
            this->Update();
            pause = true;
            break;
        case 't':
            rotsim->toggleMode();
            break;
    }
}

////////////////////////////////////////////////////////////////////////////////

void Tester::MouseButton(int btn,int state,int x,int y) {
	if(btn==GLUT_LEFT_BUTTON) {
		LeftDown = (state==GLUT_DOWN);
	}
	else if(btn==GLUT_MIDDLE_BUTTON) {
		MiddleDown = (state==GLUT_DOWN);
	}
	else if(btn==GLUT_RIGHT_BUTTON) {
		RightDown = (state==GLUT_DOWN);
	}
}

////////////////////////////////////////////////////////////////////////////////

void Tester::MouseMotion(int nx,int ny) {
	int dx = nx - MouseX;
	int dy = -(ny - MouseY);

	MouseX = nx;
	MouseY = ny;

	// Move camera
	// NOTE: this should really be part of Camera::Update()
	if(LeftDown) {
		const float rate=1.0f;
		Cam.SetAzimuth(Cam.GetAzimuth()+dx*rate);
		Cam.SetIncline(Cam.GetIncline()-dy*rate);
	}
	if(RightDown) {
		const float rate=0.01f;
		Cam.SetDistance(Cam.GetDistance()*(1.0f-dy*rate));
	}
}

////////////////////////////////////////////////////////////////////////////////
