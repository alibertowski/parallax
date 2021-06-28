#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>
#include <stdexcept>
#include <vector>
#include <iostream>
#include <set>
#include <fstream>

#include "vulkan/renderer.hpp"
#include "window.hpp"
#include "settings.hpp"
#include "vulkan/pxPhysicalDevices.hpp"
#include "vulkan/extensions.hpp"

#ifndef NDEBUG
#include "vulkan/layers.hpp"
#endif

constexpr int MAX_FRAMES_IN_FLIGHT{ 2 };

parallax_vulkan::renderer::renderer(Window* p_window){
    window_ = p_window;
}

void parallax_vulkan::renderer::renderer_init() {
    init_vulkan();
    main_loop();
}

void parallax_vulkan::renderer::init_vulkan() {
    create_instance();

    #ifndef NDEBUG
    setup_vulkan_debugger();
    #endif

    CreateSurface();
    pick_physical_device();
    createLogicalDevice();
    CreateSwapchain();
    CreateImageViews();
    CreateRenderPass();
    CreateGraphicsPipeline();
    CreateFrameBuffers();
    CreateCommandPool();
    CreateCommandBuffers();
    CreateSyncObjects();
}

void parallax_vulkan::renderer::clean_up(){
    std::cout << "Cleaning Vulkan renderer...\n";

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        vkDestroySemaphore(device, renderFinishedSemaphores[i], NULL);
        vkDestroySemaphore(device, imageAvailableSemaphores[i], NULL);
        vkDestroyFence(device, inFlightFences[i], NULL);
    }

    vkDestroyCommandPool(device, commandPool, NULL);
    for (auto framebuffer : swapChainFramebuffers) {
        vkDestroyFramebuffer(device, framebuffer, NULL);
    }

    vkDestroyPipeline(device, graphicsPipeline, NULL);
    vkDestroyPipelineLayout(device, pipelineLayout, NULL);
    vkDestroyRenderPass(device, renderPass, NULL);

    for(const auto& imageView : swapchainImageViews){
        vkDestroyImageView(device, imageView, NULL);
    }

    vkDestroySwapchainKHR(device, swapchain, NULL);
    vkDestroyDevice(device, NULL);

    #ifndef NDEBUG
        layer_.clean_up();
    #endif

    vkDestroySurfaceKHR(instance_, surface, NULL);
    vkDestroyInstance(instance_, NULL);
}

#ifndef NDEBUG
void parallax_vulkan::renderer::setup_vulkan_debugger(){    
    layer_ = layers{ &instance_ };
    layer_.layers_init();
}
#endif

void parallax_vulkan::renderer::create_instance() {
    uint32_t apiVersion;
    vkEnumerateInstanceVersion(&apiVersion);

    auto required_extensions{ extensions::get_required_instance_extensions() };

    VkApplicationInfo app_info{};
    app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.pNext = NULL;
    app_info.pApplicationName = settings::ENGINE_NAME; // TOOD: Create a real application name
    app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    app_info.pEngineName = settings::ENGINE_NAME;
    app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    app_info.apiVersion = apiVersion;

    VkInstanceCreateInfo create_info{};
    create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    create_info.flags = 0;
    create_info.pApplicationInfo = &app_info;

    #ifndef NDEBUG
        auto required_layers{ layers::get_layers() };
        VkDebugUtilsMessengerCreateInfoEXT debug_create_info{ layers::populate_debugger_messenger_create_info() };
        create_info.enabledLayerCount = static_cast<uint32_t>(required_layers.size());
        create_info.ppEnabledLayerNames = required_layers.data();
        create_info.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debug_create_info;
    #else
        create_info.enabledLayerCount = 0;
        create_info.ppEnabledLayerNames = NULL;
        create_info.pNext = NULL;
    #endif

    create_info.enabledExtensionCount = static_cast<uint32_t>(required_extensions.size());
    create_info.ppEnabledExtensionNames = required_extensions.data();

    if (!vkCreateInstance(&create_info, NULL, &instance_) != VK_SUCCESS) { // TODO: Check out the pAllocator eventually
        std::runtime_error{ "Failed to create Vulkan instance" };
    }
}

void parallax_vulkan::renderer::main_loop(){
    std::cout << "Loop started\n";

    while (!glfwWindowShouldClose(window_->get_window())) {
        glfwPollEvents();
        draw_frame();
    }
}

void parallax_vulkan::renderer::CreateSurface(){
    if(glfwCreateWindowSurface(instance_, window_->get_window(), NULL, &surface) != VK_SUCCESS){
        std::runtime_error{ "Failed to create surface" };
    }
}

