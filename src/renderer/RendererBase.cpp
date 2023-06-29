#include "renderer/RendererBase.hpp"

#include "Log.h"
#include "Window.hpp"

#include <sstream>
#include <iomanip>

namespace eng
{
#if ENABLE_VALIDATION
    const std::vector<const char *> RendererBase::ValidationLayers = {"VK_LAYER_KHRONOS_validation"};

    VKAPI_ATTR VkBool32 VKAPI_CALL CallbackDebug(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageTypes,
                                                 const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, void *pUserData)
    {
        if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
            Log::Warning(pCallbackData->pMessage, '\n');
        else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT)
            Log::Info(pCallbackData->pMessage, '\n');
        else // VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT
            Log::Error(pCallbackData->pMessage, '\n');

        return VK_FALSE;
    }

    VkResult RendererBase::FvkCreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
                                                           const VkAllocationCallbacks *pAllocator, VkDebugUtilsMessengerEXT *pDebugMessenger)
    {
        auto func = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT"));
        if (func)
            return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }

    void RendererBase::FvkDestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT messenger, const VkAllocationCallbacks *pAllocator)
    {
        auto func = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT"));
        if (func)
            return func(instance, messenger, pAllocator);
    }
#else
    const std::vector<const char *> Instance::ValidationLayers = {"VK_LAYER_LUNARG_standard_validation"};

    VKAPI_ATTR VkBool32 VKAPI_CALL CallbackDebug(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType, uint64_t object, size_t location, int32_t messageCode,
                                                 const char *pLayerPrefix, const char *pMessage, void *pUserData)
    {
        if (flags & VK_DEBUG_REPORT_WARNING_BIT_EXT)
            Log::Warning(pMessage, '\n');
        else if (flags & VK_DEBUG_REPORT_INFORMATION_BIT_EXT)
            Log::Info(pMessage, '\n');
        else //  VK_DEBUG_REPORT_ERROR_BIT_EXT
            Log::Error(pMessage, '\n');

        return VK_FALSE;
    }

    VkResult Instance::FvkCreateDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT *pCreateInfo, const VkAllocationCallbacks *pAllocator,
                                                       VkDebugReportCallbackEXT *pCallback)
    {
        auto func = reinterpret_cast<PFN_vkCreateDebugReportCallbackEXT>(vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT"));
        if (func)
            return func(instance, pCreateInfo, pAllocator, pCallback);
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }

    void Instance::FvkDestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks *pAllocator)
    {
        auto func = reinterpret_cast<PFN_vkDestroyDebugReportCallbackEXT>(vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT"));
        if (func)
            func(instance, callback, pAllocator);
    }
#endif

    RendererBase::RendererBase()
    {
#if defined(ENABLE_VALIDATION)
        settings.validation = true;
#endif

        Log::Info("Initialized RendererBase class.\n");
    }

    RendererBase::~RendererBase()
    {
    }

    void RendererBase::Init()
    {
        CreateInstance();
    }

    void RendererBase::CreateInstance()
    {
        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = name.c_str();
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = name.c_str();
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = apiVersion;

        std::vector<const char *> instanceExtensions = GetExtensions();

        if (settings.validation && !CheckValidationLayerSupport())
        {
            Log::Error("Validation layers requested, but not available!\n");
            settings.validation = false;
        }

        VkInstanceCreateInfo instanceCreateInfo = {};
        instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        instanceCreateInfo.pApplicationInfo = &appInfo;
        instanceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(instanceExtensions.size());
        instanceCreateInfo.ppEnabledExtensionNames = instanceExtensions.data();
#if __APPLE__
        instanceCreateInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
#endif

        if (settings.validation)
        {
            VkDebugUtilsMessengerCreateInfoEXT debugUtilsMessengerCreateInfo = {};
            debugUtilsMessengerCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
            debugUtilsMessengerCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                                                            VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
            debugUtilsMessengerCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                                                        VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
            debugUtilsMessengerCreateInfo.pfnUserCallback = &CallbackDebug;
            instanceCreateInfo.pNext = static_cast<VkDebugUtilsMessengerCreateInfoEXT *>(&debugUtilsMessengerCreateInfo);

            instanceCreateInfo.enabledLayerCount = static_cast<uint32_t>(ValidationLayers.size());
            instanceCreateInfo.ppEnabledLayerNames = ValidationLayers.data();
        }
        Core::CheckVk(vkCreateInstance(&instanceCreateInfo, nullptr, &instance));
    }

    std::vector<const char *> RendererBase::GetExtensions() const
    {
        auto [glfwExtensions, glfwExtensionsCount] = Window::GetInstanceExtensions();

        std::vector<const char *> extensions(glfwExtensions, glfwExtensions + glfwExtensionsCount);
        for (uint32_t i = 0; i < glfwExtensionsCount; i++)
        {
            extensions.emplace_back(glfwExtensions[i]);
        }

#ifdef _WIN32
        // Windows specific extensions
        extensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
#elif __APPLE__
        // MacOS specific extensions
        extensions.emplace_back(VK_KHR_SURFACE_EXTENSION_NAME);
        extensions.emplace_back("VK_EXT_metal_surface");
        extensions.emplace_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
#endif

        if (settings.validation)
            extensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        return extensions;
    }

    bool RendererBase::CheckValidationLayerSupport() const
    {
        uint32_t instanceLayerPropertyCount;
        vkEnumerateInstanceLayerProperties(&instanceLayerPropertyCount, nullptr);
        std::vector<VkLayerProperties> instanceLayerProperties(instanceLayerPropertyCount);
        vkEnumerateInstanceLayerProperties(&instanceLayerPropertyCount, instanceLayerProperties.data());

#ifdef DEBUG
        LogVulkanLayers(instanceLayerProperties);
#endif

        for (const auto &layerName : ValidationLayers)
        {
            bool layerFound = false;

            for (const auto &layerProperties : instanceLayerProperties)
            {
                if (strcmp(layerName, layerProperties.layerName) == 0)
                {
                    layerFound = true;
                    break;
                }
            }

            if (!layerFound)
            {
                Log::Error("Vulkan validation layer not found: ", std::quoted(layerName), '\n');
                return false;
            }
        }

        return true;
    }

    void RendererBase::LogVulkanLayers(const std::vector<VkLayerProperties> &layerProperties)
    {
        std::stringstream ss;

        ss << "Instance Layers: ";
        for (const auto &layer : layerProperties)
            ss << layer.layerName << ", ";

        ss << "\n\n";
        Log::Out(ss.str());
    }
}
