#ifndef _CIRCLECOLLIDERH_
#define _CIRCLECOLLIDERH_

#include <AarniEngine/component.h>

class CircleCollider: public Component
{
    public:
        CircleCollider()
        {
            componentType = circleCollider2D;
        }

        float radius = 10.0f;
        Vector2 offset = {0,0};
        void Start() override
        {
        }

        void Update(double deltaTime) override
        {

        }

        void OnCollisionEnter()
        {
            std::cout << "COLLIDING!" << std::endl;
        }
};

#endif