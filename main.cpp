#include <AarniEngine/engine.h>
#include <iostream>
#include <fstream>

Transform t = Transform();
Transform t3 = Transform();
Camera cam = Camera();
Mesh m = Mesh();
Sprite s;

MeshRenderer mr = MeshRenderer();

Transform e1 = Transform();
Transform e2 = Transform();
Transform e3 = Transform();
Transform e4 = Transform();
Component e5 = Component();



void Start() //This function is called at the start of the app.
{
    mainCamera = &cam;
    root->AddChild(t);
    root->AddChild(t3);
    t.AddChild(cam);
    m = Mesh::LoadModel("Cube.obj");
    s = Sprite("BrickWall.png");
    mr.mesh = &m;
    t3.AddChild(mr);
    t3.position.z = 3;
}

void Update() //This function is called at every frame.
{

    t3.position.x = sin(sessionTime);
    t3.position.y = cos(sessionTime);
    t3.position.z = 8 + sin(sessionTime) * 2 + cos(sessionTime) * 2;
}