#include <GL/glut.h>
#include <iostream>
#include <cmath>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>


using namespace std;

// Parâmetros da câmera default
float cameraPosX = 0.0f, cameraPosY = 1.0f, cameraPosZ = 5.0f;
float cameraLookAtX = 0.0f, cameraLookAtY = 1.0f, cameraLookAtZ = 0.0f;
float horizontalAngle = 3.14f;
float verticalAngle = 0.0f;
const float movementSpeed = 0.1f;



// Optimus prime
GLuint texId[5]; // IDs das texturas
GLfloat OptimusScale = 5.5f; // Escala inicial
GLfloat OptimusPosX = 1.0f;  // Posição X inicial
GLfloat OptimusPosY = 5.0f;  // Posição Y inicial
GLfloat OptimusPosZ = -15.0f;  // Posição Z inicial

// Bola quicante
float spherePosY = 2.0f; // Defina a posição X inicial desejada
float spherePosX = 0.0f; // Defina a posição Y inicial desejada
float spherePosZ = -12.0f; // Defina a posição Z inicial desejada
float sphereVelocityY = 0.1f; // Velocidade da bola
float sphereAccelerationY = -0.005f; // Aceleracao da bola

// Fantasmonstro
float monsterPosX = 0.0f; // Defina a posição X inicial desejada
float monsterPosY = 2.0f; // Defina a posição Y inicial desejada
float monsterPosZ = -10.0f; // Defina a posição Z inicial desejada

bool monsterVisible = true; // Flag para controlar a visibilidade do monstro
int monsterPosition = 0; // Variável para controlar a posição do monstro

// Variavel temporaria para textura da caixa
GLuint cubeTextureID;
// Variavel temporaria para textura da bola
GLuint sphereTextureID;

// Textura para o corpo do carro
GLuint carCubeTextureID;
// Textura para as rodas do carro
GLuint wheelTextureID;

//texturas da casa
GLuint textureHouseID;
GLuint textureRoofID;

//casa
GLfloat houseScale = 2.0f; // Escala da casa
GLfloat housePosX = 0.0f;  // Posição X da casa
GLfloat housePosY = 0.0f;  // Posição Y da casa
GLfloat housePosZ = -10.0f;  // Posição Z da casa

GLfloat roofScale = 2.0f;  // Escala do telhado
GLfloat roofPosX = 0.0f;   // Posição X do telhado
GLfloat roofPosY = 2.0f;   // Posição Y do telhado
GLfloat roofPosZ = -10.0f; // Posição Z do telhado

// Operacoes de camera
void updateLookAtDirection() {
    cameraLookAtX = cameraPosX + cos(verticalAngle) * sin(horizontalAngle);
    cameraLookAtY = cameraPosY + sin(verticalAngle);
    cameraLookAtZ = cameraPosZ + cos(verticalAngle) * cos(horizontalAngle);
}

//Serve para mover a camera nas coordenadas X
void handleKeyPress(unsigned char key, int x, int y) {
    float dx = cameraLookAtX - cameraPosX;
    float dz = cameraLookAtZ - cameraPosZ;

    switch (key) {
        case 'w':
            cameraPosX += dx * movementSpeed;
            cameraPosZ += dz * movementSpeed;
            break;

        case 's':
            cameraPosX -= dx * movementSpeed;
            cameraPosZ -= dz * movementSpeed;
            break;

        case 'd':
            cameraPosX -= dz * movementSpeed;
            cameraPosZ += dx * movementSpeed;
            break;

        case 'a':
            cameraPosX += dz * movementSpeed;
            cameraPosZ -= dx * movementSpeed;
            break;

        case 'z':
            cameraPosY += movementSpeed;  // Adicionado
            updateLookAtDirection();
            break;

        case 'x':
            cameraPosY -= movementSpeed;  // Adicionado
                        updateLookAtDirection();
            break;
        case 27:
        	exit(0);
        	break;
    }

    // Atualiza a direção do olhar da câmera.
    cameraLookAtX = cameraPosX + dx;
    cameraLookAtZ = cameraPosZ + dz;

    cout << "Position: (" << cameraPosX << ", " << cameraPosY << ", " << cameraPosZ << ")\n";
    glutPostRedisplay();
}

