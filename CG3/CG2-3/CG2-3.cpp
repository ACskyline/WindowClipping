#include<GL\glut.h>
#include<iostream>

int* pointXYArray;
int pointNumber;
int left, right, down, up;
int cleft, cright, cdown, cup;

struct Edge{
	int xmin;//y��С���xֵ����դ����������ʾ
	int ymax;//y�����yֵ
	float m;//б�ʵĵ���
	float xmath;//��ѧ�������ڼ���
	int ymin;//y��С���yֵ
};

struct EdgeNode{
	Edge edge;
	EdgeNode* next;
};

int Round(float x)//��������
{
	if (x - (int)x >= 0.5){
		return (int)x + 1;
	}
	else{
		return (int)x;
	}
}

void FillPolygon(const int* PointXYArray, int PointNumber)
{
	Edge *E = new Edge[PointNumber];
	EdgeNode *AEL = 0;
	EdgeNode **ET = 0;
	int AELcount = 0;
	int Ecount = 0;
	int yMin = PointXYArray[1];//ɨ����½�
	int yMax = PointXYArray[1];//ɨ����Ͻ�
	//����ߵ���Ϣ
	for (int i = 0; i < PointNumber; i++){//��i����������2*i,2*i+1,��Ӧ��i���ߵ����
		//����ˮƽ��
		if (PointXYArray[2 * i + 1] != PointXYArray[(2 * i + 3) % (2 * PointNumber)]){//��i����͵�i+1�����������ˮƽ��
			if (PointXYArray[2 * i + 1] <= PointXYArray[(2 * i + 3) % (2 * PointNumber)]){//�����i�����yֵС�ڵ��ڵ�i+1�����yֵ
				E[Ecount].xmin = PointXYArray[2 * i];
				E[Ecount].ymin = PointXYArray[2 * i + 1];
				E[Ecount].ymax = PointXYArray[(2 * i + 3) % (2 * PointNumber)];
			}
			else{
				E[Ecount].xmin = PointXYArray[(2 * i + 2) % (2 * PointNumber)];
				E[Ecount].ymin = PointXYArray[(2 * i + 3) % (2 * PointNumber)];
				E[Ecount].ymax = PointXYArray[2 * i + 1];
			}
			E[Ecount].m = ((float)PointXYArray[2 * i] - (float)PointXYArray[(2 * i + 2) % (2 * PointNumber)]) / ((float)PointXYArray[2 * i + 1] - (float)PointXYArray[(2 * i + 3) % (2 * PointNumber)]);
			E[Ecount].xmath = E[Ecount].xmin;
			Ecount++;
		}
		//ȷ�����ж���������yֵ����С��yֵ
		if (yMax < PointXYArray[2 * i + 1]){
			yMax = PointXYArray[2 * i + 1];
		}
		if (yMin > PointXYArray[2 * i + 1]){
			yMin = PointXYArray[2 * i + 1];
		}
	}
	//Ԥ���������ҷֲ������������
	for (int c = 0; c < Ecount; c++){
		if (E[c].ymin == E[(c + 1) % Ecount].ymax){
			E[c].xmath += E[c].m;
			E[c].xmin = Round(E[c].xmath);
			E[c].ymin++;
		}
		else if (E[c].ymax == E[(c + 1) % Ecount].ymin){
			E[c].ymax--;
		}
	}
	//printf("yMin,yMax = %d,%d\n", yMin, yMax);
	/*for (int h = 0; h < PointNumber; h++){
		printf("n,xmin,ymin,ymax,m = %d,%d,%d,%d,%f\n", h, E[h].xmin, E[h].ymin, E[h].ymax, E[h].m);
		}*/
	//��ʼ��ET
	ET = new EdgeNode*[yMax - yMin + 1];//��ET��0��yMax - yMinӳ���yMin��yMax
	for (int i = 0; i < yMax - yMin + 1; i++){
		ET[i] = 0;
	}
	for (int i = 0; i < Ecount; i++){
		if (ET[E[i].ymin - yMin] == 0){
			ET[E[i].ymin - yMin] = new EdgeNode();
			ET[E[i].ymin - yMin]->edge = E[i];
			ET[E[i].ymin - yMin]->next = 0;
		}
		else{
			EdgeNode*p = ET[E[i].ymin - yMin];
			while (p->next != 0){ p = p->next; }
			p->next = new EdgeNode();
			p->next->edge = E[i];
			p->next->next = 0;
		}
	}
	/*//��ӡET�Ա�debug
	EdgeNode **ptemp = ET;
	EdgeNode *qtemp =0;
	for (int v = 0; v < yMax - yMin + 1; v++){
	qtemp = ptemp[v];
	while (qtemp){
	printf("y,xmin,ymin,ymax,m:%d,%d,%d,%d,%f\n", v, qtemp->edge.xmin, qtemp->edge.ymin, qtemp->edge.ymax, qtemp->edge.m);
	qtemp = qtemp->next;
	}
	}
	*/
	//ɨ����ѭ��
	for (int i = yMin; i <= yMax; i++){
		//�ӱ�
		EdgeNode*p = ET[i - yMin];
		while (p){
			if (AEL){
				EdgeNode*q = AEL;
				while (q->next){ q = q->next; }
				q->next = new EdgeNode;
				q->next->edge = p->edge;
				q->next->next = 0;
			}
			else{
				AEL = new EdgeNode;
				AEL->edge = p->edge;
				AEL->next = 0;
			}
			AELcount++;
			p = p->next;
		}
		//����
		int* xPixel = new int[AELcount];
		EdgeNode* m = AEL;
		int n = 0;
		while (m != 0)//�ջ�xmin
		{
			xPixel[n] = m->edge.xmin;
			m = m->next;
			n++;
		}
		for (int a = 0; a < AELcount; a++){//��xmin����
			for (int b = a + 1; b < AELcount; b++){
				if (xPixel[a]>xPixel[b]){
					int temp = xPixel[a];
					xPixel[a] = xPixel[b];
					xPixel[b] = temp;
				}
			}
		}
		//printf("y,AELcount:%d,%d\n", i, AELcount);
		/*
		for (int xt = 0; xt < AELcount; xt++){
		printf("xPixel[%d]:%d\n", xt, xPixel[xt]);
		}*/
		//����
		if (AEL){
			int start = 0;
			int end = 1;
			while (start < AELcount&&end < AELcount){
				int temp = xPixel[start];
				while (temp <= xPixel[end]){
					glBegin(GL_POINTS);
					glVertex3f(temp, i, 0);
					glEnd();
					temp++;
				}
				start += 2;
				end += 2;
			}
		}
		else{
			printf("AEL is empty");
		}
		//ɾ��
		EdgeNode* k = AEL;
		EdgeNode* l = 0;
		while (k){
			l = k->next;
			if (l){
				if (l->edge.ymax <= i){
					k->next = l->next;
					delete l;//��ɾͷ���֮���
					AELcount--;
				}
			}
			k = k->next;
		}
		k = AEL;
		if (k){
			if (k->edge.ymax <= i){
				AEL = k->next;
				delete k;//��ɾͷ���
				AELcount--;
			}
		}
		//��xin
		EdgeNode* c = AEL;
		while (c){
			c->edge.xmath += c->edge.m;
			c->edge.xmin = Round(c->edge.xmath);
			c = c->next;
		}
	}
	//ɾ��ET
	EdgeNode*r = 0;
	EdgeNode*o = 0;
	for (int i = 0; i < yMax - yMin + 1; i++){
		r = ET[i];
		while (r){
			o = r;
			r = r->next;
			delete o;
		}
	}
	delete[] ET;
	//ɾ��E
	delete[] E;
}

