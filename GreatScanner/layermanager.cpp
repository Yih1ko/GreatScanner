#include "layermanager.h"


void LayerManager::addObject(int layer, QGraphicsObject *obj) {
    if (!m_layers.contains(layer))  addLayer(layer);
    obj->setZValue(layer);   // 设置Z轴值
    m_layers[layer].append(obj);
    if(m_scene) m_scene->addItem(obj);
}

void LayerManager::addImage(int layer, const QPixmap &pixmap, const QPointF &pos) {
    if (!m_layers.contains(layer))  addLayer(layer);
    QGraphicsPixmapItem* item = new QGraphicsPixmapItem(pixmap);
    item->setPos(pos);
    item->setZValue(layer);  // 设置Z轴值
    m_layers[layer].append(item);
    if(m_scene) m_scene->addItem(item);
}

void LayerManager::attachToScene(QGraphicsScene *scene) {
    if(m_scene == scene) return;
    m_scene = scene;
    // 将现有元素添加到新场景
    foreach(int layer, m_layers.keys())  {
        foreach(QGraphicsItem* item, m_layers[layer]) {
            m_scene->addItem(item);
        }
    }
}