//Serve para mover a camera com as setas em X e Y
void handleSpecialKeyPress(int key, int x, int y) {
    const float angleSpeed = 0.05f;
    switch (key) {
        case GLUT_KEY_UP:
            verticalAngle += angleSpeed;
            break;
        case GLUT_KEY_DOWN:
            verticalAngle -= angleSpeed;
            break;
        case GLUT_KEY_RIGHT:
            horizontalAngle -= angleSpeed;
            break;
        case GLUT_KEY_LEFT:
            horizontalAngle += angleSpeed;
            break;
    }

    cameraLookAtX = cameraPosX + 10.0f * sin(horizontalAngle);
    cameraLookAtY = cameraPosY + 10.0f * sin(verticalAngle);
    cameraLookAtZ = cameraPosZ + 10.0f * cos(horizontalAngle);

    cout << "View Direction: (" << cameraLookAtX << ", " << cameraLookAtY << ", " << cameraLookAtZ << ")\n";
    glutPostRedisplay();
}

GLuint LoadCubeTexture(const char* filepath) {
    int width, height, channels;
    unsigned char* image = stbi_load(filepath, &width, &height, &channels, STBI_rgb_alpha);
    if (image == NULL) {
        cerr << "Erro ao carregar a textura " << filepath << endl;
        return 0;
    }

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Substituição do glTexImage2D pelo gluBuild2DMipmaps
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, width, height, GL_RGBA, GL_UNSIGNED_BYTE, image);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(image);

    return textureID;
}

void drawTexturedCube(float lado) {
    float halfSide = lado / 2.0f;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, cubeTextureID);
    glColor3f(1.0, 1.0, 1.0); // cor branca para não alterar a textura

    glBegin(GL_QUADS);

    // Face frontal
    glTexCoord2f(0, 0); glVertex3f(-halfSide, -halfSide, halfSide);
    glTexCoord2f(1, 0); glVertex3f(halfSide, -halfSide, halfSide);
    glTexCoord2f(1, 1); glVertex3f(halfSide, halfSide, halfSide);
    glTexCoord2f(0, 1); glVertex3f(-halfSide, halfSide, halfSide);

    // Face traseira
    glTexCoord2f(0, 0); glVertex3f(halfSide, -halfSide, -halfSide);
    glTexCoord2f(1, 0); glVertex3f(-halfSide, -halfSide, -halfSide);
    glTexCoord2f(1, 1); glVertex3f(-halfSide, halfSide, -halfSide);
    glTexCoord2f(0, 1); glVertex3f(halfSide, halfSide, -halfSide);

    // Face esquerda
    glTexCoord2f(0, 0); glVertex3f(-halfSide, -halfSide, -halfSide);
    glTexCoord2f(1, 0); glVertex3f(-halfSide, -halfSide, halfSide);
    glTexCoord2f(1, 1); glVertex3f(-halfSide, halfSide, halfSide);
    glTexCoord2f(0, 1); glVertex3f(-halfSide, halfSide, -halfSide);

    // Face direita
    glTexCoord2f(0, 0); glVertex3f(halfSide, -halfSide, halfSide);
    glTexCoord2f(1, 0); glVertex3f(halfSide, -halfSide, -halfSide);
    glTexCoord2f(1, 1); glVertex3f(halfSide, halfSide, -halfSide);
    glTexCoord2f(0, 1); glVertex3f(halfSide, halfSide, halfSide);

    // Face superior
    glTexCoord2f(0, 0); glVertex3f(-halfSide, halfSide, halfSide);
    glTexCoord2f(1, 0); glVertex3f(halfSide, halfSide, halfSide);
    glTexCoord2f(1, 1); glVertex3f(halfSide, halfSide, -halfSide);
    glTexCoord2f(0, 1); glVertex3f(-halfSide, halfSide, -halfSide);

    // Face inferior
    glTexCoord2f(0, 0); glVertex3f(-halfSide, -halfSide, -halfSide);
    glTexCoord2f(1, 0); glVertex3f(halfSide, -halfSide, -halfSide);
    glTexCoord2f(1, 1); glVertex3f(halfSide, -halfSide, halfSide);
    glTexCoord2f(0, 1); glVertex3f(-halfSide, -halfSide, halfSide);

    glEnd();

    glDisable(GL_TEXTURE_2D);
}

