#include "Scene.hpp"

class Levelbuilder{
public:
    unsigned int blockCount = 10;
    sp<ObjMesh> mesh;
    
    void buildLevel();
    
    
private:
    v3d mLastBlockPoss;
    
    v3d  calculateNewPoss(v3d lastPos);
    void addNewBlock(v3d blockPos);
};
