
#include "Fileutil.h"
#include "global.h"
#include "Prefab.h"
#include "Scene.h"
#include "glew.h"
#include "glut.h"
#pragma comment(lib, "glew32.lib")


ShaderManager* usd_ShaderManager;
TextureManager* usd_TextureManager;
MeshManager* usd_MeshManager;
MateriaManager* usd_MateriaManager;
Scene* main_scene;


void SysInit()
{
	//场景最后创建
	usd_ShaderManager=new ShaderManager();
	usd_TextureManager=new TextureManager();
	usd_MeshManager=new MeshManager();
	usd_MateriaManager=new MateriaManager();
	main_scene=new Scene();

	Object* R= Prefab_Loader("1.p",glm::vec3(0.01,0.01,0.01));
	Object* Plane= Prefab_Loader("Plane.p",glm::vec3(0.1,0.1,0.1));
	main_scene->AddObj(R);
	main_scene->AddObj(Plane);

}

void SysEnd()
{
	if(main_scene!=NULL)
		delete main_scene;
	//记住退出顺序很重要因为材质管理器有用到其他资源管理器所以最早退出
	if(usd_MateriaManager!=NULL)
		delete usd_MateriaManager;
	if(usd_MeshManager!=NULL)
		delete usd_MeshManager;
	if(usd_TextureManager!=NULL)
		delete usd_TextureManager;
	if(usd_ShaderManager!=NULL)
		delete usd_ShaderManager;
}

void changeSize(int width, int height) 
{
  if (height == 0)
    return;

  glViewport(0, 0, (GLsizei) width, (GLsizei) height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(90.0, width/height, 1.0, 100.0);

  glMatrixMode(GL_MODELVIEW);
}
void renderScene()
{

  main_scene->Updata();
  glutSwapBuffers();
	
}

void MouseHandler(int button, int state, int x, int y)
{
  switch (button)
  {
  case GLUT_LEFT_BUTTON:
    {
		main_scene->GetCamera()->T_UP(-0.1);
		
    } break;
  case GLUT_RIGHT_BUTTON:
    {
		main_scene->GetCamera()->T_UP(0.1);
    } break;

  default:
    break;
  }
  // force a screen redraw
  glutPostRedisplay();
} // end MouseHandler()
void keyboard(unsigned char key, int x, int y) 
{
	switch(key) 
	{
		case 'W' :
			main_scene->GetCamera()->T_Forward(-1);
			break;
		case 'S' : 
			main_scene->GetCamera()->T_Forward(1);
			break;
		case 'A' : 
			main_scene->GetCamera()->T_Right(-1);
			break;
		case 'D' : 
			main_scene->GetCamera()->T_Right(1);
			break;
    }
}





int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA| GLUT_MULTISAMPLE);
	glutInitWindowPosition(100,0);
	glutInitWindowSize(800,600);
	glutCreateWindow("DEMO");
	
	//初始化glew扩展库
	GLenum err = glewInit();
	if( GLEW_OK != err )
	{
		std::cout <<"Error initializing GLEW: " << glewGetErrorString(err) << std::endl;

	}
	SysInit();
	glEnable(GL_DEPTH_TEST);

	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutMouseFunc(MouseHandler);
	glutKeyboardFunc(keyboard); 
	glutMainLoop();
	SysEnd();
	return 0;
}
/*
int main(int argc, char* argv[])
{
	std::string filename="E:/cura/Printrun-Win-Slic3r-03Feb2015/te.gcode";
	std::string run ="E:/cura/Printrun-Win-Slic3r-03Feb2015/pronsole.exe -e connect -e \"load E:/cura/Printrun-Win-Slic3r-03Feb2015/te.gcode\" -e print";
	system(run.c_str());
	int m;
	std::cin>>m;
    return 0;
}
*/


