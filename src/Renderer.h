#pragma once

#include "Device.h"
#include "SwapChain.h"
#include "Scene.h"
#include "Camera.h"
struct ImDrawData;
class Renderer {
public:
    Renderer() = delete;
    Renderer(Device* device, SwapChain* swapChain, Scene* scene, Camera* camera);
    ~Renderer();

    void CreateCommandPools();

    void CreateRenderPass();

    void CreateCameraDescriptorSetLayout();
    void CreateModelDescriptorSetLayout();
    void CreateTimeDescriptorSetLayout();
    void CreateComputeDescriptorSetLayout();

    void CreateDescriptorPool();

    void CreateCameraDescriptorSet();
    void CreateModelDescriptorSets();
    void CreateGrassDescriptorSets();
    void CreateTimeDescriptorSet();
    void CreateComputeDescriptorSets();

    void CreateGraphicsPipeline();
    void CreateGrassPipeline();
    void CreateComputePipeline();

    void CreateFrameResources();
    void DestroyFrameResources();
    void RecreateFrameResources();
    void RecordComputeCommandBuffer();

    void RenderWithImGUI(ImDrawData* draw_data);

    void Frame();
    VkRenderPass renderPass;

private:
    Device* device;
    VkDevice logicalDevice;
    SwapChain* swapChain;
    Scene* scene;
    Camera* camera;

    VkCommandPool graphicsCommandPool;
    VkCommandPool computeCommandPool;

    VkFence Fence;

    VkDescriptorSetLayout cameraDescriptorSetLayout;
    VkDescriptorSetLayout modelDescriptorSetLayout;
    VkDescriptorSetLayout grassDescriptorSetLayout;
    VkDescriptorSetLayout timeDescriptorSetLayout;
    
    VkDescriptorPool descriptorPool;

    VkDescriptorSet cameraDescriptorSet;
    std::vector<VkDescriptorSet> modelDescriptorSets;
    std::vector<VkDescriptorSet> grassDescriptorSets;
    VkDescriptorSet timeDescriptorSet;

	VkDescriptorSetLayout computeDescriptorSetLayout;
	VkDescriptorSet computeDescriptorSet;

    VkPipelineLayout graphicsPipelineLayout;
    VkPipelineLayout grassPipelineLayout;
    VkPipelineLayout computePipelineLayout;

    VkPipeline graphicsPipeline;
    VkPipeline grassPipeline;
    VkPipeline computePipeline;

    std::vector<VkImageView> imageViews;
    VkImage depthImage;
    VkDeviceMemory depthImageMemory;
    VkImageView depthImageView;
    std::vector<VkFramebuffer> framebuffers;
    VkCommandBuffer graphicsCommandBuffer;
    VkCommandBuffer computeCommandBuffer;
};
