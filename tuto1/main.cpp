#include "Renderer.h"

int main() {

    Renderer r;

    r.CreateVulkanWindow(640, 360, "Vulkan Window");

    while (r.Run())
    {

    }

#if false
    VkDevice    device  = r.m_device;
    VkQueue     queue   = r.m_queue;

    VkFence     fence;

    VkFenceCreateInfo fence_create_info {};
    fence_create_info.sType                         = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    //fence_create_info.flags             = VK_FENCE_CREATE_SIGNALED_BIT;
    vkCreateFence(device, &fence_create_info, nullptr, &fence);

    VkSemaphore semaphore;
    VkSemaphoreCreateInfo semaphore_create_info {};
    semaphore_create_info.sType                     = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    vkCreateSemaphore(device, &semaphore_create_info, nullptr, &semaphore);

    VkCommandPool command_pool;

    VkCommandPoolCreateInfo command_pool_create_info {};
    command_pool_create_info.sType                  = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    command_pool_create_info.queueFamilyIndex       = r.i_graphics_family_index;
    command_pool_create_info.flags                  = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

    vkCreateCommandPool(device, &command_pool_create_info, nullptr, &command_pool);

    VkCommandBuffer command_buffer[2];
    VkCommandBufferAllocateInfo command_buffer_allocate_info {};
    command_buffer_allocate_info.sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    command_buffer_allocate_info.commandPool        = command_pool;
    command_buffer_allocate_info.commandBufferCount = 2;
    command_buffer_allocate_info.level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

    vkAllocateCommandBuffers(device, &command_buffer_allocate_info, command_buffer);
    // 1st command buffer
    {
        VkCommandBufferBeginInfo command_buffer_begin_info {};
        command_buffer_begin_info.sType                 = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        //command_buffer_begin_info.flags                 = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

        vkBeginCommandBuffer(command_buffer[0], &command_buffer_begin_info);

        vkCmdPipelineBarrier(
            command_buffer[0],
            VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
            VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
            0,
            0, nullptr,
            0, nullptr,
            0, nullptr
        );

        VkViewport viewport {};
        viewport.maxDepth       = 1.0f;
        viewport.minDepth       = 0.0f;
        viewport.width          = 512;
        viewport.height         = 512;
        viewport.x              = 0;
        viewport.y              = 0;
        vkCmdSetViewport(command_buffer[0], 0, 1, &viewport);

        vkEndCommandBuffer(command_buffer[0]);
    }

    // 2nd command buffer
    {
        VkCommandBufferBeginInfo command_buffer_begin_info {};
        command_buffer_begin_info.sType                 = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        //command_buffer_begin_info.flags                 = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

        vkBeginCommandBuffer(command_buffer[1], &command_buffer_begin_info);

        VkViewport viewport {};
        viewport.maxDepth       = 1.0f;
        viewport.minDepth       = 0.0f;
        viewport.width          = 512;
        viewport.height         = 512;
        viewport.x              = 0;
        viewport.y              = 0;
        vkCmdSetViewport(command_buffer[1], 0, 1, &viewport);

        vkEndCommandBuffer(command_buffer[1]);
    }

    // Submit 1st command buffer
    {
        VkSubmitInfo submit_info {};
        submit_info.sType                               = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submit_info.commandBufferCount                  = 1;
        submit_info.pCommandBuffers                     = &command_buffer[0];
        submit_info.signalSemaphoreCount                = 1;
        submit_info.pSignalSemaphores                   = &semaphore;


        vkQueueSubmit(queue, 1, &submit_info, VK_NULL_HANDLE);
    }

    // Submit 2nd command buffer
    {
        VkPipelineStageFlags pipeline_stage_flags[] { VK_PIPELINE_STAGE_ALL_COMMANDS_BIT };

        VkSubmitInfo submit_info {};
        submit_info.sType                               = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submit_info.commandBufferCount                  = 1;
        submit_info.pCommandBuffers                     = &command_buffer[1];
        submit_info.waitSemaphoreCount                  = 1;
        submit_info.pWaitSemaphores                     = &semaphore;
        submit_info.pWaitDstStageMask                   = pipeline_stage_flags;

        vkQueueSubmit(queue, 1, &submit_info, VK_NULL_HANDLE);
    }

    //VkResult result = vkWaitForFences(device, 1, &fence, VK_TRUE, UINT64_MAX);
    vkQueueWaitIdle(queue);

    vkDestroyCommandPool(device, command_pool, nullptr);

    vkDestroySemaphore(device, semaphore, nullptr);

    vkDestroyFence(device, fence, nullptr);

#endif
    
    return 0;

}

//#include <iostream>
//
//int main() {
//
//    std::cout << "Hello World;" << std::endl;
//
//    return 0;
//
//}