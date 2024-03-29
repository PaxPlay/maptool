#include "VertexArray.h"

#include <GL/glew.h>
#include <utility>

VertexArray::VertexArray(std::vector<glm::vec3> *vertices, std::vector<unsigned int> *indices, Type type)
    : indices(indices), type(type)
{
    initialize(vertices);
}

template<>
void VertexArray::addBuffer<glm::vec3>(const std::string &name, std::vector<glm::vec3> *elements, unsigned int layoutIndex)
{
    addBuffer(name, std::make_unique<GenericBufferObject<glm::vec3>>(elements, 3, GL_FLOAT), layoutIndex);
}

void VertexArray::initialize(std::vector<glm::vec3> *vertices)
{

    glGenVertexArrays(1, &vertexArray);
    glBindVertexArray(vertexArray);

    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices->size() * sizeof(unsigned int), indices->data(), GL_STATIC_DRAW);

    glBindVertexArray(0);

    addBuffer<glm::vec3>("vertices", vertices, 0);
}

void VertexArray::addBuffer(const std::string &name, std::unique_ptr<VertexArray::BufferObject> buffer, unsigned int layoutIndex)
{
    buffers[name] = std::move(buffer);

    glBindVertexArray(vertexArray);

    glGenBuffers(1, &buffers[name]->object);
    glBindBuffer(GL_ARRAY_BUFFER, buffers[name]->object);
    glBufferData(GL_ARRAY_BUFFER, buffers[name]->totalSize(), buffers[name]->getData(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(layoutIndex);
    glVertexAttribPointer(layoutIndex, buffers[name]->getSize(), buffers[name]->typeId(), GL_FALSE,
                          buffers[name]->blockSize(),
                          nullptr);

    glBindVertexArray(0);
}

void VertexArray::draw()
{
    glBindVertexArray(vertexArray);
    GLenum mode = GL_TRIANGLES;
    switch (type)
    {
        case Type::TRIANGLES:
            mode = GL_TRIANGLES;
            break;
        case Type::TRIANGLE_STRIP:
            mode = GL_TRIANGLE_STRIP;
            break;
    }
    glDrawElements(mode, indices->size(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}
