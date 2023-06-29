#pragma once

#include "Core.hpp"

#include <vector>

namespace eng
{

    class RendererBase
    {
    public:
        RendererBase();
        ~RendererBase();

        void Init();

        const std::string title = "Vulkan Project";
        const std::string name = "Vulkan Project";
        const uint32_t apiVersion = VK_API_VERSION_1_3;

    private:
        void CreateInstance();
        void CreatePhysicalDevice();
        void CreateLogicalDevice();

        std::vector<const char *> GetExtensions() const;
        bool CheckValidationLayerSupport() const;
        static void LogVulkanLayers(const std::vector<VkLayerProperties> &layerProperties);

#if ENABLE_VALIDATION
        static VkResult FvkCreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo, const VkAllocationCallbacks *pAllocator,
                                                        VkDebugUtilsMessengerEXT *pDebugMessenger);
        static void FvkDestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT messenger, const VkAllocationCallbacks *pAllocator);
#else
        static VkResult FvkCreateDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT *pCreateInfo, const VkAllocationCallbacks *pAllocator,
                                                        VkDebugReportCallbackEXT *pCallback);
        static void FvkDestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks *pAllocator);
#endif

        static const std::vector<const char *> ValidationLayers;

        struct Settings
        {
            bool validation = false;
            bool vsync = false;
        } settings;

    protected:
        VkInstance instance = VK_NULL_HANDLE;
    };
}
