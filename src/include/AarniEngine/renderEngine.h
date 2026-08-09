#ifndef _RENDERENGINE_
#define _RENDERENGINE_

#include <SDL2/SDL_image.h>
#include <string>

int backgroundColour[3] = {125,125,125};

void drawCircle(int xc, int yc, int x, int y, Color color = {0,0,0,255}){
    SDL_SetRenderDrawColor(RenderInformation,color.r, color.g, color.b, color.a);
    SDL_RenderDrawPoint(RenderInformation, xc+x, yc+y); //0-45deg
    SDL_RenderDrawPoint(RenderInformation, xc+y, yc+x); //45-90
    SDL_RenderDrawPoint(RenderInformation, xc+y, yc-x); //90-135
    SDL_RenderDrawPoint(RenderInformation, xc+x, yc-y); //135-180
    SDL_RenderDrawPoint(RenderInformation, xc-x, yc-y); //180-225
    SDL_RenderDrawPoint(RenderInformation, xc-y, yc-x); //225-270
    SDL_RenderDrawPoint(RenderInformation, xc-y, yc+x); //270-315
    SDL_RenderDrawPoint(RenderInformation, xc-x, yc+y); //315-360
}

void circleBres(int xc, int yc, int r, Color color = {0,0,0,255}){
    int y = 0, x = r;
    int d = 3 - 2 * r;
    drawCircle(xc, yc, x, y, color);

    for(int y = 0; y <= x; y++)
    {
        if (d > 0) {
            x--; 
            d += 4 * (y - x) + 10;
        }
        else
        {
            d += 4*y + 6;
        }
        drawCircle(xc, yc, x, y, color);
    }
}

void drawLineSimple(int x1, int y1, int x2, int y2, Color color = {0,0,0,255}) //Bresenham's line algorithm
{
    SDL_SetRenderDrawColor(RenderInformation,color.r, color.g, color.b, color.a);
    int dx = x2 - x1;
    int dy = y2 - y1;
    if(abs(dx) >= abs(dy)) //Horizontal draw.
    {
        float m = Vector2Int::Slope({x1,y1},{x2,y2});
        if(x2 < x1) //Swaping oreder of value for loop to function.
        {
            std::swap(x1,x2);
            std::swap(y1,y2);
        }

        for(int x = x1; x < x2 ; x++)
        {
            SDL_RenderDrawPoint(RenderInformation, x, y1 + m * (x - x1));
        }
    }
    else //Vertical draw.
    {
        float m = Vector2Int::Slope({y1,x1},{y2,x2});
        if(y2 < y1) //Swaping oreder of value for loop to function.
        {
            std::swap(x1,x2);
            std::swap(y1,y2);
        }
        
        for(int y = y1; y < y2; y++)
        {
            SDL_RenderDrawPoint(RenderInformation, m*(y-y1) + x1, y);
        }
    }
}

void drawLineSimple(Vector2Int v1, Vector2Int v2, Color color = {0,0,0,255})
{
    drawLineSimple(v1.x, v1.y, v2.x, v2.y, color);
}

void drawLineSimple(Vector2 v1, Vector2 v2, Color color = {0,0,0,255})
{
    drawLineSimple(static_cast<int>(v1.x), static_cast<int>(v1.y), static_cast<int>(v2.x), static_cast<int>(v2.y), color);
}

void drawLineSimple(float x1, float y1, float x2, float y2, Color color = {0,0,0,255})
{
    drawLineSimple(static_cast<int>(x1), static_cast<int>(y1), static_cast<int>(x2), static_cast<int>(y2), color);
}

void drawLineSimple(int x1, int y1, int x2, int y2, ColorGradient color)
{
    int dx = x2 - x1;
    int dy = y2 - y1;
    if(abs(dx) >= abs(dy)) //Horizontal draw.
    {
        float m = Vector2Int::Slope({x1,y1},{x2,y2});
        if(x2 < x1) //Swaping oreder of value for loop to function.
        {
            std::swap(x1,x2);
            std::swap(y1,y2);
        }

        for(int x = x1; x < x2 ; x++)
        {
            float pros = ((float)x - (float)x1) / ((float)x2 - (float)x1);
            Color c = color.GetColor(pros);
            SDL_SetRenderDrawColor(RenderInformation,c.r, c.g, c.b, c.a);
            SDL_RenderDrawPoint(RenderInformation, x, y1 + m * (x - x1));
        }
    }
    else //Vertical draw.
    {
        float m = Vector2Int::Slope({y1,x1},{y2,x2});
        if(y2 < y1) //Swaping oreder of value for loop to function.
        {
            std::swap(x1,x2);
            std::swap(y1,y2);
        }
        
        for(int y = y1; y < y2; y++)
        {
            float pros = ((float)y - (float)y1) / ((float)y2 - (float)y1);
            Color c = color.GetColor(pros);
            SDL_SetRenderDrawColor(RenderInformation,c.r, c.g, c.b, c.a);
            SDL_RenderDrawPoint(RenderInformation, m*(y-y1) + x1, y);
        }
    }
}

