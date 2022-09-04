#ifndef STAR_H
#define STAR_H

#include <QGraphicsItem>
#include <QObject>

class Star : public QObject, public QGraphicsItem {
  Q_OBJECT
public:
  Star(size_t radius, QColor color, QObject* parent);
protected:
  void paint(QPainter *painter, const QStyleOptionGraphicsItem*, QWidget*) override;
  QRectF boundingRect() const override;
private:
  QPolygonF polygon() const;
protected:
  size_t m_radius;
  QColor m_color;
};


#endif // STAR_H