void parallax_vulkan::renderer::pick_physical_device(){
    physicalDevices = pxPhysicalDevices{ &instance_ };
    std::vector<pxPhysicalDevice>& devices { physicalDevices.GetDevices() };

    for(pxPhysicalDevice& device : devices){ 
        uint32_t queueFamilycount { 0 };
        vkGetPhysicalDeviceQueueFamilyProperties2(device.GetPhysicalDevice(), &queueFamilycount, NULL);
        
        std::vector<VkQueueFamilyProperties2> queueFamilyProperties { queueFamilycount };
        for(auto& queue: queueFamilyProperties){
            queue.sType = VK_STRUCTURE_TYPE_QUEUE_FAMILY_PROPERTIES_2;
        }

        uint32_t index{ 0 };
        vkGetPhysicalDeviceQueueFamilyProperties2(device.GetPhysicalDevice(), &queueFamilycount, queueFamilyProperties.data());
        for (const auto& queueFamily : queueFamilyProperties){
            if (queueFamily.queueFamilyProperties.queueFlags & VK_QUEUE_GRAPHICS_BIT){
                device.GetQueueFamilyIndices().graphicFamilyIndex = index;
            }

            VkBool32 presentationSupport{ VK_FALSE };
            vkGetPhysicalDeviceSurfaceSupportKHR(device.GetPhysicalDevice(), index, surface, &presentationSupport);
            if(presentationSupport == VK_TRUE){
                device.GetQueueFamilyIndices().presentFamilyIndex = index;
            }

            if(device.IsValid()){
                physicalDevices.SetPrimaryDevice(device);
                break;
            }
            ++index;
        }

        if(device.IsValid()) break;
    }

    auto primaryDevice{ physicalDevices.GetPrimaryDevice() };
    if (!primaryDevice.IsValid()) {
        throw std::runtime_error{ "Could not find a suitable GPU" };
    }
}

void parallax_vulkan::renderer::createLogicalDevice(){
    float queuePriority{ 1.0F };
    auto primaryDevice{ physicalDevices.GetPrimaryDevice() };
    auto deviceExtensions{ extensions::get_required_device_extensions() };
    std::vector<VkDeviceQueueCreateInfo> queueFamilies;
    std::set<uint32_t> uniqueFamilies{ primaryDevice.GetQueueFamilyIndices().graphicFamilyIndex.value(), primaryDevice.GetQueueFamilyIndices().presentFamilyIndex.value() };
    for(uint32_t queueIndex : uniqueFamilies){
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.pNext = NULL;
        queueCreateInfo.queueFamilyIndex = queueIndex;
        queueCreateInfo.queueCount = 1; // Possiby TODO
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueFamilies.push_back(queueCreateInfo);
    }

    VkPhysicalDeviceFeatures physicalDeviceFeatures{}; // Possibly TODO: Just enable stuff once engine is done
    VkDeviceCreateInfo deviceCreateInfo{};
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.pNext = NULL;
    deviceCreateInfo.flags = 0;
    deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(queueFamilies.size());
    deviceCreateInfo.pQueueCreateInfos = queueFamilies.data();
    deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
    deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();
    deviceCreateInfo.pEnabledFeatures = &physicalDeviceFeatures;

    if(vkCreateDevice(physicalDevices.GetPrimaryDevice().GetPhysicalDevice(), &deviceCreateInfo, NULL, &device) != VK_SUCCESS) {
        std::runtime_error{ "Logical device was unable to be created" };
    }

    VkDeviceQueueInfo2 graphicQueueInfo{};
    graphicQueueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_INFO_2;
    graphicQueueInfo.pNext = NULL;
    graphicQueueInfo.queueFamilyIndex = primaryDevice.GetQueueFamilyIndices().graphicFamilyIndex.value();
    graphicQueueInfo.queueIndex = 0; // Possibly TODO.

    VkDeviceQueueInfo2 presentQueueInfo{};
    presentQueueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_INFO_2;
    presentQueueInfo.pNext = NULL;
    presentQueueInfo.queueFamilyIndex = primaryDevice.GetQueueFamilyIndices().presentFamilyIndex.value();
    presentQueueInfo.queueIndex = 0; // Possibly TODO.

    vkGetDeviceQueue2(device, &graphicQueueInfo, &graphicsQueue);
    vkGetDeviceQueue2(device, &presentQueueInfo, &presentQueue);
}