void drawLineSimple(Vector2Int v1, Vector2Int v2, ColorGradient color)
{
    drawLineSimple(v1.x, v1.y, v2.x, v2.y, color);
}

void drawLineSimple(Vector2 v1, Vector2 v2, ColorGradient color)
{
    drawLineSimple(static_cast<int>(v1.x), static_cast<int>(v1.y), static_cast<int>(v2.x), static_cast<int>(v2.y), color);
}

void drawLineSimple(float x1, float y1, float x2, float y2, ColorGradient color)
{
    drawLineSimple(static_cast<int>(x1), static_cast<int>(y1), static_cast<int>(x2), static_cast<int>(y2), color);
}

void drawLineAntiAliasing(float x1, float y1, float x2, float y2, Color color= {0,0,0,255})
{
    int steep = abs(y2 - y1) > abs(x2 - x1);
    if(steep) //Swapping horizontal with vectical.
    {
        std::swap(x1,y1);
        std::swap(x2,y2);
    }
    if(x1 > x2) //Swapping first one to be most left one.
    {
        std::swap(x1,x2);
        std::swap(y1,y2);
    }

    float m = Vector2::Slope({x1,y1},{x2,y2});
    if(x2 - x1 == 0)
    {
        m = 1;
    }
    float intersectY = y1;

    if(steep)
    {
        int x;
        for(x = x1; x <= x2; x++)
        {
            SDL_SetRenderDrawColor(RenderInformation,color.r, color.g, color.b, color.a * (intersectY - int(intersectY)));
            SDL_RenderDrawPoint(RenderInformation, int(intersectY), x);
            SDL_SetRenderDrawColor(RenderInformation,color.r, color.g, color.b, color.a * (1 - (intersectY - int(intersectY))));
            SDL_RenderDrawPoint(RenderInformation, int(intersectY) - 1, x);
            intersectY += m;
        }
    }
    else
    {
        for(int x = x1; x <= x2; x++)
        {
            SDL_SetRenderDrawColor(RenderInformation,color.r, color.g, color.b, color.a * (intersectY - int(intersectY)));
            SDL_RenderDrawPoint(RenderInformation,x, int(intersectY));
            SDL_SetRenderDrawColor(RenderInformation,color.r, color.g, color.b, color.a * (1 - (intersectY - int(intersectY))));
            SDL_RenderDrawPoint(RenderInformation, x, int(intersectY) - 1);
            intersectY += m;
        }
    }
}

void drawLineAntiAliasing(Vector2 v1, Vector2 v2, Color color= {0,0,0,255})
{
    drawLineAntiAliasing(v1.x, v1.y, v2.x, v2.y, color);
}

void drawLineAntiAliasing(int x1, int y1, int x2, int y2, Color color= {0,0,0,255})
{
    drawLineAntiAliasing(static_cast<float>(x1),static_cast<float>(y1),static_cast<float>(x2),static_cast<float>(y2),color);
}

void drawLineAntiAliasing(Vector2Int v1, Vector2Int v2, Color color= {0,0,0,255})
{
    drawLineAntiAliasing(static_cast<float>(v1.x),static_cast<float>(v1.y),static_cast<float>(v2.x),static_cast<float>(v2.y),color);
}

