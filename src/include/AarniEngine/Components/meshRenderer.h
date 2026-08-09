#ifndef _MESHRENDERERH_
#define _MESHRENDERERH_

#include <AarniEngine/renderEngine.h>
#include <AarniEngine/Components/camera.h>

class MeshRenderer: public Component
{
    public:
        MeshRenderer()
        {
            componentType = meshRenderer;
        }

        Mesh *mesh;

        void Start() override
        {
        }

        void Update(double deltaTime) override
        {
            Vector3 pos = GetWorldPosition();
            Mat4x4 matRotZ = {0};
            Mat4x4 matRotX = {0};
            float fTheta = 1.0f * sessionTime;

            // Rotation Z
            matRotZ.m[0][0] = cosf(fTheta);
            matRotZ.m[0][1] = sinf(fTheta);
            matRotZ.m[1][0] = -sinf(fTheta);
            matRotZ.m[1][1] = cosf(fTheta);
            matRotZ.m[2][2] = 1;
            matRotZ.m[3][3] = 1;

            // Rotation X
            matRotX.m[0][0] = 1;
            matRotX.m[1][1] = cosf(fTheta * 0.5f);
            matRotX.m[1][2] = sinf(fTheta * 0.5f);
            matRotX.m[2][1] = -sinf(fTheta * 0.5f);
            matRotX.m[2][2] = cosf(fTheta * 0.5f);
            matRotX.m[3][3] = 1;

            for(int i = 0; i < mesh->faceCount; i++)
            {
                Face triProjected = Face();
                Face triTranslated = Face();
                Face triRotatedZ = Face();
                Face triRotatedZX = Face();
                
                Vector3 v4 = Vector3::Zero();
                Vector3 v5 = Vector3::Zero();
                Vector3 v6 = Vector3::Zero();
                triRotatedZ.a = &v4;
                triRotatedZ.b = &v5;
                triRotatedZ.c = &v6;
                Vector3 v7 = Vector3::Zero();
                Vector3 v8 = Vector3::Zero();
                Vector3 v9 = Vector3::Zero();
                triRotatedZX.a = &v7;
                triRotatedZX.b = &v8;
                triRotatedZX.c = &v9;
                Face *f = &(*mesh->faces)[i];

                Mat4x4::MultiplyMatrixVector(*f->a, *triRotatedZ.a, matRotZ);
                Mat4x4::MultiplyMatrixVector(*f->b, *triRotatedZ.b, matRotZ);
                Mat4x4::MultiplyMatrixVector(*f->c, *triRotatedZ.c, matRotZ);

                // Rotate in X-Axis
                Mat4x4::MultiplyMatrixVector(*triRotatedZ.a, *triRotatedZX.a, matRotX);
                Mat4x4::MultiplyMatrixVector(*triRotatedZ.b, *triRotatedZX.b, matRotX);
                Mat4x4::MultiplyMatrixVector(*triRotatedZ.c, *triRotatedZX.c, matRotX);

                Vector3 v1 = Vector3::Zero();
                Vector3 v2 = Vector3::Zero();
                Vector3 v3 = Vector3::Zero();
                triProjected.a = &v1;
                triProjected.b = &v2;
                triProjected.c = &v3;
                
                Vector3 v10 = *triRotatedZX.a + pos;
                Vector3 v11 = *triRotatedZX.b + pos;
                Vector3 v12 = *triRotatedZX.c + pos;

                triTranslated.a = &v10;
                triTranslated.b = &v11;
                triTranslated.c = &v12;
                
                Vector3 line1 = *triTranslated.b - *triTranslated.a;
                Vector3 line2 = *triTranslated.c - *triTranslated.a;

                Vector3 normal = Vector3::Normalize(Vector3::CrossProduct(line1,line2));
                if(Vector3::DotProduct(normal, *triTranslated.a - mainCamera->GetWorldPosition()) < 0.0f)
                {
                    //Project triangles from 3D --> 2D
                    Mat4x4::MultiplyMatrixVector(*triTranslated.a, *triProjected.a, mainCamera->projectionMatrix);
                    Mat4x4::MultiplyMatrixVector(*triTranslated.b, *triProjected.b, mainCamera->projectionMatrix);
                    Mat4x4::MultiplyMatrixVector(*triTranslated.c, *triProjected.c, mainCamera->projectionMatrix);
                    
                    //Scale to view.
                    triProjected.a->x += 1.0f; triProjected.a->y += 1.0f;
                    triProjected.b->x += 1.0f; triProjected.b->y += 1.0f;
                    triProjected.c->x += 1.0f; triProjected.c->y += 1.0f;
                    triProjected.a->x *= 0.5f * (float)screenWidth;
                    triProjected.b->x *= 0.5f * (float)screenWidth;
                    triProjected.c->x *= 0.5f * (float)screenWidth;
                    triProjected.a->y *= 0.5f * (float)screenHeigth;
                    triProjected.b->y *= 0.5f * (float)screenHeigth;
                    triProjected.c->y *= 0.5f * (float)screenHeigth;
                    
                    const std::vector< SDL_Vertex > verts =
                    {
                        { SDL_FPoint{ triProjected.a->x, triProjected.a->y }, SDL_Color{ 255, 0, 0, 255 }, SDL_FPoint{ 0 }, },
                        { SDL_FPoint{ triProjected.b->x, triProjected.b->y }, SDL_Color{ 0, 255, 0, 255 }, SDL_FPoint{ 0 }, },
                        { SDL_FPoint{ triProjected.c->x, triProjected.c->y }, SDL_Color{ 0, 0, 255, 255 }, SDL_FPoint{ 0 }, },
                    };
                    SDL_RenderGeometry( RenderInformation, nullptr, verts.data(), verts.size(), nullptr, 0 );

                }

                
                
            }
            
        }
};

#endif