void parallax_vulkan::renderer::CreateSwapchain(){
    auto primaryDevice{ physicalDevices.GetPrimaryDevice() };
    VkSurfaceCapabilitiesKHR capabilities;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(primaryDevice.GetPhysicalDevice(), surface, &capabilities);

    VkSurfaceFormatKHR format;
    uint32_t surfaceFormatCount{ 0 };
    vkGetPhysicalDeviceSurfaceFormatsKHR(primaryDevice.GetPhysicalDevice(), surface, &surfaceFormatCount, NULL);
    std::vector<VkSurfaceFormatKHR> surfaceFormats{ surfaceFormatCount };
    vkGetPhysicalDeviceSurfaceFormatsKHR(primaryDevice.GetPhysicalDevice(), surface, &surfaceFormatCount, surfaceFormats.data());
    bool selected{ false };
    for (const auto& availableFormat : surfaceFormats) {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            format = availableFormat;
            selected = true;
        }
    }
    if(!selected){
        format = surfaceFormats[0];
    }

    VkPresentModeKHR presentMode;
    uint32_t presentModeCount{ 0 };
    vkGetPhysicalDeviceSurfacePresentModesKHR(primaryDevice.GetPhysicalDevice(), surface, &presentModeCount, NULL);
    std::vector<VkPresentModeKHR> presentModes{ presentModeCount };
    vkGetPhysicalDeviceSurfacePresentModesKHR(primaryDevice.GetPhysicalDevice(), surface, &presentModeCount, presentModes.data());
    for(const auto& mode : presentModes){
        if(mode == VK_PRESENT_MODE_IMMEDIATE_KHR){
            presentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
            break;
        }

        presentMode = VK_PRESENT_MODE_FIFO_KHR;
    }

    queueFamilyIndices indices{ primaryDevice.GetQueueFamilyIndices() };
    uint32_t queueFamilyIndices[] = {indices.graphicFamilyIndex.value(), indices.presentFamilyIndex.value()};
    VkSwapchainCreateInfoKHR swapCreateInfo{};
    swapCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapCreateInfo.pNext = NULL;
    swapCreateInfo.flags = 0;
    swapCreateInfo.surface = surface;
    swapCreateInfo.minImageCount = capabilities.minImageCount; // TODO: Temporary solutioni
    swapCreateInfo.imageFormat = format.format;
    swapCreateInfo.imageColorSpace = format.colorSpace;
    swapCreateInfo.imageExtent = capabilities.currentExtent; // TODO: Temporary Solution
    swapCreateInfo.imageArrayLayers = 1;
    swapCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT; // TODO: Look into when the main engine is finisehd
    if (indices.graphicFamilyIndex.value() != indices.presentFamilyIndex.value()) {
        swapCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        swapCreateInfo.queueFamilyIndexCount = 2;
        swapCreateInfo.pQueueFamilyIndices = queueFamilyIndices;
    } else {
        swapCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    }

    swapCreateInfo.preTransform = capabilities.currentTransform;
    swapCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR; // TODO: look into
    swapCreateInfo.presentMode = presentMode;
    swapCreateInfo.clipped = VK_TRUE;
    swapCreateInfo.oldSwapchain = VK_NULL_HANDLE;

    if(vkCreateSwapchainKHR(device,  &swapCreateInfo, NULL, &swapchain) != VK_SUCCESS){
        throw std::runtime_error{ "Swapchain failed to be created" };
    }

    uint32_t swapchainImageCount{ 0 };
    vkGetSwapchainImagesKHR(device, swapchain, &swapchainImageCount, NULL);
    swapchainImages.resize(static_cast<size_t>(swapchainImageCount));
    vkGetSwapchainImagesKHR(device, swapchain, &swapchainImageCount, swapchainImages.data());

    swapchainFormat = format.format;
    swapchainExtent = capabilities.currentExtent;
}

void parallax_vulkan::renderer::CreateImageViews(){
    swapchainImageViews.resize(swapchainImages.size());
    for(size_t i = 0; i < swapchainImages.size(); ++i) { // TODO: Just check this thing when the engine is near finished.
        VkImageViewCreateInfo imgViewCreateInfo{};
        imgViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        imgViewCreateInfo.pNext = NULL;
        imgViewCreateInfo.image = swapchainImages[i];
        imgViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        imgViewCreateInfo.format = swapchainFormat;
        imgViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        imgViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        imgViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        imgViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        imgViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        imgViewCreateInfo.subresourceRange.baseMipLevel = 0;
        imgViewCreateInfo.subresourceRange.levelCount = 1;
        imgViewCreateInfo.subresourceRange.baseArrayLayer = 0;
        imgViewCreateInfo.subresourceRange.layerCount = 1;

        if(vkCreateImageView(device, &imgViewCreateInfo, NULL, &swapchainImageViews[i]) != VK_SUCCESS){
            throw std::runtime_error{ "Failed to create image views" };
        }
    }
}

