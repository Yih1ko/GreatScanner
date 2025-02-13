#ifndef LAYERMANAGER_H
#define LAYERMANAGER_H

/**
 * @brief 图层管理器类：用于管理多层图形对象和图片资源
 * @param scene 绑定的QGraphicsScene场景指针（可选）
 */
#include <QObject>
#include <QGraphicsView>
#include <QGraphicsitem>

class LayerManager : public QObject {
    Q_OBJECT
public:
    explicit LayerManager(QGraphicsScene* scene = nullptr, QObject* parent = nullptr)
        : QObject(parent), m_scene(scene) {
        addLayer(0);  // 初始化0层
    }

    //▄▄▄▄▄▄▄ 核心接口 ▄▄▄▄▄▄▄
    /**
     * @brief 添加图形对象到指定层
     * @param layer 层号（不存在的层会自动创建）
     * @param obj 要添加的QGraphicsObject对象
     */
    void addObject(int layer, QGraphicsObject* obj);

    /**
     * @brief 添加图片到指定层
     * @param layer 层号（不存在的层会自动创建）
     * @param pixmap 图片资源
     * @param pos 图片位置（默认左上角原点）
     */
    void addImage(int layer, const QPixmap& pixmap,
                  const QPointF& pos = QPointF(0,0));

    /**
     * @brief 将图层内容关联到场景
     * @param scene 目标QGraphicsScene场景
     */
    void attachToScene(QGraphicsScene* scene);

private:
    //▄▄▄▄▄▄▄ 内部方法 ▄▄▄▄▄▄▄
    void addLayer(int layer) {
        m_layers.insert(layer,  QList<QGraphicsItem*>());
    }

    //▄▄▄▄▄▄▄ 成员变量 ▄▄▄▄▄▄▄
    QMap<int, QList<QGraphicsItem*>> m_layers; // 层号->图形项列表
    QGraphicsScene* m_scene = nullptr;         // 关联的场景对象
};



#endif // LAYERMANAGER_H
