#include "MainWindow.h"
#include "TestRenderScene.h"
#include "OWMap.h"

int main()
{
    MainWindow mainWindow;
    //TestRenderScene scene;
    OWMap scene { "../assets/Rialto/871/", "Rialto.owmap" };
    mainWindow.getRenderContext().setScene(&scene);
    return mainWindow.runEventLoop();
}
