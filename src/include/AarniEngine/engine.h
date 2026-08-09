#ifndef _ENGINEH_
#define _ENGINEH_

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <vector>

#ifdef __linux__
	#include <unistd.h>

    void Sleep(int milliseconds)
    {
        usleep(milliseconds*1000);
    }
#endif
#ifdef __MINGW32__
    #include <windows.h>
#endif

//SDL2
#include <SDL2/SDL.h>

// ----- Time ----- //
double deltaTime;
clock_t t1, t2; //Time at start and end of the frame
double elapsedTime = 0;
double sessionTime = 0; //Total time the session has been on in milliseconds.
int fpsLimiter = 60;


const int screenWidth = 1000;
const int screenHeigth = 1000;
SDL_Window *Window;
SDL_Renderer *RenderInformation;

//My own libraries
#include <AarniEngine/input.h>
#include <AarniEngine/Math/vector.h>
#include <AarniEngine/Math/color.h>
#include <AarniEngine/Math/curve.h>
#include <AarniEngine/Math/quaternion.h>
#include <AarniEngine/Math/mat4x4.h>
#include <AarniEngine/Graphics/mesh.h>

#include <AarniEngine/component.h>

#include <AarniEngine/Components/transform.h>
#include <AarniEngine/Components/camera.h>
#include <AarniEngine/Components/circleCollider.h>
#include <AarniEngine/Components/spriteRenderer.h>

Component *root = new Component(); //root object. Every thing will be built on top of this. Components consists an array of child components.

Camera *mainCamera = nullptr;

#include <AarniEngine/renderEngine.h>

#include <AarniEngine/Components/meshRenderer.h>

bool createWindow()
{
    SDL_Init(SDL_INIT_VIDEO);

    Window = SDL_CreateWindow("SDL Practice",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,screenWidth,screenHeigth,SDL_WINDOW_ALLOW_HIGHDPI);
    if (Window != NULL)
    {
        RenderInformation = SDL_CreateRenderer(Window, -1, 0);
        std::cout << "Did create a window!" << std::endl; 
    }
    else
    {
        std::cout << "Could not create window: " << SDL_GetError() << std::endl;
    }
    return Window != NULL;
}

void closeWindow()
{
    SDL_DestroyWindow(Window);
    SDL_Quit();
}

//Function declaration
void Start();
void Update();
bool endApp = false;

int main(int argc, char *argv[])
{
    srand(time(NULL));
    std::cout << "Engine starting" << std::endl;
    
    if(createWindow() == false)
    {
        return 1;
    }

    ReadInputConfig();
    PrintAllActions();

    Start();
    //Calls update on all components
    root->StartRecursive();
    
    //Begining of calculating time.
    t1 = clock();
    
    while (true)
    {
        SDL_Event Event;
        //Ending application.
        if (SDL_PollEvent(&Event))
        {
            if (SDL_QUIT == Event.type)
            {
                endApp = true;
            }
        }
        if(endApp)
        {
            break;
        }

        clearFrame();
        
        UpdateInputs();
        Update();
        UpdatePreviousInputs();
        root->UpdateRecursive(deltaTime);
        renderFrame();

        //fps limiter
        Sleep(std::max(0.0,(1000 / fpsLimiter) - (deltaTime * 1000)));

        //Calculating passing time.
        t2 = clock();
        deltaTime = ((double)(t2 - t1)) / ((double)CLOCKS_PER_SEC);
        t1 = t2;
        sessionTime += deltaTime;
    }

    closeWindow();
    return 1;
}

void QuitApplication(){
    endApp = true;
}

#endif