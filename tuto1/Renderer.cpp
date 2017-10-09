#include "BUILD_OPTIONS.h"
#include "Platform.h"

#include "Renderer.h"
#include "Shared.h"
#include "VulkanWindow.h"

#include<cstdlib>
#include<assert.h>
#include <vector>
#include <iostream>
#include <sstream>

Renderer::Renderer()
{
#if BUILD_ENABLE_VULKAN_DEBUG
    setupDebug();
#endif
    initInstance();
#if BUILD_ENABLE_VULKAN_DEBUG
    initDebug();
#endif
    initDevice();
}

Renderer::~Renderer()
{
    unInitDevice();
#if BUILD_ENABLE_VULKAN_DEBUG
    unInitDebug();
#endif
    unInitInstance();
}

VulkanWindow * Renderer::CreateVulkanWindow(uint32_t size_x, uint32_t size_y, std::string name)
{
    m_window    = new VulkanWindow(size_x, size_y, name);
    return m_window;
}

bool Renderer::Run()
{
    if (m_window != nullptr) {
        return m_window->Update();
    }
    return false;
}

void Renderer::initInstance()
{
    // Aplication
    VkApplicationInfo								aplication_info{};
    aplication_info.sType							= VK_STRUCTURE_TYPE_APPLICATION_INFO;
    // App
    aplication_info.pApplicationName				= "vulkan_tutorial_1";
    aplication_info.applicationVersion				= VK_MAKE_VERSION(0, 1, 0);
    // API
    aplication_info.apiVersion						= VK_API_VERSION_1_0;

    // Instance
    VkInstanceCreateInfo							instance_create_info{};
    instance_create_info.sType						= VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    // App
    instance_create_info.pApplicationInfo			= &aplication_info;
    // Layers
    instance_create_info.enabledLayerCount			= m_instance_layer_list.size();
    instance_create_info.ppEnabledLayerNames		= m_instance_layer_list.data();
    //Extentions
    instance_create_info.enabledExtensionCount		= m_instance_extension_list.size();
    instance_create_info.ppEnabledExtensionNames	= m_instance_extension_list.data();
#if BUILD_ENABLE_VULKAN_DEBUG
    // Debug
    instance_create_info.pNext                      = &m_debug_report_callback_create_info;
#endif

#if BUILD_ENABLE_VULKAN_RUNTIME_DEBUG
    ErrorCheck(vkCreateInstance(&instance_create_info, nullptr, &m_instance));
#endif
}

void Renderer::unInitInstance()
{
    vkDestroyInstance(m_instance, nullptr);
    m_instance = nullptr;
}

void Renderer::initDevice()
{
    {
        uint32_t n_gpu_count = 0;
        vkEnumeratePhysicalDevices(m_instance, &n_gpu_count, nullptr);
        std::vector<VkPhysicalDevice> gpu_list(n_gpu_count);
        vkEnumeratePhysicalDevices(m_instance, &n_gpu_count, gpu_list.data());
        m_gpu = gpu_list[0];
    }
    {
        uint32_t n_family_count = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(m_gpu, &n_family_count, nullptr);
        std::vector<VkQueueFamilyProperties> family_property_list(n_family_count);
        vkGetPhysicalDeviceQueueFamilyProperties(m_gpu, &n_family_count, family_property_list.data());

        bool queue_supporting_graphics_found = false;
        for (uint32_t i_family = 0; i_family < n_family_count; i_family++)
        {
            if (family_property_list[i_family].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                queue_supporting_graphics_found = true;
                i_graphics_family_index = i_family;
            }
        }
        if (!queue_supporting_graphics_found) {
            assert(0 && "Vulkan ERROR: Queue family supporting graphics not found.");
            std::exit(-1);
        }
    }

    {
        uint32_t layer_count = 0;
        vkEnumerateInstanceLayerProperties(&layer_count, nullptr);
        std::vector<VkLayerProperties> layer_property_list(layer_count);
        vkEnumerateInstanceLayerProperties(&layer_count, layer_property_list.data());
        std::cout << "Instance Layers: \n";
        for (auto &i : layer_property_list)
        {
            std::cout << "    " << i.layerName << "\t\t |" << i.description << std::endl;
        }
        std::cout << std::endl;
    }
    {
        uint32_t layer_count = 0;
        vkEnumerateDeviceLayerProperties(m_gpu, &layer_count, nullptr);
        std::vector<VkLayerProperties> layer_property_list(layer_count);
        vkEnumerateDeviceLayerProperties(m_gpu, &layer_count, layer_property_list.data());
        std::cout << "Device Layers: \n";
        for (auto &i : layer_property_list)
        {
            std::cout << "    " << i.layerName << "\t\t |" << i.description << std::endl;
        }
        std::cout << std::endl;
    }

    float queue_priorities[]{ 1.0f };
    VkDeviceQueueCreateInfo                     device_queue_create_info{};
    device_queue_create_info.sType              = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    device_queue_create_info.queueFamilyIndex   = i_graphics_family_index;
    device_queue_create_info.queueCount			= 1;
    device_queue_create_info.pQueuePriorities   = queue_priorities;

    VkDeviceCreateInfo                          device_create_info{};
    device_create_info.sType                    = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    device_create_info.queueCreateInfoCount     = 1;
    device_create_info.pQueueCreateInfos        = &device_queue_create_info;

#if BUILD_ENABLE_VULKAN_RUNTIME_DEBUG
    ErrorCheck(vkCreateDevice(m_gpu, &device_create_info, nullptr, &m_device));
#endif

    vkGetDeviceQueue(m_device, i_graphics_family_index, 0, &m_queue);
}

