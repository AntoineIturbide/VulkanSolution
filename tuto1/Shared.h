#pragma once
#include "BUILD_OPTIONS.h"
#include "Platform.h"

#include <assert.h>
#include <iostream>

#if BUILD_ENABLE_VULKAN_RUNTIME_DEBUG
const void ErrorCheck(VkResult result);
#endif