void ClipPolygon(int** PointXYArray, int *PointNumber, int left, int right, int down, int up)
{
	int PointNumberTemp = *PointNumber;
	int* PointXYArrayTemp = *PointXYArray;
	for (int side = 0; side < 4; side++){
		int count = 0;
		int *tempArray = new int[PointNumberTemp * 4];
		//һ������ʼ
		for (int i = 0; i < PointNumberTemp; i++){
			if (side == 0)//��
			{
				if (PointXYArrayTemp[2 * i] < left&&PointXYArrayTemp[2 * (i + 1) % (2 * PointNumberTemp)] >= left)//out-in
				{
					//v0'
					tempArray[2 * count] = left;
					tempArray[2 * count + 1] = Round(PointXYArrayTemp[2 * i + 1] + (PointXYArrayTemp[(2 * (i + 1) + 1) % (2 * PointNumberTemp)] - PointXYArrayTemp[2 * i + 1])*(left - PointXYArrayTemp[2 * i]) / (PointXYArrayTemp[2 * (i + 1) % (2 * PointNumberTemp)] - PointXYArrayTemp[2 * i]));
					//v1
					tempArray[2 * (count + 1)] = PointXYArrayTemp[2 * (i + 1) % (2 * PointNumberTemp)];
					tempArray[2 * (count + 1) + 1] = PointXYArrayTemp[(2 * (i + 1) + 1) % (2 * PointNumberTemp)];
					count += 2;
				}
				else if (PointXYArrayTemp[2 * i] >= left&&PointXYArrayTemp[2 * (i + 1) % (2 * PointNumberTemp)] >= left)//in-in
				{
					//v1
					tempArray[2 * count] = PointXYArrayTemp[2 * (i + 1) % (2 * PointNumberTemp)];
					tempArray[2 * count + 1] = PointXYArrayTemp[(2 * (i + 1) + 1) % (2 * PointNumberTemp)];
					count++;
				}
				else if (PointXYArrayTemp[2 * i] >= left&&PointXYArrayTemp[2 * (i + 1) % (2 * PointNumberTemp)] < left)//in-out
				{
					//v0'
					tempArray[2 * count] = left;
					tempArray[2 * count + 1] = Round(PointXYArrayTemp[2 * i + 1] + (PointXYArrayTemp[(2 * (i + 1) + 1) % (2 * PointNumberTemp)] - PointXYArrayTemp[2 * i + 1])*(left - PointXYArrayTemp[2 * i]) / (PointXYArrayTemp[2 * (i + 1) % (2 * PointNumberTemp)] - PointXYArrayTemp[2 * i]));
					count++;
				}
				else if (PointXYArrayTemp[2 * i] < left&&PointXYArrayTemp[2 * (i + 1) % (2 * PointNumberTemp)]<left)//out-out
				{
					//do nothing
				}
			}
			else if (side == 1)//��
			{
				if (PointXYArrayTemp[2 * i]>right&&PointXYArrayTemp[2 * (i + 1) % (2 * PointNumberTemp)] <= right)//out-in
				{
					//v0'
					tempArray[2 * count] = right;
					tempArray[2 * count + 1] = Round(PointXYArrayTemp[2 * i + 1] + (PointXYArrayTemp[(2 * (i + 1) + 1) % (2 * PointNumberTemp)] - PointXYArrayTemp[2 * i + 1])*(right - PointXYArrayTemp[2 * i]) / (PointXYArrayTemp[2 * (i + 1) % (2 * PointNumberTemp)] - PointXYArrayTemp[2 * i]));
					//v1
					tempArray[2 * (count + 1)] = PointXYArrayTemp[2 * (i + 1) % (2 * PointNumberTemp)];
					tempArray[2 * (count + 1) + 1] = PointXYArrayTemp[(2 * (i + 1) + 1) % (2 * PointNumberTemp)];
					count += 2;
				}
				else if (PointXYArrayTemp[2 * i] <= right&&PointXYArrayTemp[2 * (i + 1) % (2 * PointNumberTemp)] <= right)//in-in
				{
					//v1
					tempArray[2 * count] = PointXYArrayTemp[2 * (i + 1) % (2 * PointNumberTemp)];
					tempArray[2 * count + 1] = PointXYArrayTemp[(2 * (i + 1) + 1) % (2 * PointNumberTemp)];
					count++;
				}
				else if (PointXYArrayTemp[2 * i] <= right&&PointXYArrayTemp[2 * (i + 1) % (2 * PointNumberTemp)] > right)//in-out
				{
					//v0'
					tempArray[2 * count] = right;
					tempArray[2 * count + 1] = Round(PointXYArrayTemp[2 * i + 1] + (PointXYArrayTemp[(2 * (i + 1) + 1) % (2 * PointNumberTemp)] - PointXYArrayTemp[2 * i + 1])*(right - PointXYArrayTemp[2 * i]) / (PointXYArrayTemp[2 * (i + 1) % (2 * PointNumberTemp)] - PointXYArrayTemp[2 * i]));
					count++;
				}
				else if (PointXYArrayTemp[2 * i] > right&&PointXYArrayTemp[2 * (i + 1) % (2 * PointNumberTemp)] > right)//out-out
				{
					//do nothing
				}
			}
			else if (side == 2)//��
			{
				if (PointXYArrayTemp[2 * i + 1] < down&&PointXYArrayTemp[(2 * (i + 1) + 1) % (2 * PointNumberTemp)] >= down)//out-in
				{
					//v0'
					tempArray[2 * count] = Round(PointXYArrayTemp[2 * i] + (PointXYArrayTemp[2 * (i + 1) % (2 * PointNumberTemp)] - PointXYArrayTemp[2 * i])*(down - PointXYArrayTemp[2 * i + 1]) / (PointXYArrayTemp[(2 * (i + 1) + 1) % (2 * PointNumberTemp)] - PointXYArrayTemp[2 * i + 1]));
					tempArray[2 * count + 1] = down;
					//v1
					tempArray[2 * (count + 1)] = PointXYArrayTemp[2 * (i + 1) % (2 * PointNumberTemp)];
					tempArray[2 * (count + 1) + 1] = PointXYArrayTemp[(2 * (i + 1) + 1) % (2 * PointNumberTemp)];
					count += 2;
				}
				else if (PointXYArrayTemp[2 * i + 1] >= down&&PointXYArrayTemp[(2 * (i + 1) + 1) % (2 * PointNumberTemp)] >= down)//in-in
				{
					//v1
					tempArray[2 * count] = PointXYArrayTemp[2 * (i + 1) % (2 * PointNumberTemp)];
					tempArray[2 * count + 1] = PointXYArrayTemp[(2 * (i + 1) + 1) % (2 * PointNumberTemp)];
					count++;
				}
				else if (PointXYArrayTemp[2 * i + 1] >= down&&PointXYArrayTemp[(2 * (i + 1) + 1) % (2 * PointNumberTemp)] < down)//in-out
				{
					//v0'
					tempArray[2 * count] = Round(PointXYArrayTemp[2 * i] + (PointXYArrayTemp[2 * (i + 1) % (2 * PointNumberTemp)] - PointXYArrayTemp[2 * i])*(down - PointXYArrayTemp[2 * i + 1]) / (PointXYArrayTemp[(2 * (i + 1) + 1) % (2 * PointNumberTemp)] - PointXYArrayTemp[2 * i + 1]));
					tempArray[2 * count + 1] = down;
					count++;
				}
				else if (PointXYArrayTemp[2 * i + 1] < down&&PointXYArrayTemp[(2 * (i + 1) + 1) % (2 * PointNumberTemp)]<down)//out-out
				{
					//do nothing
				}
			}
			else if (side == 3)//��
			{
				if (PointXYArrayTemp[2 * i + 1]>up&&PointXYArrayTemp[(2 * (i + 1) + 1) % (2 * PointNumberTemp)] <= up)//out-in
				{
					//v0'
					tempArray[2 * count] = Round(PointXYArrayTemp[2 * i] + (PointXYArrayTemp[2 * (i + 1) % (2 * PointNumberTemp)] - PointXYArrayTemp[2 * i])*(up - PointXYArrayTemp[2 * i + 1]) / (PointXYArrayTemp[(2 * (i + 1) + 1) % (2 * PointNumberTemp)] - PointXYArrayTemp[2 * i + 1]));
					tempArray[2 * count + 1] = up;
					//v1
					tempArray[2 * (count + 1)] = PointXYArrayTemp[2 * (i + 1) % (2 * PointNumberTemp)];
					tempArray[2 * (count + 1) + 1] = PointXYArrayTemp[(2 * (i + 1) + 1) % (2 * PointNumberTemp)];
					count += 2;
				}
				else if (PointXYArrayTemp[2 * i + 1] <= up&&PointXYArrayTemp[(2 * (i + 1) + 1) % (2 * PointNumberTemp)] <= up)//in-in
				{
					//v1
					tempArray[2 * count] = PointXYArrayTemp[2 * (i + 1) % (2 * PointNumberTemp)];
					tempArray[2 * count + 1] = PointXYArrayTemp[(2 * (i + 1) + 1) % (2 * PointNumberTemp)];
					count++;
				}
				else if (PointXYArrayTemp[2 * i + 1] <= up&&PointXYArrayTemp[(2 * (i + 1) + 1) % (2 * PointNumberTemp)] > up)//in-out
				{
					//v0'
					tempArray[2 * count] = Round(PointXYArrayTemp[2 * i] + (PointXYArrayTemp[2 * (i + 1) % (2 * PointNumberTemp)] - PointXYArrayTemp[2 * i])*(up - PointXYArrayTemp[2 * i + 1]) / (PointXYArrayTemp[(2 * (i + 1) + 1) % (2 * PointNumberTemp)] - PointXYArrayTemp[2 * i + 1]));
					tempArray[2 * count + 1] = up;
					count++;
				}
				else if (PointXYArrayTemp[2 * i + 1] > up&&PointXYArrayTemp[(2 * (i + 1) + 1) % (2 * PointNumberTemp)] > up)//out-out
				{
					//do nothing
				}
			}
		}//���һ������
		PointNumberTemp = count;
		//debug
		//printf("%d\n", PointNumberTemp);
		//delete
		if (PointXYArrayTemp != 0){
			delete[] PointXYArrayTemp;
			PointXYArrayTemp = 0;
		}
		PointXYArrayTemp = tempArray;
	}//����ĸ�����
	//�޸Ĵ���Ĳ���
	*PointNumber = PointNumberTemp;
	*PointXYArray = PointXYArrayTemp;
}

