#ifndef _SPRITEH_
#define _SPRITEH_

#include <SDL2/SDL_image.h>
#include <iostream>

struct Sprite
{
    enum Splicing
    {
        GridByCellSize,
        GridByCellCount,
    };

    Splicing splicingMethod = GridByCellCount;

    //For cell size -> (width or height) of cell in pixels; for cell count -> (horizontal / vertical) count of cells
    int horizontalCount;
    int verticalCount;

    char imageName[128];
    int spriteIndex; //Which image of the spritesheet is used.
    SDL_Surface *sourceImage;

    Sprite(const char* path = "", Splicing splicing = Splicing::GridByCellCount, int horCount = 1, int verCount = 1, int index = 0)
    {
        splicingMethod = splicing;
        horizontalCount = horCount;
        verticalCount = verCount;
        spriteIndex = index;
        loadTexture(path);
    }

    void loadTexture(const char* path)
    {
        char buffer[128];
        strcpy(buffer, "src/Game/Sprites/");
        strcat(buffer, path);
        
        strcpy(imageName, path);
        sourceImage = IMG_Load(buffer);
        if(!sourceImage)
        {
            SDL_Log("IMG_Load failed: %s", IMG_GetError());
        }
    }

    int pixelCountHorizontal()
    {
        switch (splicingMethod)
        {
            case Splicing::GridByCellCount:
                if(sourceImage == NULL) {return 1;}
                return (sourceImage->w - sourceImage->w % horizontalCount) / horizontalCount;
            case Splicing::GridByCellSize:
                return horizontalCount;
                return 1; //Missing image will be treated as singular image.

            default:
                return 1; //Missing image will be treated as singular image.
        }
    }

    int pixelCountVertical()
    {
        switch (splicingMethod)
        {
            case Splicing::GridByCellCount:
                if(sourceImage == NULL) {return 1;}
                return (sourceImage->h - sourceImage->h % verticalCount) / verticalCount;
            case Splicing::GridByCellSize:
                return verticalCount;
                return 1; //Missing image will be treated as singular image.

            default:
                return 1; //Missing image will be treated as singular image.
        }
    }

    int pixelCount()
    {
        return pixelCountHorizontal() * pixelCountVertical();
    }

    int CellCountHorizontal()
    {
        switch (splicingMethod)
        {
            case Splicing::GridByCellCount:
                return horizontalCount;

            case Splicing::GridByCellSize:
                if(sourceImage != NULL)
                {
                    return ((int)sourceImage->w / horizontalCount);
                }
                return 1; //Missing image will be treated as singular image.

            default:
                return 1; //Missing image will be treated as singular image.
        }
    }

    int CellCountVertical()
    {
        switch (splicingMethod)
        {
            case Splicing::GridByCellCount:
                return verticalCount;

            case Splicing::GridByCellSize:
                if(sourceImage != NULL)
                {
                    return ((int)sourceImage->h / verticalCount);
                }
                return 1; //Missing image will be treated as singular image.

            default:
                return 1; //Missing image will be treated as singular image.
        }
    }

    int CellCount()
    {
        switch (splicingMethod)
        {
            case Splicing::GridByCellCount:
                return horizontalCount * verticalCount;

            case Splicing::GridByCellSize:
                if(sourceImage != NULL)
                {
                    return CellCountHorizontal() * CellCountVertical();
                }
                return 1; //Missing image will be treated as singular image.

            default:
                return 1; //Missing image will be treated as singular image.
        }
    }

    SDL_Rect GetSrcRect(int index)
    {
        if(sourceImage == NULL)
        {
            std::cout << "Missing image source of " << imageName << std::endl;
            return {0,0,0,0};
        }
        if(index >= CellCount())
        {
            return {0,0,0,0};
        }

        switch (splicingMethod)
        {
            case Splicing::GridByCellCount:
            {
                int cellWidth = pixelCountHorizontal();
                int cellHeight = pixelCountVertical();
                int column = index % horizontalCount;
                int row = (index - column) / horizontalCount;
                SDL_Rect rect = {column * cellWidth, row * cellHeight, cellWidth, cellHeight};
                return rect;
            }
                
            case Splicing::GridByCellSize:
            {
                int column = index % CellCountHorizontal();
                int row = index / CellCountHorizontal();
                SDL_Rect rect = {column * horizontalCount, row * verticalCount, horizontalCount, verticalCount};
                return rect;
            }
                
            default:
                std::cout << "Invalid splicing method" << std::endl;
                return {0,0,0,0};
        }
    }
};

#endif