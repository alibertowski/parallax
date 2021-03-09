#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include "window.hpp"
#include "pxPhysicalDevices.hpp"
#include "layers.hpp"

namespace plxVulkan{
    class renderer{
    public:
        renderer(window* pWindow);

        void renderer_init();
        void cleanup();
    private:
        window* windowInstance;
        VkSurfaceKHR surface;

        VkInstance instance;
        layers layer;
        pxPhysicalDevices physicalDevices;
        VkDevice device;

        VkQueue graphicsQueue;
        VkQueue presentQueue;

        VkSwapchainKHR swapchain;
        std::vector<VkImage> swapchainImages;
        std::vector<VkImageView> swapchainImageViews;
        VkFormat swapchainFormat;
        VkExtent2D swapchainExtent;

        VkPipelineLayout pipelineLayout;
        VkRenderPass renderPass;
        VkPipeline graphicsPipeline;

        std::vector<VkFramebuffer> swapChainFramebuffers;
        VkCommandPool commandPool;
        std::vector<VkCommandBuffer> commandBuffers;

        std::vector<VkSemaphore> imageAvailableSemaphores;
        std::vector<VkSemaphore> renderFinishedSemaphores;
        std::vector<VkFence> inFlightFences;
        std::vector<VkFence> imagesInFlight;
        size_t currentFrame = 0;

        void createInstance();
        void DrawFrame();
        void initVulkan();
        void mainLoop();
        void CreateSurface();
        void pickPhysicalDevice();
        void createLogicalDevice();
        void SetupVulkanDebugger();
        void CreateSwapchain();
        void CreateImageViews();
        void CreateGraphicsPipeline();
        void CreateRenderPass();
        void CreateFrameBuffers();
        void CreateCommandPool();
        void CreateCommandBuffers();
        void CreateSyncObjects();
    };
}