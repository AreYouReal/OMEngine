#include "GameObject.hpp"
#include <queue>

class PlayerController{
public:
    PlayerController(GameObject * const gameObject);
    ~PlayerController();
    
    void debugInit();
    
    void onTouch();
    
    void update();
    
    std::queue<float> actions{};
    std::queue<float> currentAction{};
    
private:
    
    float playerSpeed = 20;
    
// Fields
    GameObject *mGo;
    RigidBodyComponent *mRigidBodyComp;

// Helpers
    void rotate();
    void refreshVelocity();
    
};