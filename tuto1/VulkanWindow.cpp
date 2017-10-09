#include "VulkanWindow.h"

#include <assert.h>

VulkanWindow::VulkanWindow(uint32_t size_x, uint32_t size_y, std::string name) :
    m_surface_size_x(size_x),
    m_surface_size_y(size_y),
    m_window_name(name)
{
    initOsWindow();
}


VulkanWindow::~VulkanWindow()
{
    unInitOsWindow();
}

void VulkanWindow::Close()
{
    b_window_should_keep_running = false;
}
bool VulkanWindow::Update()
{
    updateOsWindow();
    return b_window_should_keep_running;
}
//
//void VulkanWindow::initOsWondow()
//{
//    m_win32_instance = GetModuleHandle(nullptr);
//    m_win32_class_name      = m_win
//}
