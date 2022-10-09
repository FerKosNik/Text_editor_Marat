#include "scene.h"
#include "star.h"
#include "random_points.h"
#include <QGraphicsEllipseItem>
#include <QKeyEvent>
#include <QGraphicsSceneMouseEvent>
#include <QtMath>
#include <QGraphicsView>

Scene::Scene(QObject *parent)
  : QGraphicsScene(parent),
    m_activeItem(nullptr){
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event) {
  QGraphicsScene::mousePressEvent(event);

  int radius = getBetween(10, 50);
  int x = getBetween(10, 50);
  int y = getBetween(10, 50);
  int height = getBetween(10, 50);
  int weight = getBetween(10, 50);
  QPointF pos = event->scenePos();

  QColor color(getBetween(0,255), getBetween(0,255), getBetween(0,255));

  m_activeItem = nullptr;

  QList<QGraphicsItem *> foundItems = this->items(QPolygonF()
             << QPointF(event->scenePos().x(), event->scenePos().y())
             << QPointF(event->scenePos().x(), event->scenePos().y() + 1)
             << QPointF(event->scenePos().x() + 1, event->scenePos().y() + 1)
             << QPointF(event->scenePos().x() + 1, event->scenePos().y()));

  if (event->button() == Qt::RightButton && foundItems.isEmpty()) {
      switch(eventCount){
      case 1:
          m_activeItem = new QGraphicsRectItem(-x, -y, height*2, weight*2);
          static_cast<QGraphicsRectItem*>(m_activeItem)->setBrush(color);
          eventCount++;
          break;
      case 2:
          m_activeItem = new QGraphicsEllipseItem(-radius,-radius, radius*2, radius*2);
          static_cast<QGraphicsEllipseItem*>(m_activeItem)->setBrush(color);
          eventCount++;
          break;
      case 3:
          m_activeItem = new Star(radius, color, this);
          eventCount = 1;
          break;
      }
  }else if(event->button() == Qt::RightButton && !foundItems.isEmpty()){
      foreach (QGraphicsItem *item, foundItems) {
          delete item;
      }
  }


  if (nullptr == m_activeItem)
    return;
  else m_activeItem->setFlag(QGraphicsItem::ItemIsMovable, QGraphicsItem::ItemIgnoresTransformations);

  addItem(m_activeItem);
  m_activeItem->setPos(pos);
}

void Scene::wheelEvent(QGraphicsSceneWheelEvent *event){
    QGraphicsView *view = new QGraphicsView(this);
    qreal scaleFactor=pow((double)2, -event->delta() / 240.0);

    qreal factor = view->transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor<0.0002 || factor>0.8000)
        return;

    view->scale(scaleFactor, scaleFactor);
    view->resetCachedContent();
}
