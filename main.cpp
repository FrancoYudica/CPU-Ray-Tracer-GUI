#include "WEngine.h"
#include "CPU-Ray-Tracing/CPURayTracer.hpp"
#include "src/MainLayer.hpp"

int main()
{
    auto window_config = Wolf::Window::Configuration();
    window_config.fullscreen = false;
    window_config.title = "Ray tracing from the ground up";
    window_config.width = 1920;
    window_config.height = 900;
    bool success = Wolf::init(window_config);

    if (!success) {
        std::cout << "Unable to initialize Wolf" << std::endl;
        return -1;
    }

    for (uint32_t i = 0; i < 20; i++)
        std::cout << RT::Random::unit_float() << std::endl;


    auto& app = Wolf::Application::get();
    auto layer = std::make_shared<MainLayer>();
    app->add_layer(layer);
    app->run();


    return 0;
}