void drawLineAntiAliasing(float x1, float y1, float x2, float y2, ColorGradient color)
{
    int steep = abs(y2 - y1) > abs(x2 - x1);
    if(steep) //Swapping horizontal with vectical.
    {
        std::swap(x1,y1);
        std::swap(x2,y2);
    }
    if(x1 > x2) //Swapping first one to be most left one.
    {
        std::swap(x1,x2);
        std::swap(y1,y2);
    }

    float m = Vector2::Slope({x1,y1},{x2,y2});
    if(x2 - x1 == 0)
    {
        m = 1;
    }
    float intersectY = y1;

    if(steep)
    {
        int x;
        for(x = x1; x <= x2; x++)
        {
            float pros = ((float)x - (float)x1) / ((float)x2 - (float)x1);
            Color c = color.GetColor(pros);
            SDL_SetRenderDrawColor(RenderInformation,c.r, c.g, c.b, c.a * (intersectY - int(intersectY)));
            SDL_RenderDrawPoint(RenderInformation, int(intersectY), x);
            SDL_SetRenderDrawColor(RenderInformation,c.r, c.g, c.b, c.a * (1 - (intersectY - int(intersectY))));
            SDL_RenderDrawPoint(RenderInformation, int(intersectY) - 1, x);
            intersectY += m;
        }
    }
    else
    {
        for(int x = x1; x <= x2; x++)
        {
            float pros = ((float)x - (float)x1) / ((float)x2 - (float)x1);
            Color c = color.GetColor(pros);
            SDL_SetRenderDrawColor(RenderInformation, c.r, c.g, c.b, c.a * (intersectY - int(intersectY)));
            SDL_RenderDrawPoint(RenderInformation,x, int(intersectY));
            SDL_SetRenderDrawColor(RenderInformation, c.r, c.g, c.b, c.a * (1 - (intersectY - int(intersectY))));
            SDL_RenderDrawPoint(RenderInformation, x, int(intersectY) - 1);
            intersectY += m;
        }
    }
}

void drawLineAntiAliasing(Vector2 v1, Vector2 v2, ColorGradient color)
{
    drawLineAntiAliasing(v1.x, v1.y, v2.x, v2.y, color);
}

void drawLineAntiAliasing(int x1, int y1, int x2, int y2, ColorGradient color)
{
    drawLineAntiAliasing(static_cast<float>(x1),static_cast<float>(y1),static_cast<float>(x2),static_cast<float>(y2),color);
}

void drawLineAntiAliasing(Vector2Int v1, Vector2Int v2, ColorGradient color)
{
    drawLineAntiAliasing(static_cast<float>(v1.x),static_cast<float>(v1.y),static_cast<float>(v2.x),static_cast<float>(v2.y),color);
}

void drawLineCurve(int x1, int y1, int x2, int y2, Curve curve, float curveHeight,  Color color = {0,0,0,255})
{
    SDL_SetRenderDrawColor(RenderInformation,color.r, color.g, color.b, color.a);
    float m = Vector2::Slope({x1,y1},{x2,y2});
    float m2 = m + M_PI / 2; //90deg angle.
    
    int i = 0;
    Vector2 posLine1 = {x1 + i, y1 + i * m};
    Vector2 posLine2 = Vector2::Zero();
    while(i < x2 - x1)
    {
        posLine2 = {x1 + i + 1, y1 + (i + 1) * m};
        //float steepness = Vector2::Slope(posLine1,posLine2);
        float pros = (float)i / (float)(x2 - x1);
        
        Vector2 offDir = Vector2(cos(m2),sin(m2));
        Vector2 targetPos = posLine1 + (offDir * curve.GetValue(pros) * curveHeight); 
        SDL_RenderDrawPoint(RenderInformation, targetPos.x, targetPos.y);
        
        i++;
        posLine1 = posLine2;
    }
}

void drawLineCurve(float x1, float y1, float x2, float y2, Curve curve, float curveHeight, Color color = {0,0,0,255})
{
    drawLineCurve((int)x1, (int)y1, (int)x2, (int)y2, curve, curveHeight, color);
}

void drawLineCurve(Vector2 v1, Vector2 v2, Curve curve, float curveHeight, Color color = {0,0,0,255})
{
    drawLineCurve((int)v1.x, (int)v1.y, (int)v2.x, (int)v2.y, curve, curveHeight, color);
}

void drawLineCurve(Vector2Int v1, Vector2Int v2, Curve curve, float curveHeight, Color color = {0,0,0,255})
{
    drawLineCurve(v1.x, v1.y, v2.x, v2.y, curve, curveHeight, color);
}

