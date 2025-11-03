#include "renderer/RenderContext.h"
#include "renderer/backends/VulkanRenderContext.h"
#include "renderer/backends/OpenGLRenderContext.h"

#ifdef _WIN32
#include "renderer/backends/DirectX11RenderContext.h"
#include "renderer/backends/DirectX12RenderContext.h"
#endif

#include <iostream>
#include <stdexcept>

namespace fresh {

std::unique_ptr<IRenderContext> RenderContextFactory::create(GraphicsAPI api) {
    // Check if API is available
    if (!isGraphicsAPIAvailable(api)) {
        std::cerr << "Graphics API " << getGraphicsAPIName(api) 
                  << " is not available on this platform" << std::endl;
        return nullptr;
    }
    
    switch (api) {
        case GraphicsAPI::Vulkan:
            std::cout << "Creating Vulkan render context" << std::endl;
            return std::make_unique<VulkanRenderContext>();
            
        case GraphicsAPI::OpenGL:
            std::cout << "Creating OpenGL render context" << std::endl;
            return std::make_unique<OpenGLRenderContext>();
            
#ifdef _WIN32
        case GraphicsAPI::DirectX11:
            std::cout << "Creating DirectX 11 render context" << std::endl;
            return std::make_unique<DirectX11RenderContext>();
            
        case GraphicsAPI::DirectX12:
            std::cout << "Creating DirectX 12 render context" << std::endl;
            return std::make_unique<DirectX12RenderContext>();
#endif
            
        case GraphicsAPI::Auto:
            return createBest();
            
        default:
            std::cerr << "Unknown graphics API" << std::endl;
            return nullptr;
    }
}

std::unique_ptr<IRenderContext> RenderContextFactory::createBest() {
    GraphicsAPI bestAPI = selectBestGraphicsAPI();
    std::cout << "Auto-selecting best graphics API: " 
              << getGraphicsAPIName(bestAPI) << std::endl;
    return create(bestAPI);
}

} // namespace fresh
