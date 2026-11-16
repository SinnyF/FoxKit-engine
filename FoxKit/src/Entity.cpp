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

Entity::Entity(const string &_modelPath, const string &_texturePath, unsigned int _shaderID)
{
    modelPath = _modelPath;
    texturePath = _texturePath;
    shaderID = _shaderID;
}

void Entity::Translate()
{
    modelMatrix = mat4(1.0f);
    
    modelMatrix = translate(modelMatrix, position);
    vec3 axis = vec3(0);
    if (glm::abs(rotation.x) > 0) modelMatrix = rotate(modelMatrix, radians(rotation.x), vec3(1.0f, 0.0f, 0.0f));
    if (glm::abs(rotation.y) > 0) modelMatrix = rotate(modelMatrix, radians(rotation.y), vec3(0.0f, 1.0f, 0.0f));
    if (glm::abs(rotation.z) > 0) modelMatrix = rotate(modelMatrix, radians(rotation.z), vec3(0.0f, 0.0f, 1.0f));
    modelMatrix = glm::scale(modelMatrix, scale);
}