// Operações para desenhar a bola quicante
// Função para atualizar a posição da bola (chamada a cada 16 milissegundos)
void updateSpherePosition(int value) {
    // Atualiza a posição da esfera
    spherePosY += sphereVelocityY;
    sphereVelocityY += sphereAccelerationY;

    // Verifica se a esfera atingiu o chão
    if (spherePosY <= 1.0f) {
        spherePosY = 1.0f; // Certifique-se de que a esfera não penetre o chão
        sphereVelocityY = 0.1f; // Reinicie o movimento vertical para pular novamente
    }

    // Agenda a próxima chamada para esta função
    glutTimerFunc(16, updateSpherePosition, 0);

    glutPostRedisplay(); // Solicita a atualização da exibição
}

GLuint LoadSphereTexture(const char* filepath) {
    int width, height, channels;
    unsigned char* image = stbi_load(filepath, &width, &height, &channels, STBI_rgb_alpha);
    if (image == NULL) {
        cerr << "Erro ao carregar a textura " << filepath << endl;
        return 0;
    }

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Substituição do glTexImage2D pelo gluBuild2DMipmaps
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, width, height, GL_RGBA, GL_UNSIGNED_BYTE, image);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(image);

    return textureID;
}

void drawSolidSphere(float radius, int slices, int stacks) {
    glColor3f(1.0, 0.0, 0.0); // cor vermelha
    glutSolidSphere(radius, slices, stacks);
}

void drawTexturedSphere(float radius, int slices, int stacks) {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, sphereTextureID);
    
    GLUquadric* quad = gluNewQuadric();
    gluQuadricTexture(quad, GL_TRUE);
    glColor3f(1.0, 1.0, 1.0); // cor branca para não alterar a textura
    gluSphere(quad, radius, slices, stacks);
    gluDeleteQuadric(quad);

    glDisable(GL_TEXTURE_2D);
}

// Função para atualizar a posição e a visibilidade do monstro
void updateMonsterPosition(int value) {
    cout << "Case: " << monsterPosition << endl;
    // Adicione declarações cout para imprimir as posições X, Y e Z do monstro
    cout << "MonsterPosX: " << monsterPosX << endl;
    cout << "MonsterPosY: " << monsterPosY << endl;
    cout << "MonsterPosZ: " << monsterPosZ << endl;

    monsterVisible = !monsterVisible; // Inverte a visibilidade do monstro

    // Incrementa a posição do monstro
    monsterPosition++;

    // Se o monstro atingiu a última posição, volte para a posição 0
    if (monsterPosition > 3) {
        monsterPosition = 0;
    }

    // Defina a posição do monstro com base nas coordenadas X, Y e Z
    switch (monsterPosition) {
        case 0:
            monsterPosX = 2.0f;
            monsterPosY = 2.4f;
            monsterPosZ = -10.5f; // Defina a posição Z desejada aqui
            break;
        case 1:
            monsterPosX = -2.0f;
            monsterPosY = 2.4f;
            monsterPosZ = -10.5f; // Defina a posição Z desejada aqui
            break;
        case 2:
            monsterPosX = -3.0f;
            monsterPosY = 2.4f;
            monsterPosZ = -10.5f; // Defina a posição Z desejada aqui
            break;
        case 3:
            monsterPosX = 3.0f;
            monsterPosY = 2.2f;
            monsterPosZ = -10.5f; // Defina a posição Z desejada aqui
            break;
    }

    glutTimerFunc(1500, updateMonsterPosition, 0); // Chama a função novamente após 5 segundos
    glutPostRedisplay(); // Solicita a atualização da exibição
}

