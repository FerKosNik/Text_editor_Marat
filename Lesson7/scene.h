#ifndef SCENE_H
#define SCENE_H
#include <QGraphicsScene>
#include <QWheelEvent>

class Scene : public QGraphicsScene {
  Q_OBJECT
public:
  explicit Scene(QObject *parent = nullptr);
    int eventCount = 1;

protected:
  void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
  void wheelEvent(QGraphicsSceneWheelEvent *wheelEvent) override;
  QGraphicsItem *m_activeItem;

};

#endif // SCENE_H
