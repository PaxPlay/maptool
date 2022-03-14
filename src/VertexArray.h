#ifndef MAPTOOL_SRC_VERTEXARRAY_H
#define MAPTOOL_SRC_VERTEXARRAY_H

#include <vector>
#include <map>
#include <string>
#include <memory>
#include <glm/glm.hpp>

class VertexArray {
public:
    VertexArray(std::vector<glm::vec3> vertices, std::vector<unsigned int> indices);

    template<typename T>
    void addBuffer(const std::string& name, std::vector<T> elements, unsigned int layoutIndex);

    void draw();
private:
    void initialize(std::vector<glm::vec3> vertices);

    unsigned int vertexArray = 0;
    unsigned int indexBuffer = 0;
    std::vector<unsigned int> indices;

    class BufferObject {
    public:
        unsigned int object = 0;
        unsigned int index = 0;
        virtual ~BufferObject() = default;
        virtual void* getData() = 0;
        virtual size_t totalSize() = 0;
        virtual size_t numElements() = 0;
        virtual size_t blockSize() = 0;
        virtual size_t getSize() = 0;
        virtual unsigned int typeId() = 0;
    };

    template<typename T>
    struct GenericBufferObject : BufferObject {
    private:
        std::vector<T> elements;
        size_t size;
        unsigned int baseType;
    public:
        GenericBufferObject(std::vector<T> elements, size_t size, unsigned int baseType)
            : elements(std::move(elements)), size(size), baseType(baseType)
        {
        }
        ~GenericBufferObject() override = default;
        void* getData() override { return elements.data(); }
        size_t totalSize() override { return elements.size() * sizeof(T); };
        size_t numElements() override { return elements.size(); }
        size_t blockSize() override { return sizeof(T); }
        size_t getSize() override { return size; };
        unsigned int typeId() override { return baseType; }
    };

    void addBuffer(const std::string& name, std::unique_ptr<BufferObject> buffer, unsigned int layoutIndex);
    std::map<std::string, std::unique_ptr<BufferObject>> buffers;
};


#endif //MAPTOOL_SRC_VERTEXARRAY_H
