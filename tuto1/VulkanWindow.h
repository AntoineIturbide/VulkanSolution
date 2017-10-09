#pragma once

#include "BUILD_OPTIONS.h"
#include "Platform.h"
#include <string>

class VulkanWindow
{
public:
    VulkanWindow(uint32_t size_x, uint32_t size_y, std::string name);
    ~VulkanWindow();

    void Close();

    bool Update();

private:

    void initOsWindow();
    void unInitOsWindow();
    void updateOsWindow();
    void initOsSurface();

    uint32_t        m_surface_size_x                    = 640;
    uint32_t        m_surface_size_y                    = 360;
    std::string     m_window_name;

    bool            b_window_should_keep_running        = true;


#if VK_USE_PLATFORM_WIN32_KHR // Windows OS

    HINSTANCE       m_win32_instance     = NULL;
    HWND            m_win32_window       = NULL;
    std::string     m_win32_class_name;
    static uint64_t m_win32_class_id_counter;

#endif
};

