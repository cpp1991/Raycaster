#include "maploadingscene.h"
#include "initialscene.h"
#include <QLineEdit>




maploadingscene::maploadingscene() : scene{}
{ 
    QLineEdit lineEdit{};
    this->scene.addWidget(&lineEdit);

    this->setSceneRect(0, 0, InitialScene::SCENE_WIDTH, InitialScene::SCENE_HEIGHT);
    this->setScene(&scene);

    this->show();
}