void display()
{
	//���������λ�úͶ�άƽ��λ��
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT);
	gluLookAt(0, 0, 0, 0, 0, -1, 0, 1, 0);
	glTranslatef(0, 0, -5);//��άƽ��Ϊz=-5
	//�ֲ�����
	int pointNumberTemp = pointNumber;
	int * pointXYArrayTemp = new int[2*pointNumberTemp];
	for (int i = 0; i < 2*pointNumberTemp; i++){
		pointXYArrayTemp[i] = pointXYArray[i];
	}
	//ԭʼ
	glColor3f(1, 0, 0);
	FillPolygon(pointXYArrayTemp, pointNumberTemp);
	//���ü�ƽ��
	glColor3f(0, 0, 1);
	glBegin(GL_LINES);
	glVertex3f(left, down, 0);
	glVertex3f(left, up, 0);
	glVertex3f(left, up, 0);
	glVertex3f(right, up, 0);
	glVertex3f(right, up, 0);
	glVertex3f(right, down, 0);
	glVertex3f(right, down, 0);
	glVertex3f(left, down, 0);
	glEnd();
	//�ü�
	ClipPolygon(&pointXYArrayTemp, &pointNumberTemp, left, right, down, up);
	//���
	glColor3f(0, 1, 0);
	FillPolygon(pointXYArrayTemp, pointNumberTemp);
	//ˢ��
	glFlush();
	delete []pointXYArrayTemp;
}

