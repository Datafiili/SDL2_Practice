#ifndef _SPRITERENDERERH_
#define _SPRITERENDERERH_

#include <AarniEngine/component.h>
#include <AarniEngine/Graphics/sprite.h>
#include <AarniEngine/renderEngine.h>
#include <AarniEngine/Math/vector.h>

class SpriteRenderer: public Component
{
    public:
        SpriteRenderer()
        {
            componentType = spriteRenderer;
        }

        Sprite *sprite;
        Vector2Int position = Vector2Int::Zero();
        Vector2Int size = Vector2Int::Zero();

        void Start() override
        {
        }

        void Update(double deltaTime)
        {
            if(sprite != nullptr)
            {
                //renderSprite(*sprite, {position.x, position.y, sprite->pixelCountHorizontal(), sprite->pixelCountVertical()});
            }
        }
};

#endif