void Renderer::unInitDevice()
{
    vkDestroyDevice(m_device, nullptr);
    m_device = nullptr;
}

#if BUILD_ENABLE_VULKAN_DEBUG

VKAPI_ATTR VkBool32 VKAPI_CALL
VulkanDebugCallback(
    VkDebugReportFlagsEXT       msg_flags,
    VkDebugReportObjectTypeEXT  obj_type,
    uint64_t                    src_obj,
    size_t                      location,
    int32_t                     msg_code,
    const char *                layer_prefix,
    const char *                msg,
    void *                      user_data
) {
    std::ostringstream stream;
    stream << "VKDBG | ";
    if (msg_flags & VK_DEBUG_REPORT_INFORMATION_BIT_EXT) {
        stream << "INFO       ";
    }
    if (msg_flags & VK_DEBUG_REPORT_WARNING_BIT_EXT) {
        stream << "WARNING /!\\";
    }
    if (msg_flags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT) {
        stream << "PERFORMANCE";
    }
    if (msg_flags & VK_DEBUG_REPORT_ERROR_BIT_EXT) {
        stream << "ERROR!!!!!!";
    }
    if (msg_flags & VK_DEBUG_REPORT_DEBUG_BIT_EXT) {
        stream << "DEBUG      ";
    }
    stream << "| @" << layer_prefix << "\t: ";
    stream << msg << std::endl;
    std::cout << stream.str();

#ifdef  _WIN32
    if (msg_flags & VK_DEBUG_REPORT_ERROR_BIT_EXT) {
        MessageBox(NULL, stream.str().c_str(), "Vulkan Error!", 0);
    }
#endif //  _WIN32

    return false;
}

void Renderer::setupDebug()
{
    //VkDebugReportCallbackCreateInfoEXT debug_report_callback_create_info {};
    m_debug_report_callback_create_info.sType					= VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
    m_debug_report_callback_create_info.pfnCallback			= VulkanDebugCallback;
    m_debug_report_callback_create_info.flags					=
        //VK_DEBUG_REPORT_INFORMATION_BIT_EXT         |
        //VK_DEBUG_REPORT_WARNING_BIT_EXT             |
        //VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT |
        //VK_DEBUG_REPORT_ERROR_BIT_EXT               |
        //VK_DEBUG_REPORT_DEBUG_BIT_EXT               |
        //VK_DEBUG_REPORT_FLAG_BITS_MAX_ENUM_EXT      |
        //0
        VK_DEBUG_REPORT_FLAG_BITS_MAX_ENUM_EXT  // Shortcut (Equivalent of the above.)
        ;

    m_instance_layer_list.push_back("VK_LAYER_LUNARG_standard_validation");
    //_instance_layer_list.push_back("VK_LAYER_GOOGLE_threading");
    //_instance_layer_list.push_back("VK_LAYER_LUNARG_draw_state");
    //_instance_layer_list.push_back("VK_LAYER_LUNARG_image");
    //_instance_layer_list.push_back("VK_LAYER_LUNARG_mem_tracker");
    //_instance_layer_list.push_back("VK_LAYER_LUNARG_object_tracker");
    //_instance_layer_list.push_back("VK_LAYER_LUNARG_param_checker");

    m_instance_extension_list.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
}

PFN_vkCreateDebugReportCallbackEXT		fvkCreateDebugReportCallbackEXT			= nullptr;
PFN_vkDestroyDebugReportCallbackEXT		fvkDestroyDebugReportCallbackEXT		= nullptr;

void Renderer::initDebug()
{
    fvkCreateDebugReportCallbackEXT = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(m_instance, "vkCreateDebugReportCallbackEXT");
    if (fvkCreateDebugReportCallbackEXT == nullptr) {
        assert(0 && "Vulkan ERROR: Can't fetch debug function pointers.");
        std::exit(-1);
    }
    fvkDestroyDebugReportCallbackEXT = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(m_instance, "vkDestroyDebugReportCallbackEXT");
    if (fvkDestroyDebugReportCallbackEXT == nullptr) {
        assert(0 && "Vulkan ERROR: Can't fetch debug function pointers.");
        std::exit(-1);
    }

    fvkCreateDebugReportCallbackEXT(m_instance, &m_debug_report_callback_create_info, nullptr, &m_debug_report);
    //vkCreateDebugReportCallbackEXT(_instance, nullptr, nullptr, nullptr);
}

void Renderer::unInitDebug()
{
    fvkDestroyDebugReportCallbackEXT(m_instance, m_debug_report, nullptr);
    m_debug_report = VK_NULL_HANDLE;
}

#endif