void reshape(int w, int h)
{
	if (0 == h)
		h = 1;
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(cleft, cright, cdown, cup, 1, 100);//��άƽ�淶Χ��0<=x<=100,0<=y<=100
	glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'q':
		std::cout << "How many vertices?" << std::endl;
		std::cin >> pointNumber;
		delete[]pointXYArray;
		pointXYArray = new int[2 * pointNumber];
		std::cout << "Input the x and y cordinates of the vertices?" << std::endl;
		for (int i = 0; i < pointNumber; i++){
			std::cin >> pointXYArray[2 * i];
			std::cin >> pointXYArray[2 * i + 1];
		}
		break;
	case 'w':
		std::cout << "Input the left, right, down, up of the clip plane in order." << std::endl;
		std::cin >> left;
		std::cin >> right;
		std::cin >> down;
		std::cin >> up;
		break;
	case 'e':
		std::cout << "Input the left, right, down, up of the camera." << std::endl;
		std::cin >> cleft;
		std::cin >> cright;
		std::cin >> cdown;
		std::cin >> cup;
		glOrtho(cleft, cright, cdown, cup, 1, 100);//��άƽ�淶Χ��0<=x<=100,0<=y<=100
		glutPostRedisplay();
		break;
	default:
		break;
	}
	glutPostRedisplay();
}

