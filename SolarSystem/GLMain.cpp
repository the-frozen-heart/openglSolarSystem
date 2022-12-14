#include "pch.h"
#include "GLRequire.h"

// 初始化摄像机
GLCamera camera(EYE_POS_X, EYE_POS_Y, EYE_POS_Z, VIEW_ANGLE);

// 文字显示器
GLFontMaker *fontMaker;

// 天空盒
GLSkyBox *skybox;
float rotateAngle = 0.0f;
GLuint skybox_texture_id[6];

// 太阳光晕纹理
GLuint sun_light;
float rotate_light = 0.0f;

// 行星
Planet *planet[PLANET_COUNT];
GLuint texture_id[PLANET_COUNT];

// 界面显示设置
bool needRadius = true; // 是否绘制轨道
bool needText	= false; // 是否显示文字
int tanslateSpeed = 1; // 播放速度
int ViewAngle = VIEW_ANGLE;
float ViewDown = VIEW_DOWN;

//  材质表面光照反射设置
GLfloat mat_ambient_box[] = { 1.0f, 1.0f, 1.0f, 1.0f };// 天空盒
GLfloat mat_diffuse_box[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat mat_specular_box[] = { 0.0f, 0.0f, 0.0f, 1.0f };

GLfloat mat_ambient_sun[]  = { 1.0f, 1.0f, 1.0f, 1.0f };// 太阳
GLfloat mat_diffuse_sun[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat mat_specular_sun[] = { 0.0f, 0.0f, 0.0f, 1.0f };

GLfloat mat_ambient_planet[]  = { 0.2f, 0.2f, 0.2f, 1.0f };// 普通行星
GLfloat mat_diffuse_planet[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat mat_specular_planet[] = { 0.1f, 0.1f, 0.1f, 1.0f };

// 星球的名字
const char* names[] = { "Sun", "Mercury", "Venus", "Earth", "Moon", "Mars", "Jupiter", "Saturn", "Uranus", "Neptune", "Pluto", "Moon", "Moon", "Moon", "Moon", "Moon", "Moon", "Moon", "Moon", "Moon" };

// 材质设置
void GLSetMateria(GLfloat ambient[], GLfloat diffuse[], GLfloat specular[])
{
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);// 设置环境光成分
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);// 设置镜面反射光成分
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);// 设置漫反射光成分
}

//  光照设置
void GLSetLight()
{
	GLfloat light_position[] = { 0.0f, 0.0f, 0.0f, 1.0f }; // 光源位置
	GLfloat light_ambient[] = { 1.0f, 1.0f, 1.0f, 0.0f }; // 环境光
	GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 0.0f }; // 镜面反射光

	glLightfv(GL_LIGHT1, GL_POSITION, light_position); // 设置光源位置
	glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);  // 设置光源环境光成分
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);  // 设置光源镜面反射光成分

	GLfloat light_position2[] = { 0.0f, 0.0f, 200.0f, 1.0f }; // 光源位置
	GLfloat light_ambient2[] = { 1.0f, 1.0f, 1.0f, 0.0f }; // 环境光
	GLfloat light_diffuse2[] = { 1.0f, 1.0f, 1.0f, 0.0f }; // 镜面反射光

	glLightfv(GL_LIGHT2, GL_POSITION, light_position2); // 设置光源位置
	glLightfv(GL_LIGHT2, GL_AMBIENT, light_ambient2);  // 设置光源环境光成分
	glLightfv(GL_LIGHT2, GL_DIFFUSE, light_diffuse2);  // 设置光源镜面反射光成分



	glEnable(GL_LIGHTING); // 启动光照
	glEnable(GL_LIGHT1); // 开启光源
	glEnable(GL_LIGHT2);
}

// 音乐设置
void GLSetMusic()
{
	MCI_OPEN_PARMS op;
	op.dwCallback = NULL;
	op.lpstrAlias = NULL;
	op.lpstrDeviceType = "MPEGAudio";
	op.lpstrElementName = "..\\Music\\music.mp3";
	op.wDeviceID = NULL;
	UINT rs;
	rs = mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT, (DWORD)&op);
	if (rs == 0)
	{
		MCI_PLAY_PARMS pp;
		pp.dwCallback = NULL;
		pp.dwFrom = 0;
		mciSendCommand(op.wDeviceID, MCI_PLAY, MCI_NOTIFY, (DWORD)&pp);
	}

}