void drawLineCurve(int x1, int y1, int x2, int y2, Curve curve, float curveHeight, ColorGradient cg)
{
    float m = Vector2::Slope({x1,y1},{x2,y2});
    float m2 = m + M_PI / 2; //90deg angle.
    
    int i = 0;
    Vector2 posLine1 = {x1 + i, y1 + i * m};
    Vector2 posLine2 = Vector2::Zero();
    while(i < x2 - x1)
    {
        posLine2 = {x1 + i + 1, y1 + (i + 1) * m};
        //float steepness = Vector2::Slope(posLine1,posLine2);
        float pros = (float)i / (float)(x2 - x1);
        
        Vector2 offDir = Vector2(cos(m2),sin(m2));
        Vector2 targetPos = posLine1 + (offDir * curve.GetValue(pros) * curveHeight);
        Color c = cg.GetColor(pros);
        //std::cout << "Pros: " << pros << " COLOR: " << c << std::endl;
        SDL_SetRenderDrawColor(RenderInformation, c.r, c.g, c.b, c.a);
        SDL_RenderDrawPoint(RenderInformation, targetPos.x, targetPos.y);
        posLine1 = posLine2;
        i++;
    }
}

void drawLineCurve(float x1, float y1, float x2, float y2, Curve curve, float curveHeight, ColorGradient cg)
{
    drawLineCurve((int)x1, (int)y1, (int)x2, (int)y2, curve, curveHeight, cg);
}

void drawLineCurve(Vector2 v1, Vector2 v2, Curve curve, float curveHeight, ColorGradient cg)
{
    drawLineCurve((int)v1.x, (int)v1.y, (int)v2.x, (int)v2.y, curve, curveHeight, cg);
}

void drawLineCurve(Vector2Int v1, Vector2Int v2, Curve curve, float curveHeight, ColorGradient cg)
{
    drawLineCurve(v1.x, v1.y, v2.x, v2.y, curve, curveHeight, cg);
}

void drawRectangle(int x1, int y1, int x2, int y2, Color color = {0,0,0,255})
{
    drawLineSimple(x1,y1,x2,y1,color);
    drawLineSimple(x2,y1,x2,y2,color);
    drawLineSimple(x2,y2,x1,y2,color);
    drawLineSimple(x1,y2,x1,y1,color);
}

void drawRectangle(float x1, float y1, float x2, float y2, Color color = {0,0,0,255})
{
    drawRectangle((int)x1,(int)y1,(int)x2,(int)y2,color);
}

void drawRectangle(Vector2 v1, Vector2 v2, Color color = {0,0,0,255})
{
    drawRectangle((int)v1.x,(int)v1.y,(int)v2.x,(int)v2.y,color);
}

void drawRectangle(Vector2Int v1, Vector2Int v2, Color color = {0,0,0,255})
{
    drawRectangle(v1.x,v1.y,v2.x,v2.y,color);
}

void drawRectangleFillder(int x1, int y1, int x2, int y2, Color color = {0,0,0,255})
{
    for(int i = 0; i < y2 - y1; i++)
    {
        drawLineSimple(x1,y1+i,x2,y1+i,color);
    }
}

void drawRectangleFillder(float x1, float y1, float x2, float y2, Color color = {0,0,0,255})
{
    drawRectangleFillder((int)x1,(int)y1,(int)x2,(int)y2,color);
}

void drawRectangleFillder(Vector2 v1, Vector2 v2, Color color = {0,0,0,255})
{
    drawRectangleFillder((int)v1.x,(int)v1.y,(int)v2.x,(int)v2.y,color);
}

void drawRectangleFillder(Vector2Int v1, Vector2Int v2, Color color = {0,0,0,255})
{
    drawRectangleFillder(v1.x,v1.y,v2.x,v2.y,color);
}

void drawTriangle(Vector2 v[3], Color color = {0,0,0,255})
{
    for(int i = 0; i < 3; i++)
    {
        drawLineSimple(v[i],v[(i+1)%3],color);
    }
}

void drawTriangle(Vector2 v1, Vector2 v2, Vector2 v3, Color color = {0,0,0,255})
{
    Vector2 v[3] = {v1,v2,v3};
    drawTriangle(v,color);
}

void drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, Color color = {0,0,0,255})
{
    Vector2 v[3] = {{x1,y1},{x2,y2},{x3,y3}};
    drawTriangle(v,color);
}

void drawTriangle(float x1, float y1, float x2, float y2, float x3, float y3, Color color = {0,0,0,255})
{
    Vector2 v[3] = {{x1,y1},{x2,y2},{x3,y3}};
    drawTriangle(v,color);
}

