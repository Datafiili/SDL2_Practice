#ifndef _COMPONENTH_
#define _COMPONENTH_

enum ComponentType {
    empty,
    transform,
    camera,
    circleCollider2D,
    ball,
    spriteRenderer,
    meshRenderer,
};

std::ostream& operator<<(std::ostream& os, ComponentType c) {
    switch (c) {
        case ComponentType::empty:          return os << "Epmty";
        case ComponentType::transform:      return os << "Transform";
        case ComponentType::camera:         return os << "Camera";
        case ComponentType::meshRenderer:         return os << "MeshRenderer";
    }
    return os << c;
}

class Component
{
    public:
        virtual void Start(){};
        virtual void Update(double deltaTime){};
        virtual void Reset(){};
        virtual ~Component() = default;
        int childCount = 0;
        std::vector<Component*> children;
        Component *parent = nullptr;
        
        enum ComponentType componentType = empty;
        
        Component()
        {
            componentType = empty;
        }

    void StartRecursive() //Recursivly Starts all components.
    {
        Start();
        for(int i = 0; i < childCount; i++)
        {
            children[i]->Start();
            children[i]->StartRecursive();
        }
    }

    void UpdateRecursive(double deltaTime) //Recursivly Updates all components.
    {
        Update(deltaTime);
        for(int i = 0; i < childCount; i++)
        {
            children[i]->UpdateRecursive(deltaTime);
        }
    }

    void AddChild(Component& child)
    {
        child.parent = this;
        children.push_back(&child);
        childCount++;
    }

    int Depth(Component *c = nullptr, int value = 0)
    {
        if(c == nullptr){c = this;}
        if(c->childCount == 0){ return value; }
        int greatest = 0;
        for(int i = 0; i < c->childCount; i++)
        {
            int holder = Depth(c->children[i], value + 1);
            if(holder > greatest)
            {
                greatest = holder;
            }
        }
        return greatest;
    }

    void PrintTree(int level = 0)
    {
        for(int i = 0; i < level; i++)
        {
            std::cout << "\t";
        }
        std::cout << componentType << std::endl;
        for(int i = 0; i < childCount; i++)
        {
            children[i]->PrintTree(level + 1);
        }
    }

    virtual Vector3 GetWorldPosition(Vector3 total = {0,0,0})
    {
        if(parent == nullptr)
        {
            return total;
        }
        return parent->GetWorldPosition(total);
    }
};

#endif