void drawMonster(float scale, float posX, float posY, float posZ) {
    glPushMatrix();
    glTranslatef(posX, posY, posZ);
    glScaled(scale, scale, scale);

    // TEETH THING
    glPushMatrix();
    glColor3f(0, 0, 0); // Cor dos dentes (preto)
    glRotated(30, 0, 0, 1);
    glTranslated(-0.1, -0.2, 0.6);
    glScaled(0.7, 0.7, 0.7);
    glutSolidTetrahedron();
    glPopMatrix();
    // TEETH THING

    // ARMS
    glPushMatrix();
    glColor3f(0, 0, 0); // Cor dos braços (preto)

    // BRAÇO DIREITO
    glPushMatrix();
    glTranslated(1, 0, 0);
    glScaled(1, 10, 1);
    glutSolidCube(0.1);
    glPopMatrix();
    // BRAÇO DIREITO

    // BRAÇO ESQUERDO
    glPushMatrix();
    glTranslated(-1, 0, 0);
    glScaled(1, 10, 1);
    glutSolidCube(0.1);
    glPopMatrix();
    // BRAÇO ESQUERDO

    glPopMatrix();
    // ARMS

    // LEG THING
    glPushMatrix();
    glColor3f(1, 1, 1); // Cor das pernas (branco)
    glTranslated(0, -0.65, 0);
    glRotated(90, 1, 0, 0);
    glScaled(0.5, 1, 0.5);
    gluCylinder(gluNewQuadric(), 0.75, 0.3, 2, 15, 15);
    glPopMatrix();
    // LEG THING

    // 2 EYES
    glPushMatrix();
    glColor3f(0, 0, 0); // Cor dos olhos (preto)
    glTranslated(0, 0.4, 0);

    glPushMatrix();
    glTranslated(0.3, 0, 0);
    glutSolidCone(0.5, 1.5, 15, 15);
    glPopMatrix();

    glPushMatrix();
    glTranslated(-0.3, 0, 0);
    glutSolidCone(0.5, 1.5, 15, 15);
    glPopMatrix();

    glPopMatrix();
    // 2 EYES

    // THE ACTUAL THING
    glColor3f(1, 1, 1); // Cor do corpo principal (branco)
    glutSolidSphere(1, 15, 15);
    glPopMatrix();
}

void updateMonster(int value) {
    // Alterna a visibilidade do monstro de neve
    monsterVisible = !monsterVisible;

    glutTimerFunc(2000, updateMonster, 0);

    glutPostRedisplay(); // Solicita a atualização da exibição
}

void drawCube(GLfloat xSize, GLfloat ySize, GLfloat zSize, GLuint texId) {
    // Define os vértices do cubo
    GLfloat vertices[][3] = {
        { -xSize / 2, -ySize / 2, -zSize / 2 },
        { xSize / 2, -ySize / 2, -zSize / 2 },
        { xSize / 2, ySize / 2, -zSize / 2 },
        { -xSize / 2, ySize / 2, -zSize / 2 },
        { -xSize / 2, -ySize / 2, zSize / 2 },
        { xSize / 2, -ySize / 2, zSize / 2 },
        { xSize / 2, ySize / 2, zSize / 2 },
        { -xSize / 2, ySize / 2, zSize / 2 }
    };

    // Define as faces do cubo
    GLint faces[][4] = {
        { 0, 1, 2, 3 },
        { 4, 5, 6, 7 },
        { 0, 4, 5, 1 },
        { 2, 6, 7, 3 },
        { 0, 4, 7, 3 },
        { 1, 5, 6, 2 }
    };

    // Define as coordenadas de textura
    GLfloat texCoords[][2] = {
        { 0.0, 0.0 },
        { 1.0, 0.0 },
        { 1.0, 1.0 },
        { 0.0, 1.0 }
    };

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texId);

    for (int i = 0; i < 6; i++) {
        glBegin(GL_QUADS);
        for (int j = 0; j < 4; j++) {
            glTexCoord2fv(texCoords[j]);
            glVertex3fv(vertices[faces[i][j]]);
        }
        glEnd();
    }

    glDisable(GL_TEXTURE_2D);
}

void loadTexturesOptimus() {
    glGenTextures(5, texId);

    const char* textureFiles[] = {
        "./resources/optimus_cabeca.bmp",
        "./resources/optimus_corpo.bmp",
        "./resources/optimus_braco.bmp",
        "./resources/optimus_perna.bmp",
        "./resources/optimus_olhos.bmp"
    };

    for (int i = 0; i < 5; i++) {
        glBindTexture(GL_TEXTURE_2D, texId[i]);

        int width, height, channels;
        unsigned char* texels = stbi_load(textureFiles[i], &width, &height, &channels, 0);

        if (texels) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texels);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            stbi_image_free(texels);
        } else {
            cerr << "Erro ao carregar a textura!" << endl;
        }
    }
}