int main(int argc, char** argv)
{
	//���ö���εĶ���
	pointNumber = 8;// 4;
	pointXYArray = new int[2 * pointNumber];
	pointXYArray[0] = 20;// 0;
	pointXYArray[1] = 30;// 20;
	pointXYArray[2] = 50;// 20;
	pointXYArray[3] = 30;// 40;
	pointXYArray[4] = 50;// 40;
	pointXYArray[5] = 10;// 20;
	pointXYArray[6] = 20;// 20;
	pointXYArray[7] = 10;// 0;
	pointXYArray[8] = 20;
	pointXYArray[9] = 15;
	pointXYArray[10] = 40;
	pointXYArray[11] = 15;
	pointXYArray[12] = 40;
	pointXYArray[13] = 25;
	pointXYArray[14] = 20;
	pointXYArray[15] = 25;
	//���������Χ
	cleft = 0;
	cright = 100;
	cdown = 0;
	cup = 100;
	//���òü�ƽ��
	left = 5;
	right = 35;
	down = 5;
	up = 35;

	std::cout << "press 'q' to change vertices" << std::endl;
	std::cout << "press 'w' to change clip plane" << std::endl;
	std::cout << "press 'e' to change camera" << std::endl;
	//��ʼ��OpenGL
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("CG2-3");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	return 0;
}