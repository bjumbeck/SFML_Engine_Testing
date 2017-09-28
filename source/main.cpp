#include "Application/Application.hpp"
#include "ResourceManagement/ResourceManager.hpp"
#include "Math/VectorMath.hpp"

int main()
{
    Application& application = Application::getInstance();
    application.runApplication();

    return 0;
}