void drawOptimus() {
	
    // Desenhar o personagem
    glPushMatrix();

    // Aplicar transformações de escala e translação para o personagem inteiro
    glTranslatef(OptimusPosX, OptimusPosY, OptimusPosZ);
    glScalef(OptimusScale, OptimusScale, OptimusScale);

    //CABECA  (textura 0)
    glBindTexture(GL_TEXTURE_2D, texId[0]);
    glPushMatrix();
    glTranslatef(0.0f, 1.4f, 0.0f);
    drawCube(1.0f, 1.0f, 1.0f, texId[0]);
    glPopMatrix();

    // Corpo (textura 1)
    glBindTexture(GL_TEXTURE_2D, texId[1]);
    drawCube(1.2f, 1.2f, 0.6f, texId[1]);

    // Braço Direito (textura 2)
    glBindTexture(GL_TEXTURE_2D, texId[2]);
    glPushMatrix();
    glTranslatef(0.8f, 0.6f, 0.0f);
    drawCube(0.4f, 0.1f, 0.1f, texId[2]);
    glPopMatrix();

    // Braço Esquerdo (textura 2)
    glBindTexture(GL_TEXTURE_2D, texId[2]);
    glPushMatrix();
    glTranslatef(-0.8f, 0.6f, 0.0f);
    drawCube(0.4f, 0.1f, 0.1f, texId[2]);
    glPopMatrix();

    // Perna Direita (textura 3)
    glBindTexture(GL_TEXTURE_2D, texId[3]);
    glPushMatrix();
    glTranslatef(0.4f, -0.8f, 0.0f);
    drawCube(0.3f, 0.8f, 0.3f, texId[3]);
    glPopMatrix();

    // Perna Esquerda (textura 3)
    glBindTexture(GL_TEXTURE_2D, texId[3]);
    glPushMatrix();
    glTranslatef(-0.4f, -0.8f, 0.0f);
    drawCube(0.3f, 0.8f, 0.3f, texId[3]);
    glPopMatrix();

    // Olhos (textura 4)
    //glBindTexture(GL_TEXTURE_2D, texId[4]);
    //glPushMatrix();
    //glTranslatef(0.1f, 1.4f, 0.6f);
    //drawCube(0.3f, 0.1f, 0.1f, texId[4]);
    //glPopMatrix();

    //glPushMatrix();
    //glTranslatef(-0.2f, 1.4f, 0.6f);
    //drawCube(0.3f, 0.1f, 0.1f, texId[4]);
    //glPopMatrix();

    glPopMatrix(); // Fim do desenho do personagem
}

void drawTexturedWheel(float radius) {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, wheelTextureID);
    glColor3f(1.0f, 1.0f, 1.0f); // Cor branca para não alterar a textura

    // Desenha uma roda usando uma esfera
    glutSolidSphere(radius, 20, 20); // Ajuste a quantidade de segmentos conforme necessário

    glDisable(GL_TEXTURE_2D);
}

void drawCarCube(float xSize, float ySize, float zSize, GLuint texId) {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texId);
    glColor3f(1.0f, 1.0f, 1.0f); // Cor branca para não alterar a textura

    // Desenha o corpo do carro usando um cubo
    drawCube(xSize, ySize, zSize, texId);

    glDisable(GL_TEXTURE_2D);
}

void drawCar() {
    glPushMatrix();
    glTranslatef(0.0f, 0.5f, -5.0f); // Ajuste a posição do carro conforme necessário
    // Aplica a rotação em torno do eixo Y (vertical)
    glRotatef(-65.01f, 0.0f, 1.0f, 0.0f);
    glScalef(2.5f, 1.5f, 1.5f); // Ajuste a escala do carro conforme necessário

    // Corpo do carro
    drawCarCube(1.0f, 0.5f, 0.5f, carCubeTextureID);

    // Roda traseira direita
    glPushMatrix();
    glTranslatef(0.35f, -0.25f, 0.25f); // Ajuste a posição da roda conforme necessário
    drawTexturedWheel(0.15f);
    glPopMatrix();

    // Roda traseira esquerda
    glPushMatrix();
    glTranslatef(0.35f, -0.25f, -0.25f); // Ajuste a posição da roda conforme necessário
    drawTexturedWheel(0.15f);
    glPopMatrix();

    // Roda dianteira direita
    glPushMatrix();
    glTranslatef(-0.35f, -0.25f, 0.25f); // Ajuste a posição da roda conforme necessário
    drawTexturedWheel(0.15f);
    glPopMatrix();

    // Roda dianteira esquerda
    glPushMatrix();
    glTranslatef(-0.35f, -0.25f, -0.25f); // Ajuste a posição da roda conforme necessário
    drawTexturedWheel(0.15f);
    glPopMatrix();

    glPopMatrix(); // Fim do desenho do carro
}