// 初始化数据
void GLInitData()
{
	// 初始化文字显示器
	fontMaker = new GLFontMaker();
	
	// 申请纹理对象 ID
	glGenTextures(PLANET_COUNT, texture_id);

	// 初始化各星球数据		纹理图片路径，			公转速度	自转速度	公转半径	自身半径	中心天体	纹理ID
	planet[ 0] = new Planet("..\\texture\\太阳.png", 0.0f,		0.1f,		 0.0f,		15.0f,		NULL,		texture_id[ 0]);
	planet[ 1] = new Planet("..\\texture\\水星.png", 0.20f,		0.4f,		25.0f,		2.5f,		planet[0],	texture_id[ 1]);
	planet[ 2] = new Planet("..\\texture\\金星.png", 0.15f,		0.5f,		38.0f,		5.0f,		planet[0],	texture_id[ 2]);
	planet[ 3] = new Planet("..\\texture\\地球.png", 0.12f,		0.4f,		58.8f,		5.5f,		planet[0],	texture_id[ 3]);
	planet[ 4] = new Planet("..\\texture\\月球.png",-0.20f,		0.6f,		10.0f,		2.0f,		planet[3],	texture_id[ 4]);
	planet[ 5] = new Planet("..\\texture\\火星.png", 0.08f,		0.7f,		79.0f,		5.2f,		planet[0],	texture_id[ 5]);
	planet[ 6] = new Planet("..\\texture\\木星.png", 0.14f,		0.5f,		120.0f,		11.0f,		planet[0],	texture_id[ 6]);
	planet[ 7] = new Planet("..\\texture\\土星.png", 0.09f,		0.4f,		145.0f,		7.5f,		planet[0],	texture_id[ 7]);
	planet[ 8] = new Planet("..\\texture\\天王.png", 0.13f,		0.5f,		170.0f,		6.6f,		planet[0],	texture_id[ 8]);
	planet[ 9] = new Planet("..\\texture\\海王.png", 0.16f,		0.5f,		195.0f,		6.3f,		planet[0],	texture_id[ 9]);
	planet[10] = new Planet("..\\texture\\冥王.png", 0.11f,		0.5f,		220.0f,		6.0f,		planet[0],	texture_id[10]);
	planet[11] = new Planet("..\\texture\\月球.png", -0.20f, 0.6f, 17.0f, 2.0f, planet[0], texture_id[11]);
	planet[12] = new Planet("..\\texture\\月球.png", -0.20f, 0.6f, 10.0f, 2.0f, planet[1], texture_id[12]);
	planet[13] = new Planet("..\\texture\\月球.png", -0.20f, 0.6f, 10.0f, 2.0f, planet[2], texture_id[13]);
	planet[14] = new Planet("..\\texture\\月球.png", -0.20f, 0.6f, 10.0f, 2.0f, planet[5], texture_id[14]);
	planet[15] = new Planet("..\\texture\\月球.png", -0.20f, 0.6f, 14.0f, 2.0f, planet[6], texture_id[15]);
	planet[16] = new Planet("..\\texture\\月球.png", -0.20f, 0.6f, 10.0f, 2.0f, planet[7], texture_id[16]);
	planet[17] = new Planet("..\\texture\\月球.png", -0.20f, 0.6f, 10.0f, 2.0f, planet[8], texture_id[17]);
	planet[18] = new Planet("..\\texture\\月球.png", -0.20f, 0.6f, 10.0f, 2.0f, planet[9], texture_id[18]);
	planet[19] = new Planet("..\\texture\\月球.png", -0.20f, 0.6f, 10.0f, 2.0f, planet[10], texture_id[19]);

	// 申请纹理对象 ID
	glGenTextures(6, skybox_texture_id);
	// 天空盒纹理路径
	const char* skybox_file[6] = {
		"..\\texture\\bk_front.png",
		"..\\texture\\bk_back.png",
		"..\\texture\\bk_left.png",
		"..\\texture\\bk_right.png",
		"..\\texture\\bk_top.png",
		"..\\texture\\bk_bottom.png"
	};
	// 构建天空盒
	skybox = new GLSkyBox(EYE_POS_X, EYE_POS_Y, EYE_POS_Z, skybox_file, skybox_texture_id, SKYBOX_SIZE);

	// 太阳光晕纹理导入
	glGenTextures(1, &sun_light);
	unsigned int width, height;
	std::vector<unsigned char> sun;
	unsigned int error = lodepng::decode(sun, width, height, "..\\texture\\sun_light.png");
	if (error != 0)
	{
		std::cout << "error " << error << ": " << lodepng_error_text(error) << std::endl;
		return;
	}
	glBindTexture(GL_TEXTURE_2D, sun_light);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 4, width, height, GL_RGBA, GL_UNSIGNED_BYTE, &sun[0]);

	// 开启深度测试
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	// 开启混合
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glShadeModel(GL_SMOOTH); // 设置阴影模式

}