void drawTriangleFill(Vector2 v[3], Color color = {0,0,0,255})
{
    //TODO: Figure out
    std::cout << "## ----- Draw Triangel Fill ----- ##" << std::endl;
    std::cout << "Vectors:" << std::endl;
    for(int i = 0; i < 3; i++)
    {
        std::cout << "\t v" << i << " " << v[i] << std::endl;
    }
    //Draw from longest to a side.
    float distances[3];

    int longest = 0;
    int greatestDistance = 0;
    
    for(int i = 0; i < 3; i++)
    {
        distances[i] = Vector2::Distance(v[i % 3],v[(i+1) % 3]);
        if(distances[i] > greatestDistance)
        { 
            longest = i;
            greatestDistance = distances[i];
        }
    }

    std::cout << "Longest side is " << longest << std::endl;
    

    std::cout << "Distances:" << std::endl;
    for(int i = 0; i < 3; i++)
    {
        std::cout << "\t" << distances[i] << std::endl;
    }
    
    float m1 = Vector2::Slope(v[0],v[1]);    
    float m2 = Vector2::Slope(v[1],v[2]);
    float m3 = Vector2::Slope(v[2],v[0]);
    float dist = Vector2::Distance(v[2],v[1]);
    /*for(int i = 0; i < v[1].x - v[0].x; i++)
    {
        float pros = i / (v[1].x - v[0].x);
        drawLineSimple(v[0].x + i, v[0].y + i*m1, v[2].x - pros * (v[2].x - v[1].x), v[2].y - dist * pros);
    }*/
    std::cout << "M1: " << m1 << std::endl;
    std::cout << "M2: " << m2 << std::endl;
    std::cout << "M3: " << m3 << std::endl;

    int index = 0;
    if(abs(m2) < abs(m1)){index = 1;}
    if(abs(m3) < abs(m1) && abs(m3) < abs(m2)){index = 2;}

    std::cout << "index: " << index << std::endl;
    
    int dir = 1;
    if(v[(index + 1) % 3].x - v[index].x < 0){dir = -1;}

    for(int i = 0; i < abs(v[(index + 1) % 3].x - v[index].x); i++)
    {
        float pros = i / (v[(index + 1) % 3].x - v[index].x);
        float x1 = v[index].x + i * dir;
        float y1 = v[index].y + i * dir * m1;
        float x2 = v[(index + 2) % 3].x + i * dir;
        float y2 = v[(index + 2) % 3].y + i * dir * m1;
        std::cout << "i: " << i << " x1: " << x1 << " y1: " << y1 << " x2: " << x2 << " y2: " << y2 << std::endl;

        drawLineSimple(x1,y1,x2,y2);
    }

    /*
    for(int i = 0; i < v[1].x - v[0].x; i++)
    {
        float pros = i / (v[1].x - v[0].x);
        float x1 = v[0].x + i;
        float y1 = v[0].y + i * m1;
        float x2 = v[2].x + i;
        float y2 = v[2].y + i * m1;
        std::cout << "i: " << i << " x1: " << x1 << " y1: " << y1 << " x2: " << x2 << " y2: " << y2 << std::endl;

        drawLineSimple(x1,y1,x2,y2);
    }
    */
}

void drawTriangleFill(Vector2 v1, Vector2 v2, Vector2 v3, Color color = {0,0,0,255})
{
    Vector2 arr[3] = {v1,v2,v3};
    drawTriangleFill(arr,color);
}

void drawTriangleFill(int x1, int y1, int x2, int y2, int x3, int y3, Color color = {0,0,0,255})
{
    Vector2 arr[3] = {{x1,y1},{x2,y2},{x3,y3}};
    drawTriangleFill(arr,color);
}

void drawTriangleFill(float x1, float y1, float x2, float y2, float x3, float y3, Color color = {0,0,0,255})
{
    Vector2 arr[3] = {{x1,y1},{x2,y2},{x3,y3}};
    drawTriangleFill(arr,color);
}

/*
void renderSprite(Sprite sprite, SDL_Rect dstRect)
{
    if(sprite.sourceImage == NULL) //Missing image.
    {
        return;
    }
    
    SDL_Texture* texture = SDL_CreateTextureFromSurface(RenderInformation, sprite.sourceImage);
    SDL_Rect r = sprite.GetSrcRect(sprite.spriteIndex);
    SDL_RenderCopy(RenderInformation, texture, &r, &dstRect);
}
*/

void clearFrame()
{
    // ----- Draw background ----- //
    SDL_SetRenderDrawColor(RenderInformation, backgroundColour[0], backgroundColour[1], backgroundColour[2], 255);
    SDL_RenderClear(RenderInformation); //Fills the screen with the background colour
}

void renderFrame()
{
    SDL_RenderPresent(RenderInformation); //Draws things.
}

#endif