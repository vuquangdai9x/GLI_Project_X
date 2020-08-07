#include "stdafx.h"
#include "Model3D.h"
#include <stddef.h>

Model3D::Model3D(int id) : m_id(id){}

Model3D::~Model3D()
{
	glDeleteBuffers(1, &m_vboId);
	glDeleteBuffers(1, &m_iboId);
}

bool Model3D::LoadFromFile(char* filePath, char* heightMapFile, float heightMapScale) {
	FILE * fIn = fopen(filePath, "r");
	if (fIn == NULL) {
		return false;
	}

	Vertex* aVertex = NULL;
	int* aIndice = NULL;

	fscanf(fIn, "NrVertices: %d\n", &m_iNumOfVertex);
	//printf("Num of vertices: %d\n", m_iNumOfVertex);

	aVertex = new Vertex[m_iNumOfVertex];
	float x, y, z, u, v, alpha;
	int index;
	unsigned int color;
	for (int i = 0; i < m_iNumOfVertex; i++) {
		fscanf(fIn, " %d. pos:[%f, %f, %f]; ", &index, &x, &y, &z);
		aVertex[i].SetPosition(x, y, z);
		fscanf(fIn, "norm:[%f, %f, %f]; ", &x, &y, &z);
		aVertex[i].SetNorm(x, y, z);
		fscanf(fIn, "binorm:[%f, %f, %f]; ", &x, &y, &z);
		aVertex[i].SetBinorm(x, y, z);
		fscanf(fIn, "tgt:[%f, %f, %f]; ", &x, &y, &z);
		aVertex[i].SetTangent(x, y, z);
		fscanf(fIn, "uv:[%f, %f];\n", &u, &v);
		aVertex[i].SetUV(u, v);

		//printf("Vertex %d:\n", index);
		//aVertex[i].PrintInfo();
	}

	fscanf(fIn, "NrIndices: %d\n", &m_iNumOfIndice);
	//printf("Num of indices: %d\n", m_iNumOfIndice);
	int v1, v2, v3;
	aIndice = new int[m_iNumOfIndice];
	for (int i = 0; i < m_iNumOfIndice / 3; i++) {
		//0.    0, 1, 2
		fscanf(fIn, "   %d.    %d,    %d,    %d", &index, &v1, &v2, &v3);
		aIndice[3 * i] = v1;
		aIndice[3 * i + 1] = v2;
		aIndice[3 * i + 2] = v3;
		//printf("Indice %d: %d %d %d\n", i, v1, v2, v3);
	}

	glGenBuffers(1, &m_vboId);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * m_iNumOfVertex, aVertex, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &m_iboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * m_iNumOfIndice, aIndice, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// clear memory
	if (aVertex != NULL) delete[] aVertex;
	if (aIndice != NULL) delete[] aIndice;
	fclose(fIn);

	return true;
}

bool Model3D::LoadFromRaw(char* filePath, int iWidth, int iHeight, float heightMapScale) {
	FILE* fIn = fopen(filePath, "rb");
	if (fIn == NULL) {
		return false;
	}

	fseek(fIn, 0L, SEEK_END);
	long int fileSize = ftell(fIn);
	if (iWidth * iHeight != fileSize) {
		printf("[ERR] Model3D: File size incompatible!\n");
		return false;
	}

	fseek(fIn, 0L, SEEK_SET);
	m_iNumOfVertex = iWidth * iHeight;
	Vertex* aVertex = new Vertex[m_iNumOfVertex];
	Vertex* v;
	float y;
	char c;
	for (int z = 0; z < iHeight; z++) {
		for (int x = 0; x < iWidth; x++) {
			fscanf(fIn, "%c", &c);
			y = (1 - (unsigned char)c / (float)255) * heightMapScale;
			v = &aVertex[z * iHeight + x];
			v->SetPosition(x,y,z);
			v->SetNorm(0, 0, 0);
			v->SetBinorm(0, 0, 0);
			v->SetTangent(0, 0, 0);
			v->SetUV(x / (float)iWidth, z / (float)iHeight);
		}
	}

	m_iNumOfIndice = (iWidth - 1) * (iHeight - 1) * 2 * 3;
	int* aIndice = new int[m_iNumOfIndice];
	for (int z = 0; z < iHeight-1; z++) {
		for (int x = 0; x < iWidth-1; x++) {
			aIndice[(z * (iWidth - 1) + x) * 6 + 0] = z * iWidth + x;
			aIndice[(z * (iWidth - 1) + x) * 6 + 1] = z * iWidth + x+1;
			aIndice[(z * (iWidth - 1) + x) * 6 + 2] = (z+1) * iWidth + x;
			aIndice[(z * (iWidth - 1) + x) * 6 + 3] = z * iWidth + x+1;
			aIndice[(z * (iWidth - 1) + x) * 6 + 4] = (z+1) * iWidth + x;
			aIndice[(z * (iWidth - 1) + x) * 6 + 5] = (z+1) * iWidth + x+1;
		}
	}

	/*for (int i = 0;i < m_iNumOfVertex;i++) {
		aVertex[i].PrintInfo();
	}*/
	/*for (int i = 0;i < m_iNumOfIndice;i++) {
		printf("%d ", aIndice[i]);
	}
	printf("\n");*/

	glGenBuffers(1, &m_vboId);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * m_iNumOfVertex, aVertex, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &m_iboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * m_iNumOfIndice, aIndice, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// clear memory
	delete[] aVertex;
	delete[] aIndice;
	fclose(fIn);

	return true;
}