std::vector<char> ReadShader(const std::string& filePath){
    std::ifstream file{ filePath, std::ios_base::ate | std::ios_base::binary };
    if (!file.is_open()) {
        throw std::runtime_error("failed to open file!");
    }

    std::streampos fileSize{ file.tellg() };
    std::vector<char> buffer;
    buffer.resize(static_cast<std::size_t>(fileSize));

    file.seekg(0);
    file.read(buffer.data(), fileSize) ;
    file.close();

    return buffer;
}

void parallax_vulkan::renderer::CreateGraphicsPipeline(){
    auto vertShaderBinary{ ReadShader("shaders/shaders.vert.spv") };
    auto fragShaderBinary{ ReadShader("shaders/shaders.frag.spv") };

    VkShaderModule vertShaderModule;
    VkShaderModuleCreateInfo vertCreateInfo{};
    vertCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    vertCreateInfo.pNext = NULL;
    vertCreateInfo.flags = 0;
    vertCreateInfo.codeSize = vertShaderBinary.size();
    vertCreateInfo.pCode = reinterpret_cast<const uint32_t*>(vertShaderBinary.data());

    vkCreateShaderModule(device, &vertCreateInfo, NULL, &vertShaderModule);

    VkShaderModule fragShaderModule;
    VkShaderModuleCreateInfo fragCreateInfo{};
    fragCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    fragCreateInfo.pNext = NULL;
    fragCreateInfo.flags = 0;
    fragCreateInfo.codeSize = fragShaderBinary.size();
    fragCreateInfo.pCode = reinterpret_cast<const uint32_t*>(fragShaderBinary.data());

    vkCreateShaderModule(device, &fragCreateInfo, NULL, &fragShaderModule);

    VkPipelineLayoutCreateInfo plCreateInfo{};
    plCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    plCreateInfo.pNext = NULL;
    plCreateInfo.flags = 0;
    plCreateInfo.setLayoutCount = 0;
    plCreateInfo.pushConstantRangeCount = 0;

    if(vkCreatePipelineLayout(device, &plCreateInfo, NULL, &pipelineLayout) != VK_SUCCESS){
        throw std::runtime_error{ "Failed to create pipeline layout" };
    }

    VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
    vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertShaderStageInfo.module = vertShaderModule;
    vertShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
    fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragShaderStageInfo.module = fragShaderModule;
    fragShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo, fragShaderStageInfo};

    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount = 0;
    vertexInputInfo.vertexAttributeDescriptionCount = 0;

    VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssembly.primitiveRestartEnable = VK_FALSE;

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(swapchainExtent.width);
    viewport.height = static_cast<float>(swapchainExtent.height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    VkRect2D scissor{};
    scissor.offset = {0, 0};
    scissor.extent = swapchainExtent;

    VkPipelineViewportStateCreateInfo viewportState{};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.pViewports = &viewport;
    viewportState.scissorCount = 1;
    viewportState.pScissors = &scissor;

    VkPipelineRasterizationStateCreateInfo rasterizer{};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
    rasterizer.depthBiasEnable = VK_FALSE;

    VkPipelineMultisampleStateCreateInfo multisampling{};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

    VkPipelineColorBlendAttachmentState colorBlendAttachment{};
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_FALSE;

    VkPipelineColorBlendStateCreateInfo colorBlending{};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.logicOp = VK_LOGIC_OP_COPY;
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;
    colorBlending.blendConstants[0] = 0.0f;
    colorBlending.blendConstants[1] = 0.0f;
    colorBlending.blendConstants[2] = 0.0f;
    colorBlending.blendConstants[3] = 0.0f;

    VkGraphicsPipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = shaderStages;
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampling;
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.layout = pipelineLayout;
    pipelineInfo.renderPass = renderPass;
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

    if (vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, NULL, &graphicsPipeline) != VK_SUCCESS) {
        throw std::runtime_error{ "failed to create graphics pipeline!" };
    }

    vkDestroyShaderModule(device, vertShaderModule, NULL);
    vkDestroyShaderModule(device, fragShaderModule, NULL);
}

