#pragma once

#include "vulkan/vulkan.h"
#include "vulkan/vulkan_core.h"

#include <string>

#define ENABLE_VALIDATION true
#define DEBUG

namespace eng
{
    class Core
    {
    public:
        Core();
        ~Core();

        static std::string StringifyResultVk(VkResult result);
        static void CheckVk(VkResult result);
    };
}