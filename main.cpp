#if defined(WIN32)
//#  pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")
#  include "glut.h"
#elif defined(__APPLE__) || defined(MACOSX)
#  include <GLUT/glut.h>
#else
#  define GL_GLEXT_PROTOTYPES
#  include <GL/glut.h>
#endif

/*
** 境界箱
*/
static GLdouble pmin[] = { -1.0, -1.0, -1.0 };
static GLdouble pmax[] = {  1.0,  1.0,  1.0 };

/*
** 画面表示
*/
static void display(void)
{
  GLdouble range = pmax[2] - pmin[2];
  GLdouble offset = 0.6;
  
  /* 境界箱を視野空間に設定する */
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(pmin[0], pmax[0], pmin[1], pmax[1], offset, range + offset);
  
  /* 視点を境界箱の前方面の位置に移動する */
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslated(0.0, 0.0, -pmax[2]);
  
  /* 画面消去 */
  glClear(GL_COLOR_BUFFER_BIT);
  
  /* 論理演算処理開始 */
  glEnable(GL_COLOR_LOGIC_OP);
  
  /* 図形表示 */
  glutSolidTeapot(0.5);
  
  /* 論理演算処理終了 */
  glDisable(GL_COLOR_LOGIC_OP);
  
  glFlush();
}

/*
** 初期設定
*/
static void init(void)
{
  /* 背景色 */
  glClearColor(0.0, 0.0, 0.0, 1.0);
  
  /* 前景色 */
  glColor3d(1.0, 1.0, 1.0);
  
  /* フレームバッファに書きこむたびにフレームバッファの内容を反転 */
  glLogicOp(GL_INVERT);
  
  /* 隠面消去処理は行わない */
  glDisable(GL_DEPTH_TEST);
}

/*
** メインプログラム
*/
int main(int argc, char *argv[])
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH);
  glutCreateWindow(argv[0]);
  glutDisplayFunc(display);
  init();
  glutMainLoop();
  
  return 0;
}