void parallax_vulkan::renderer::CreateRenderPass(){
    VkAttachmentDescription attachmentDesc{};
    attachmentDesc.format = swapchainFormat;
    attachmentDesc.samples = VK_SAMPLE_COUNT_1_BIT;
    attachmentDesc.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    attachmentDesc.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    attachmentDesc.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attachmentDesc.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attachmentDesc.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    attachmentDesc.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference colorAttachmentRef{};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;

    VkRenderPassCreateInfo rsCreateInfo{};
    rsCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    rsCreateInfo.pNext = NULL;
    rsCreateInfo.attachmentCount = 1;
    rsCreateInfo.pAttachments = &attachmentDesc;
    rsCreateInfo.subpassCount = 1;
    rsCreateInfo.pSubpasses = &subpass;

    if(vkCreateRenderPass(device, &rsCreateInfo, NULL, &renderPass) != VK_SUCCESS){
        throw std::runtime_error{ "Failed to create render pass" };
    }
}

void parallax_vulkan::renderer::CreateFrameBuffers(){
    swapChainFramebuffers.resize(swapchainImageViews.size());

    for (size_t i = 0; i < swapchainImageViews.size(); i++) {
        VkImageView attachments[] = {
            swapchainImageViews[i]
        };

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = renderPass;
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments = attachments;
        framebufferInfo.width = swapchainExtent.width;
        framebufferInfo.height = swapchainExtent.height;
        framebufferInfo.layers = 1;

        if (vkCreateFramebuffer(device, &framebufferInfo, NULL, &swapChainFramebuffers[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create framebuffer!");
        }
    }
}

void parallax_vulkan::renderer::CreateCommandPool(){
    auto primaryDevice{ physicalDevices.GetPrimaryDevice() };

    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.pNext = NULL;
    poolInfo.queueFamilyIndex = primaryDevice.GetQueueFamilyIndices().graphicFamilyIndex.value();

    if (vkCreateCommandPool(device, &poolInfo, NULL, &commandPool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create command pool!");
    }
}

void parallax_vulkan::renderer::CreateCommandBuffers(){
    commandBuffers.resize(swapChainFramebuffers.size());

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.pNext = NULL;
    allocInfo.commandPool = commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = (uint32_t) commandBuffers.size();

    if (vkAllocateCommandBuffers(device, &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate command buffers!");
    }

    for (size_t i = 0; i < commandBuffers.size(); i++) {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.pNext = NULL;

        if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS) {
            throw std::runtime_error("failed to begin recording command buffer!");
        }

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = renderPass;
        renderPassInfo.framebuffer = swapChainFramebuffers[i];
        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = swapchainExtent;

        VkClearValue clearColor = {0.0f, 0.0f, 0.0f, 1.0f};
        renderPassInfo.clearValueCount = 1;
        renderPassInfo.pClearValues = &clearColor;

        vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);

        vkCmdDraw(commandBuffers[i], 3, 1, 0, 0);

        vkCmdEndRenderPass(commandBuffers[i]);

        if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to record command buffer!");
        }
    }
}

void parallax_vulkan::renderer::CreateSyncObjects(){
   imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
    imagesInFlight.resize(swapchainImages.size(), VK_NULL_HANDLE);

    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS ||
            vkCreateSemaphore(device, &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS ||
            vkCreateFence(device, &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create synchronization objects for a frame!");
        }
    } 
}

void parallax_vulkan::renderer::draw_frame(){
    vkWaitForFences(device, 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

    uint32_t imageIndex;
    vkAcquireNextImageKHR(device, swapchain, UINT64_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

    if (imagesInFlight[imageIndex] != VK_NULL_HANDLE) {
        vkWaitForFences(device, 1, &imagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);
    }
    imagesInFlight[imageIndex] = inFlightFences[currentFrame];

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = {imageAvailableSemaphores[currentFrame]};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffers[imageIndex];

    VkSemaphore signalSemaphores[] = {renderFinishedSemaphores[currentFrame]};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    vkResetFences(device, 1, &inFlightFences[currentFrame]);

    if (vkQueueSubmit(graphicsQueue, 1, &submitInfo, inFlightFences[currentFrame]) != VK_SUCCESS) {
        throw std::runtime_error("failed to submit draw command buffer!");
    }

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = {swapchain};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;

    presentInfo.pImageIndices = &imageIndex;

    vkQueuePresentKHR(presentQueue, &presentInfo);

    currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}