void setupLights() {
	glEnable(GL_LIGHTING); // Habilita a iluminação globalmente
    glEnable(GL_DEPTH_TEST);

    // Define a posição da luz (luz pontual no topo da caixa, dentro dela)
    GLfloat light_position[] = {0.0f, 2.0f, 0.0f, 1.0};
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    // Reduza a intensidade da iluminação ambiente
	GLfloat ambient_light[] = {2.5, 2.5, 2.5, 2.0};
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light);

    // Reduza a intensidade da iluminação difusa
    GLfloat diffuse_light[] = {0.6, 0.6, 0.6, 1.0};
    //glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_light);

    // Direção do spotlight (apontando para baixo)
    GLfloat spot_direction[] = {0.0, -1.0, 0.0};
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);

    // Define o ângulo do cone de luz e o foco da luz
    glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 180.0);     // Ângulo de corte
    glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 5.0);    // Concentração da luz

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    //glDisable(GL_LIGHTING);
}

//CASA

GLuint LoadHouseTexture(const char* filepath) {
    int width, height, channels;
    unsigned char* image = stbi_load(filepath, &width, &height, &channels, STBI_rgb_alpha);
    if (image == NULL) {
        cerr << "Erro ao carregar a textura da casa: " << filepath << endl;
        return 0;
    }

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(image);

    return textureID;
}

GLuint LoadRoofTexture(const char* filepath) {
    int width, height, channels;
    unsigned char* image = stbi_load(filepath, &width, &height, &channels, STBI_rgb_alpha);
    if (image == NULL) {
        cerr << "Erro ao carregar a textura do telhado: " << filepath << endl;
        return 0;
    }

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(image);

    return textureID;
}

void drawHouse(float scale, float posX, float posY, float posZ, GLuint textureHouseID) {
    glPushMatrix();
    glTranslatef(posX, posY, posZ);
    glScalef(scale, scale, scale);

    // Parede frontal da casa
    glBindTexture(GL_TEXTURE_2D, textureHouseID);
    glColor3f(1.0f, 1.0f, 1.0f); // Cor branca para não alterar a textura
    drawCube(2.0f, 2.0f, 2.0f, textureHouseID);

    glPopMatrix(); // Fim do desenho da casa
}

void drawRoof(float scale, float posX, float posY, float posZ, GLuint textureRoofID) {
    glPushMatrix();
    glTranslatef(posX, posY, posZ);
    glScalef(scale, scale, scale);
glEnable(GL_TEXTURE_2D);
    // Telhado da casa
    glBindTexture(GL_TEXTURE_2D, textureRoofID);
    glColor3f(1.0f, 1.0f, 1.0f); // Cor branca para não alterar a textura

    glBegin(GL_TRIANGLES);

    // Triângulo frontal
    glTexCoord2f(0.5f, 1.0f);
    glVertex3f(0.0f, 2.5f, 0.0f);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-2.5f, -0.5f, 2.5f);

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(2.5f, -0.5f, 2.5f);

    // Triângulo direito
    glTexCoord2f(0.5f, 1.0f);
    glVertex3f(0.0f, 2.5f, 0.0f);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(2.5f, -0.5f, 2.5f);

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(2.5f, -0.5f, -2.5f);

    // Triângulo traseiro
    glTexCoord2f(0.5f, 1.0f);
    glVertex3f(0.0f, 2.5f, 0.0f);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(2.5f, -0.5f, -2.5f);

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-2.5f, -0.5f, -2.5f);

    // Triângulo esquerdo
    glTexCoord2f(0.5f, 1.0f);
    glVertex3f(0.0f, 2.5f, 0.0f);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-2.5f, -0.5f, -2.5f);

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-2.5f, -0.5f, 2.5f);

    glEnd();

    glPopMatrix(); // Fim do desenho do telhado
}

