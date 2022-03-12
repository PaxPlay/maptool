#include "MainWindow.h"
#include "TestRenderScene.h"

int main()
{
    MainWindow mainWindow;
    TestRenderScene scene;
    mainWindow.getRenderContext().setScene(&scene);
    return mainWindow.runEventLoop();
}
