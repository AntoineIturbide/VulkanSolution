#pragma once
#include "BUILD_OPTIONS.h"

#include "Platform.h"

#include <vector>

class VulkanWindow;

class Renderer
{
public:
	Renderer();
	~Renderer();

    VulkanWindow* CreateVulkanWindow(uint32_t size_x, uint32_t size_y, std::string name);

    bool    Run();

//private:
	void    initInstance();
	void    unInitInstance();

	void    initDevice();
	void    unInitDevice();

#if BUILD_ENABLE_VULKAN_RUNTIME_DEBUG
	void    setupDebug();
	void    initDebug();
	void    unInitDebug();
#endif

	VkInstance					m_instance                   = VK_NULL_HANDLE;
    VkPhysicalDevice			m_gpu                        = VK_NULL_HANDLE;
    VkDevice					m_device                     = VK_NULL_HANDLE;
    VkQueue                     m_queue                      = VK_NULL_HANDLE;
    uint32_t					i_graphics_family_index;

    VulkanWindow*                     m_window;

	std::vector<const char*>	m_instance_layer_list;
	std::vector<const char*>	m_instance_extension_list;
	//							device layer where deprecated since SDK 1.0.13.0
	std::vector<const char*>	m_device_extension_list;

#if BUILD_ENABLE_VULKAN_RUNTIME_DEBUG
	VkDebugReportCallbackEXT	m_debug_report			    = VK_NULL_HANDLE;

    VkDebugReportCallbackCreateInfoEXT m_debug_report_callback_create_info = {};
#endif
};