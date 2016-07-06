#include "Skybox.hpp"

static std::vector<float> cubeVerts = {
    -1.0f,  -1.0f,   1.0f,
    1.0f,  -1.0f,   1.0f,
    -1.0f,   1.0f,   1.0f,
    1.0f,   1.0f,   1.0f,
    -1.0f,  -1.0f,  -1.0f,
    1.0f,  -1.0f,  -1.0f,
    -1.0f,   1.0f,  -1.0f,
    1.0f,   1.0f,  -1.0f,
};

static std::vector<unsigned short> cubeIndices = {0, 1, 2, 3, 7, 1, 5, 4, 7, 6, 2, 4, 0, 1};


up<GameObject> Skybox::create(){
    up<GameObject> go = up<GameObject>(new GameObject("SKYBOX"));
    go->mTransform.scale(50, 50, 50);
    up<Skybox> skyboxComp = up<Skybox>(new Skybox(go.get()));
    go->addComponent(std::move(skyboxComp));
    return go;
}


Skybox::Skybox(GameObject * const gameObject) : IComponent(gameObject){
    

    
     
    mComponentType = ComponentEnum::SKYBOX;
    string prefix = "";
#ifdef ANDROID
    prefix = "skybox/";
#endif
    Materials::instance()->loadTexture(prefix + "Right.png", false);
    Materials::instance()->loadTexture(prefix + "Left.png", false);
    Materials::instance()->loadTexture(prefix + "Top.png", false);
    Materials::instance()->loadTexture(prefix + "Bottom.png", false);
    Materials::instance()->loadTexture(prefix + "Front.png", false);
    Materials::instance()->loadTexture(prefix + "Back.png", false);
    

    
    
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ibo);
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    unsigned char size = sizeof(v3d) / sizeof(float);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, size, GL_FLOAT, GL_FALSE, 0, 0);
    
    
    glBufferData(GL_ARRAY_BUFFER, cubeVerts.size() * sizeof(float), &cubeVerts[0], GL_STATIC_DRAW );
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, cubeIndices.size() * sizeof(unsigned short), &cubeIndices[0], GL_STATIC_DRAW);
    
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, size, GL_FLOAT, GL_FALSE, 0, 0);
     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    
    
    glBindVertexArray(0);
    
    
    unsigned int cubeMapID;
    glGenTextures(1, &cubeMapID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapID);
    
    sp<Texture> right = Materials::instance()->getTexture(prefix + "Right.png");
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, right->width, right->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &right->texelArray[0] );
    
    sp<Texture> left = Materials::instance()->getTexture(prefix + "Left.png");
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, left->width, left->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &left->texelArray[0]);
    
    sp<Texture> top = Materials::instance()->getTexture(prefix + "Top.png");
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, top->width, top->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &top->texelArray[0]);
    
    sp<Texture> bottom = Materials::instance()->getTexture(prefix + "Bottom.png");
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, bottom->width, bottom->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &bottom->texelArray[0]);
    
    sp<Texture> front = Materials::instance()->getTexture(prefix + "Front.png");
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, front->width, front->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &front->texelArray[0]);
    
    sp<Texture> back = Materials::instance()->getTexture(prefix + "Back.png");
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, back->width, back->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &back->texelArray[0]);
    
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    sp<ObjMaterial> skyboxMat = std::make_shared<ObjMaterial>("skybox");
    skyboxMat->tAmbient = right;
    skyboxMat->tAmbient->target = GL_TEXTURE_CUBE_MAP;
    right->ID = cubeMapID;
    skyboxMat->program = Materials::instance()->getProgram("skybox.omg");
    Materials::instance()->addMaterial(skyboxMat);
    
    
    
    
}

Skybox::~Skybox(){
}

bool Skybox::init(){
    logMessage("Init skybox goes here");
    return true;
}


void Skybox::draw(){
    Camera::instance()->pushMMatrix( go->transformMatrix() );
    glCullFace(GL_FRONT);
//    glDisable(GL_DEPTH_TEST);
    glBindVertexArray(vao);
    
    sp<ObjMaterial> mat = Materials::instance()->getMaterial("skybox");
    mat->use();

    glDrawElements(GL_TRIANGLE_STRIP, cubeIndices.size(), GL_UNSIGNED_SHORT, nullptr);
    glBindVertexArray(0);
    Camera::instance()->popMMatrix();
    glCullFace(GL_BACK);
//    glEnable(GL_DEPTH_TEST);
}



