#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include "window.hpp"
#include "pxPhysicalDevices.hpp"

#ifndef NDEBUG
#include "layers.hpp"
#endif

namespace parallax_vulkan{
    class renderer{
    public:
        renderer() = default;
        renderer(Window* p_window);

        void renderer_init();
        void clean_up();
    private:
        #ifndef NDEBUG
        Layers layer_;

        void setup_vulkan_debugger();
        #endif

        VkInstance instance_;
        Window* window_;
        VkSurfaceKHR surface;
        
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

        void init_vulkan();
        void create_instance();
        void draw_frame();
        void main_loop();
        void CreateSurface();
        void pick_physical_device();
        void createLogicalDevice();
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