void drawHouseAndRoof(float scale, float posX, float posY, float posZ, GLuint textureHouseID, GLuint textureRoofID) {
    glPushMatrix();
    glTranslatef(posX, posY, posZ);
    glScalef(scale, scale, scale);

    // Desenhe a casa
    drawHouse(1.0f, 0.0f, 0.0f, 0.0f, textureHouseID); // Ajuste a posição e escala conforme necessário

    // Desenhe o telhado
    drawRoof(0.5f, 0.0f, 1.0f, 0.0f, textureRoofID); // Ajuste a posição e escala conforme necessário

    glPopMatrix(); // Fim do desenho da casa e do telhado
}

void init() {
    setupLights();
}


void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, (double)w / (double)h, 1.0, 200.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// Função de exibição ajustada:

// Função de exibição
void display() {
	setupLights();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(cameraPosX, cameraPosY, cameraPosZ,
              cameraLookAtX, cameraLookAtY, cameraLookAtZ,
              0.0f, 1.0f, 0.0f);

    // Desenhe a esfera com a nova posição
    glPushMatrix();
    glTranslatef(spherePosX, spherePosY, spherePosZ);
    drawTexturedSphere(1.0f, 20, 20);
    glPopMatrix();

    // Desenhe o quarto no cenário
    glPushMatrix();
    glScalef(10.0f, 10.0f, 10.0f);
    glTranslatef(0.0f, 1.0f, -0.55f);
    drawTexturedCube(2.0f); // Passando o valor 2.0f como tamanho do lado
    glPopMatrix();

    // Desenha o monstro com propriedades personalizadas
    glDisable(GL_LIGHTING);
    if (monsterVisible) {
        glPushMatrix();
        glTranslatef(monsterPosX, monsterPosY, -2.0f); // Ajuste a posição conforme necessário
        glScalef(1.0f, 1.0f, 1.0f); // Ajuste a escala conforme necessário
drawMonster(1.0f, monsterPosX, monsterPosY, monsterPosZ);
        glPopMatrix();
    }
    glEnable(GL_LIGHTING);

	//Optimus
	drawOptimus();
	//CAR
	drawCar();
	//HOUSE
	drawHouseAndRoof(1.0f, 4.5f, 1.0f, -10.0f, textureHouseID, textureRoofID);
    glutSwapBuffers();
    

}

void LoadTextures() {
	loadTexturesOptimus();
    sphereTextureID = LoadSphereTexture("./resources/bola.bmp");
    cubeTextureID = LoadCubeTexture("./resources/caixa.bmp");
    carCubeTextureID = LoadCubeTexture("./resources/car_body_texture.bmp");
    wheelTextureID = LoadSphereTexture("./resources/car_wheel_texture.bmp");
    textureRoofID = LoadRoofTexture("./resources/house_roof_texture.bmp");
	textureHouseID = LoadHouseTexture("./resources/house_body_texture.bmp");

}

int main(int argc, char** argv) {
	
    // Inicializa a biblioteca GLUT
    glutInit(&argc, argv);
    
    glutTimerFunc(2000, updateMonster, 0);

    // Define o modo de display (buffer duplo, modo RGB e profundidade)
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    
    // Define o tamanho da janela de exibição
    glutInitWindowSize(1920, 1080);
    
    // Cria a janela na tela com um título
    glutCreateWindow("CAIXA DE BRINQUEDOS ASSOMBRADA PELO FANTASMA DA COMPUTACAO GRAFICA");

    // Chama a função de inicialização que configura a iluminação, etc.
    init();
    
    // Carregue todas as texturas
    LoadTextures();    

    // Associa a função 'display' para ser chamada sempre que a janela precisar ser redesenhada
    glutDisplayFunc(display);

    // Associa a função 'reshape' para ser chamada sempre que a janela for redimensionada
    glutReshapeFunc(reshape);

    // Associa a função 'handleKeyPress' para tratar eventos de teclas comuns
    glutKeyboardFunc(handleKeyPress);

    // Associa a função 'handleSpecialKeyPress' para tratar eventos de teclas especiais (setas, F1, F2, etc.)
    glutSpecialFunc(handleSpecialKeyPress);

	// Inicialize a função de atualização da posição da esfera
    glutTimerFunc(0, updateSpherePosition, 0);
    
    // Agende a primeira chamada para a função de atualização do monstro
    glutTimerFunc(2000, updateMonsterPosition, 0);
    
    // Entra no loop principal da GLUT
    glutMainLoop();



    return 0;
}