// 更新视角和显示位置
void UpdateFrame()
{
	int width = fontMaker->width;
	int height = fontMaker->height;
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(ViewAngle, (GLdouble)width / (GLdouble)height, 0.1, 1600.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(
		camera.EYE_X, camera.EYE_Y, camera.EYE_Z,
		camera.EYE_X + sin(camera.VIEW_POS), camera.EYE_Y + ViewDown, camera.EYE_Z - cos(camera.VIEW_POS),
		0.0, 1.0, 0.0
	);
	
}

// 界面UI文字绘制
void DrawUI()
{
	const int lines = 9;
	char buf[lines][128];
	snprintf(buf[0], 128, "ProjectName: SolarSystem");
	snprintf(buf[1], 128, "@CopyRight: RecoAaron");
	snprintf(buf[2], 128, "Position - X: %.3f", camera.EYE_X);
	snprintf(buf[3], 128, "Position - Y: %.3f", camera.EYE_Y);
	snprintf(buf[4], 128, "Position - Z: %.3f", camera.EYE_Z);
	snprintf(buf[5], 128, "PlanetaryOrbit( F3 ): %s", (needRadius) ? "show" : "hide");
	snprintf(buf[6], 128, "DisplaySpeed( F4 ): %d", tanslateSpeed);
	snprintf(buf[7], 128, "Perspective:( F6 - F7 ): %d", ViewAngle);
	snprintf(buf[8], 128, "View Angle( F8 - F9 ): %3f", ViewDown);
	for (int i = 0; i < lines; i++)
	{
		fontMaker->PrintTextScreen(camera, 0.2f, 0.5f + i * 0.5f, buf[i]);
	}
	UpdateFrame();
}

// 绘制天空盒
void RenderSky()
{
	// 调整为天空盒材质属性
	GLSetMateria(mat_ambient_box, mat_diffuse_box, mat_specular_box);

	glPushMatrix(); // 保存当前模型视图矩阵
	// 移动坐标系
	glTranslatef(skybox->POS_X, skybox->POS_Y, skybox->POS_Z);
	glRotatef(rotateAngle, 0.0f, 1.0f, 0.0f);
	glEnable(GL_TEXTURE_2D);
	skybox->DrawScence(); // 绘制天空盒
	glDisable(GL_TEXTURE_2D);
	rotateAngle -= 0.01f;
	if (rotateAngle < -360)
		rotateAngle = 0;
	glPopMatrix(); // 重置模型视图矩阵
}

// 绘制太阳
void RenderSun()
{
	// 绘制太阳光晕
	//glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, sun_light);
	glPushMatrix();
	//glRotatef(45, 0, 1, 0);
	glRotatef(-camera.VIEW_POS / EDGE_ANGLE, 0.0f, 1.0f, 0.0f);
	glRotatef(atan(ViewDown) / EDGE_ANGLE, 1.0f, 0.0f, 0.0f);
	glRotatef(rotate_light, 0.0f, 0.0f, 1.0f);
	rotate_light += 0.2f;
	if (rotate_light > 360)
		rotate_light = 0.0f;
	glColor4f(1.0f, 0.6f, 0.0f, 1.0f);
	glBegin(GL_QUADS);
	float size = planet[0]->SelfRadius*1.4f;
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-size, -size, 0.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f( size, -size, 0.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f( size,  size, 0.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-size,  size, 0.0f);
	glEnd();
	glPopMatrix();
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);

	GLSetLight();
	// 调整为太阳材质属性
	GLSetMateria(mat_ambient_sun, mat_diffuse_sun, mat_specular_sun);
	glPushMatrix(); // 保存当前模型视图矩阵
	glEnable(GL_TEXTURE_2D);
	planet[0]->DrawPlanet(needRadius, tanslateSpeed); // 绘制太阳
	glDisable(GL_TEXTURE_2D);
	if (needText)
		fontMaker->PrintText(0, planet[0]->SelfRadius + 1.0f, 0, names[0]);// 绘制字体
	glPopMatrix(); // 重置模型视图矩阵
}

// 绘制各大行星
void RenderPlanet()
{
	for (int i = 1; i < PLANET_COUNT; i++)
	{
		// 调整为普通行星的材质属性
		GLSetMateria(mat_ambient_planet, mat_diffuse_planet, mat_specular_planet);
		glPushMatrix();// 保存当前模型视图矩阵
		glEnable(GL_TEXTURE_2D);
		planet[i]->DrawPlanet(needRadius, tanslateSpeed);// 绘制星球
		glDisable(GL_TEXTURE_2D);
		if (needText)
			fontMaker->PrintText(0, planet[i]->SelfRadius + 1.0f, 0, names[i]);// 绘制字体
		glPopMatrix();// 重置模型视图矩阵
	}
}

// 场景绘制函数
void GLRenderScence()
{
	// 清除深度缓存和颜色缓存
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glShadeModel(GL_SMOOTH);

	//如果开启，则显示UI文字
	if (needText)
		DrawUI();

	// 启动光照
	GLSetLight();

	// 开起光源
	glEnable(GL_LIGHT0);
	RenderSky();
	RenderSun();
	glDisable(GL_LIGHT0);
	RenderPlanet();

	// 刷新视图
	glutSwapBuffers();
}

// 窗口重绘函数
void GLReshape(GLsizei width, GLsizei height)
{
	if (height == 0)
		height = 1;
	fontMaker->width = width;
	fontMaker->height = height;
	UpdateFrame();
}

// 键盘交互函数
void GLControler(int key, int x, int y)
{
	// 记录偏移量
	float offset_right		= 0; // 向右偏移量
	float offset_up			= 0; // 向上偏移量
	float offset_forward	= 0; // 向前偏移量
	float view_pos			= 0; // 旋转偏移量

	switch (key)
	{
		// 方向键 left
		case GLUT_KEY_LEFT:
			offset_right = -0.01f * MOVE_SPEED;
			break;
		// 方向键 right
		case GLUT_KEY_RIGHT:
			offset_right = 0.01f * MOVE_SPEED;
			break;
		// 方向键 up
		case GLUT_KEY_UP:
			offset_forward = 0.01f * MOVE_SPEED;
			break;
		// 方向键 down
		case GLUT_KEY_DOWN:
			offset_forward = -0.01f * MOVE_SPEED;
			break;
		// 按键 PgUp
		case GLUT_KEY_PAGE_UP:
			offset_up = 0.01f * JUMP_SPEED;
			break;
		// 按键 PgDn
		case GLUT_KEY_PAGE_DOWN:
			offset_up = -0.01f * JUMP_SPEED;
			break;
		// 按键 F1
		case GLUT_KEY_F1:
			view_pos = -0.01f * ROTATE_SPEED;
			break;
		// 按键 F2
		case GLUT_KEY_F2:
			view_pos = 0.01f * ROTATE_SPEED;
			break;
		// 按键 F3 启用/关闭轨道显示
		case GLUT_KEY_F3:
			needRadius = !needRadius;
			break;
		// 按键 F4 切换运动速度（暂停，低速，正常，加速）
		case GLUT_KEY_F4:
			tanslateSpeed++;
			tanslateSpeed %= 4;
			break;
		// 按键 F5 是否显示屏幕文字
		case GLUT_KEY_F5:
			needText = !needText;
			break;
		// 按键 F6 视角变小（放大）
		case GLUT_KEY_F6:
			if (ViewAngle > VIEW_ANGLE_MIN)
				ViewAngle--;
			break;
		// 按键 F7 视角变小（放大）
		case GLUT_KEY_F7:
			if (ViewAngle < VIEW_ANGLE_MAX)
				ViewAngle++;
			break;
		// 按键 F8 视角上移
		case GLUT_KEY_F8:
			if (ViewDown > VIEW_DOWN_MIN)
				ViewDown -= 0.1f;
			break;
			// 按键 F8 视角下移
		case GLUT_KEY_F9:
			if (ViewDown < VIEW_DOWN_MAX)
				ViewDown += 0.1f;
			break;
		case GLUT_KEY_F10:
			//F10开启抗锯齿
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glEnable(GL_BLEND);
			// 点抗锯齿
			glEnable(GL_POINT_SMOOTH);
			/*
			 GL_NICEST:效果最好
			 GL_FASTEST:效果最快
			 */
			glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
			// 线抗锯齿
			glEnable(GL_LINE_SMOOTH);
			glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
			// 多边形抗锯齿
			glEnable(GL_POLYGON_SMOOTH);
			glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
			break;
		case GLUT_KEY_F11:
			// 关闭抗锯齿处理
			glDisable(GL_BLEND);
			glDisable(GL_POINT_SMOOTH);
			glDisable(GL_LINE_SMOOTH);
			glDisable(GL_POLYGON_SMOOTH);
			break;
	}
	// 摄像机移动
	camera.GLMove(offset_right, offset_up, offset_forward);
	// 摄像机旋转
	camera.GLRotate(view_pos);
	// 更新天空盒位置
	skybox->UpdatePosition(camera.EYE_X, camera.EYE_Y, camera.EYE_Z);
	UpdateFrame();
}

// Main 线程
int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GL_DISPLAY_MODE);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(GL_WINDOW_WIDTH, GL_WINDOW_HEIGHT);
	glutCreateWindow("SolarSystem");
	glClearColor(1.0, 1.0, 1.0, 1.0);

	GLInitData();
	GLSetMusic();


	glutDisplayFunc(GLRenderScence);
	glutReshapeFunc(GLReshape);
	glutSpecialFunc(GLControler);
	glutIdleFunc(GLRenderScence);

	glutMainLoop();
	return 0;
}