#include <vector>
#include "Blades.h"
#include "BufferUtils.h"

float generateRandomFloat() {
    return rand() / (float)RAND_MAX;
}

Blades::Blades(Device* device, VkCommandPool commandPool, float planeDim) : Model(device, commandPool, {}, {}) {
    transferCommandPool = commandPool;
    std::vector<Blade> blades;
    blades.reserve(NUM_BLADES);

    for (int i = 0; i < NUM_BLADES; i++) {
        Blade currentBlade = Blade();

        glm::vec3 bladeUp(0.0f, 1.0f, 0.0f);

        // Generate positions and direction (v0)
        float x = (generateRandomFloat() - 0.5f)* planeDim;
        float y = 0.0f;
        float z = (generateRandomFloat() - 0.5f)* planeDim;
        float direction = generateRandomFloat() * 2.f * 3.14159265f;
        glm::vec3 bladePosition(x, y, z);
        currentBlade.v0 = glm::vec4(bladePosition, direction);

        // Bezier point and height (v1)
        float height = MIN_HEIGHT + (generateRandomFloat() * (MAX_HEIGHT - MIN_HEIGHT));
        currentBlade.v1 = glm::vec4(bladePosition + bladeUp * height, height);

        // Physical model guide and width (v2)
        float width = MIN_WIDTH + (generateRandomFloat() * (MAX_WIDTH - MIN_WIDTH));
        currentBlade.v2 = glm::vec4(bladePosition + bladeUp * height, width);

        // Up vector and stiffness coefficient (up)
        float stiffness = MIN_BEND + (generateRandomFloat() * (MAX_BEND - MIN_BEND));
        currentBlade.up = glm::vec4(bladeUp, stiffness);

        blades.push_back(currentBlade);
    }

    BladeDrawIndirect indirectDraw;
    indirectDraw.vertexCount = 0;
    indirectDraw.instanceCount = 1;
    indirectDraw.firstVertex = 0;
    indirectDraw.firstInstance = 0;

    BufferUtils::CreateBufferFromData(device, commandPool, blades.data(), NUM_BLADES * sizeof(Blade), VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, bladesBuffer, bladesBufferMemory);
    BufferUtils::CreateBuffer(device, NUM_BLADES * sizeof(Blade), VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, culledBladesBuffer, culledBladesBufferMemory);
    
    
    {
        VkDeviceSize bufferSize = sizeof(Params);
        VkBufferUsageFlags bufferUsage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
        VkBufferUsageFlags stagingUsage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
        VkMemoryPropertyFlags stagingProperties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
        BufferUtils::CreateBuffer(device, bufferSize, stagingUsage, stagingProperties, paramStagingBuffer, paramStagingBufferMemory);
        // Create the buffer
        VkBufferUsageFlags usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | bufferUsage;
        VkMemoryPropertyFlags flags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
        BufferUtils::CreateBuffer(device, bufferSize, usage, flags, paramUniformBuffer, paramUniformBufferMemory);
    }
    
    {
		VkDeviceSize bufferSize = sizeof(BladeDrawIndirect);
		void* bufferData = &indirectDraw;
		VkBufferUsageFlags bufferUsage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT;


        VkBufferUsageFlags stagingUsage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
        VkMemoryPropertyFlags stagingProperties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
        BufferUtils::CreateBuffer(device, bufferSize, stagingUsage, stagingProperties, stagingBuffer, stagingBufferMemory);

        // Fill the staging buffer
        void* data;
        vkMapMemory(device->GetVkDevice(), stagingBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, bufferData, static_cast<size_t>(bufferSize));
        vkUnmapMemory(device->GetVkDevice(), stagingBufferMemory);

        // Create the buffer
        VkBufferUsageFlags usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | bufferUsage;
        VkMemoryPropertyFlags flags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
        BufferUtils::CreateBuffer(device, bufferSize, usage, flags, numBladesBuffer, numBladesBufferMemory);

        // Copy data from staging to buffer
        BufferUtils::CopyBuffer(device, commandPool, stagingBuffer, numBladesBuffer, bufferSize);
    }
}

VkBuffer Blades::GetBladesBuffer() const {
    return bladesBuffer;
}

VkBuffer Blades::GetCulledBladesBuffer() const {
    return culledBladesBuffer;
}

VkBuffer Blades::GetNumBladesBuffer() const {
    return numBladesBuffer;
}

VkBuffer Blades::GetParamBuffer() const {
    return paramUniformBuffer;
}

void Blades::ResetNumBladesBuffer() {
    // Copy data from staging to buffer
    VkDeviceSize bufferSize = sizeof(BladeDrawIndirect);
    BufferUtils::CopyBuffer(device, transferCommandPool, stagingBuffer, numBladesBuffer, bufferSize);
}

void Blades::CopyParams(void* inParamData) {
    // Fill the staging buffer
    VkDeviceSize bufferSize = sizeof(Params);
    void* data;
    vkMapMemory(device->GetVkDevice(), paramStagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, inParamData, static_cast<size_t>(bufferSize));
    vkUnmapMemory(device->GetVkDevice(), paramStagingBufferMemory);
    // Copy data from staging to buffer
    BufferUtils::CopyBuffer(device, transferCommandPool, paramStagingBuffer, paramUniformBuffer, bufferSize);
}

Blades::~Blades() {
    vkDestroyBuffer(device->GetVkDevice(), bladesBuffer, nullptr);
    vkFreeMemory(device->GetVkDevice(), bladesBufferMemory, nullptr);
    vkDestroyBuffer(device->GetVkDevice(), culledBladesBuffer, nullptr);
    vkFreeMemory(device->GetVkDevice(), culledBladesBufferMemory, nullptr);
    vkDestroyBuffer(device->GetVkDevice(), numBladesBuffer, nullptr);
    vkFreeMemory(device->GetVkDevice(), numBladesBufferMemory, nullptr);

    // No need for the staging buffer anymore
    vkDestroyBuffer(device->GetVkDevice(), stagingBuffer, nullptr);
    vkFreeMemory(device->GetVkDevice(), stagingBufferMemory, nullptr);
}
