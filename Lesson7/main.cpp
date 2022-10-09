#include "scene.h"
#include <QApplication>
#include <QGraphicsView>

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);

  Scene *scene = new Scene;
  QGraphicsView view(scene);

  scene->setSceneRect(-10, -10, 800, 600);

  view.show();

  a.exec();
}
