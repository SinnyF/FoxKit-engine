#include "headers/Entity.h"

void Entity::Move(vec3 move)
{
    position = move;
    Translate();
}

void Entity::Rotate(vec3 rotate)
{
    rotation = rotate;
    Translate();
}

void Entity::Scale(vec3 _scale)
{
    scale = _scale;
    Translate();
}

vec3 Entity::getPosition() const
{
    return position;
}

vec3 Entity::getRotation() const
{
    return rotation;
}

vec3 Entity::getScale() const
{
    return scale;
}

Entity::Entity(const string &_modelPath, unsigned int _shaderID, const string &_texturePath, const string &_normalPath)
{
    modelPath = _modelPath;
    texturePath = _texturePath;
    normalPath = _normalPath;
    shaderID = _shaderID;
    modelMatrix = mat4(1.0f);
    Translate();
}

void Entity::setTexture(const string& _texturePath, const string& _normalPath)
{
    texturePath = _texturePath;
    if (!_normalPath.empty())
        normalPath = _normalPath;
}

Entity::Entity()
{
    modelMatrix = mat4(1.0f);
    Translate();
}

vector<Entity*> Entity::GetChildren() const
{
    return children;
}

Entity* Entity::GetParent() const
{
    return parent;
}

void Entity::setParent(Entity* _parent)
{
    this->parent = _parent;
    parent->children.push_back(this);
}

void Entity::Translate()
{        
    modelMatrix = mat4(1.0f);
    if (parent)
        modelMatrix = parent->modelMatrix;
    modelMatrix = translate(modelMatrix, position);
    vec3 axis = vec3(0);
    if (glm::abs(rotation.x) > 0) modelMatrix = rotate(modelMatrix, radians(rotation.x), vec3(1.0f, 0.0f, 0.0f));
    if (glm::abs(rotation.y) > 0) modelMatrix = rotate(modelMatrix, radians(rotation.y), vec3(0.0f, 1.0f, 0.0f));
    if (glm::abs(rotation.z) > 0) modelMatrix = rotate(modelMatrix, radians(rotation.z), vec3(0.0f, 0.0f, 1.0f));
    modelMatrix = glm::scale(modelMatrix, scale);
    for (Entity* e : children)
    {
        e->Translate();
    }
}
