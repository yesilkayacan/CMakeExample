#include "renderer/Core.hpp"

#include <vulkan/vk_enum_string_helper.h>

namespace eng
{
    std::string Core::StringifyResultVk(VkResult result)
    {
        return std::string(string_VkResult(result));
    }

    void Core::CheckVk(VkResult result)
    {
        if (result >= 0)
            return;

        auto failure = StringifyResultVk(result);

        throw std::runtime_error("Vulkan error: " + failure);
    }
}