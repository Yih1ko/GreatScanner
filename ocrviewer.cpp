#include "ocrviewer.h"
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QMouseEvent>
#include <QApplication>
#include <QClipboard>
#include <QDebug>
#include "Def.h"

OCRViewer::OCRViewer(QWidget* parent)
    : QGraphicsView(parent)
{
    setScene(new QGraphicsScene(this));
    setRenderHint(QPainter::Antialiasing);//抗锯齿
    setDragMode(QGraphicsView::NoDrag);
}

void OCRViewer::loadData(const nlohmann::json& jsonData) {
    scene()->clear();
    m_textItems.clear();
    m_currentItem = nullptr;
    m_isSelecting = false;

    try {
        // 1. 强化错误码处理机制
        if (!jsonData.contains("error"))  {
            qWarning() << "JSON缺少错误码字段";
            return;
        }

        const int errorCode = jsonData["error"].get<int>();
        if (errorCode != ipc::ErrorCodes::Success) {  // 使用defs.h中定义的错误码
            // 获取详细错误信息（如果存在）
            std::string errorMsg = jsonData.value("message",  "OCR识别失败");
            qWarning() << "OCR错误[" << errorCode << "]:"
                       << QString::fromStdString(errorMsg);

            // 2. 添加错误可视化提示（可选）
            auto* errorText = new QGraphicsTextItem("OCR处理失败\n错误码: " + QString::number(errorCode));
            errorText->setDefaultTextColor(Qt::red);
            scene()->addItem(errorText);
            return;
        }

        // 3. 增强图像路径验证
        if (!jsonData.contains("image_path"))  {
                qWarning() << "JSON缺少image_path字段/图源";
                return;
        }

        const std::string imgPath = jsonData["image_path"];
        QPixmap bg(QString::fromStdString(imgPath));

        qreal sourceDpr = bg.devicePixelRatio();
        if (qFuzzyIsNull(sourceDpr)) sourceDpr = 1.0;

        if (bg.isNull())  {
            qWarning() << "图片加载失败:" << QString::fromStdString(imgPath);

            // 4. 添加替代背景（灰色底+文字提示）
            QPixmap placeholder(scene()->width(), scene()->height());
            placeholder.fill(Qt::lightGray);
            scene()->addPixmap(placeholder);

            auto* text = new QGraphicsTextItem("图片加载失败:\n" + QString::fromStdString(imgPath));
            text->setDefaultTextColor(Qt::red);
            scene()->addItem(text);
            return;
        }

        scene()->setSceneRect(bg.rect());
        scene()->addPixmap(bg)->setZValue(-1);

        // 5. 强化文本对象数据验证
        if (!jsonData.contains("text_objects"))  {
            qWarning() << "JSON缺少text_objects字段";
            return;
        }

        const auto& textObjects = jsonData["text_objects"];
        if (!textObjects.is_array())  {
            qWarning() << "text_objects字段格式错误（应为数组）";
            return;
        }

        for (const auto& obj : textObjects) {
            // 6. 验证必需字段存在性
            if (!obj.contains("text")  || !obj.contains("box"))  {
                qWarning() << "文本对象缺失必要字段";
                continue;
            }

            std::string text = obj["text"];
            QPolygonF polygon;

            // 7. 验证坐标数据结构
            if (!obj["box"].is_array() || obj["box"].size() != 4) {
                qWarning() << "无效的box坐标结构";
                continue;
            }

            // 8. 坐标值解析保护
            try {
                for (const auto& point : obj["box"]) {
                    if (!point.is_array()  || point.size()  < 2) {
                        qWarning() << "无效的坐标点格式";
                        continue;
                    }
                    // double x = point[0].get<double>();
                    // double y = point[1].get<double>();
                    double x = point[0].get<double>() / sourceDpr;
                    double y = point[1].get<double>() / sourceDpr;
                    polygon << QPointF(x, y);
                }
            } catch (const nlohmann::json::exception& e) {
                qWarning() << "坐标解析错误:" << e.what();
                continue;
            }

            // 9. 多边形闭合检查（确保是四边形）
            if (polygon.size()  != 4) {
                qWarning() << "无效的四边形坐标点数量:" << polygon.size();
                continue;
            }

            auto* textItem = new PlaceholderTextItem(
                polygon,
                QString::fromStdString(text)
                );
            textItem->setZValue(1);
            scene()->addItem(textItem);
            m_textItems.append(textItem);
        }
        // 成功加载后发射文本信号
        emit textReady(formatAllText());
    } catch (const nlohmann::json::exception& e) {
        qWarning() << "JSON解析错误:" << e.what();

        // 10. 添加可视化错误提示
        auto* errorItem = new QGraphicsTextItem("JSON数据格式错误:\n" + QString(e.what()));
        errorItem->setDefaultTextColor(Qt::red);
        scene()->addItem(errorItem);
        // 加载失败发射空文本
        //emit textReady(QString());
    }
}

void OCRViewer::loadData(const nlohmann::json& jsonData, QImage &image)
{
    scene()->clear();
    m_textItems.clear();
    m_currentItem = nullptr;
    m_isSelecting = false;
    qreal sourceDpr = image.devicePixelRatio();
    if (qFuzzyIsNull(sourceDpr)) sourceDpr = 1.0;

    try {
        // 1. 强化错误码处理机制
        if (!jsonData.contains("error"))  {
            qWarning() << "JSON缺少错误码字段";
            return;
        }

        const int errorCode = jsonData["error"].get<int>();
        if (errorCode != ipc::ErrorCodes::Success) {  // 使用defs.h中定义的错误码
            // 获取详细错误信息（如果存在）
            std::string errorMsg = jsonData.value("message",  "OCR识别失败");
            qWarning() << "OCR错误[" << errorCode << "]:"
                       << QString::fromStdString(errorMsg);

            // 2. 添加错误可视化提示（可选）
            auto* errorText = new QGraphicsTextItem("OCR处理失败\n错误码: " + QString::number(errorCode));
            errorText->setDefaultTextColor(Qt::red);
            scene()->addItem(errorText);
            return;
        }

        // 3. 增强图像存在验证
        if (image.isNull())  {
            qWarning() << "缺少图源";
            return;
        }

        QPixmap bg = QPixmap::fromImage(image);

        if (bg.isNull())  {
            qWarning() << "图片加载失败:";

            // 4. 添加替代背景（灰色底+文字提示）
            QPixmap placeholder(scene()->width(), scene()->height());
            placeholder.fill(Qt::lightGray);
            scene()->addPixmap(placeholder);

            auto* text = new QGraphicsTextItem("图片加载失败:\n");
            text->setDefaultTextColor(Qt::red);
            scene()->addItem(text);
            return;
        }

        scene()->setSceneRect(bg.rect());
        scene()->addPixmap(bg)->setZValue(-1);

        // 5. 强化文本对象数据验证
        if (!jsonData.contains("text_objects"))  {
            qWarning() << "JSON缺少text_objects字段";
            return;
        }

        const auto& textObjects = jsonData["text_objects"];
        if (!textObjects.is_array())  {
            qWarning() << "text_objects字段格式错误（应为数组）";
            return;
        }

        for (const auto& obj : textObjects) {
            // 6. 验证必需字段存在性
            if (!obj.contains("text")  || !obj.contains("box"))  {
                qWarning() << "文本对象缺失必要字段";
                continue;
            }

            std::string text = obj["text"];
            QPolygonF polygon;

            // 7. 验证坐标数据结构
            if (!obj["box"].is_array() || obj["box"].size() != 4) {
                qWarning() << "无效的box坐标结构";
                continue;
            }

            // 8. 坐标值解析保护
            try {
                for (const auto& point : obj["box"]) {
                    if (!point.is_array()  || point.size()  < 2) {
                        qWarning() << "无效的坐标点格式";
                        continue;
                    }
                    // double x = point[0].get<double>();
                    // double y = point[1].get<double>();
                    // 关键修改：坐标除以DPR
                    double x = point[0].get<double>() / sourceDpr;
                    double y = point[1].get<double>() / sourceDpr;
                    polygon << QPointF(x, y);
                }
            } catch (const nlohmann::json::exception& e) {
                qWarning() << "坐标解析错误:" << e.what();
                continue;
            }

            // 9. 多边形闭合检查（确保是四边形）
            if (polygon.size()  != 4) {
                qWarning() << "无效的四边形坐标点数量:" << polygon.size();
                continue;
            }

            auto* textItem = new PlaceholderTextItem(
                polygon,
                QString::fromStdString(text)
                );
            textItem->setZValue(1);
            scene()->addItem(textItem);
            m_textItems.append(textItem);
        }
        // 成功加载后发射文本信号
        emit textReady(formatAllText());
    } catch (const nlohmann::json::exception& e) {
        qWarning() << "JSON解析错误:" << e.what();

        // 10. 添加可视化错误提示
        auto* errorItem = new QGraphicsTextItem("JSON数据格式错误:\n" + QString(e.what()));
        errorItem->setDefaultTextColor(Qt::red);
        scene()->addItem(errorItem);
        // 加载失败发射空文本
        //emit textReady(QString());
    }
}

void OCRViewer::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        // 查找点击位置下的文本项
        auto item = dynamic_cast<PlaceholderTextItem*>(itemAt(event->pos()));

        if (item) {
            // 点击文本项：开始文本选择
            m_currentItem = item;
            m_startPos = mapToScene(event->pos());
            m_selectionAnchor = charIndexAtPosition(item, m_startPos);
            m_isSelecting = true;

            // 设置初始选择（单个字符）
            item->setSelection(m_selectionAnchor, m_selectionAnchor + 1);
        } else {
            // 点击背景：清除所有选中状态
            clearAllSelections();

            // 可选择添加视觉反馈（可选）
            viewport()->update();  // 刷新视图
        }
    }
    QGraphicsView::mousePressEvent(event);
}

void OCRViewer::mouseMoveEvent(QMouseEvent* event) {
    if (m_isSelecting && m_currentItem) {
        QPointF currentPos = mapToScene(event->pos());
        int currentIndex = charIndexAtPosition(m_currentItem, currentPos);

        if (m_selectionAnchor >= 0 && currentIndex >= 0) {
            int start = qMin(m_selectionAnchor, currentIndex);
            int end = qMax(m_selectionAnchor, currentIndex) + 1;
            m_currentItem->setSelection(start, end);
        }
    }
    QGraphicsView::mouseMoveEvent(event);
}

void OCRViewer::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        // 修复：无论是否区域选择，都要结束选择状态
        m_isSelecting = false;          // 新增行
        m_isAreaSelecting = false;      // 保留现有逻辑

        // 可选：添加选择结束的视觉反馈
        if (m_currentItem) {
            m_currentItem->update();
        }
        event->accept();
        return;
    }
    QGraphicsView::mouseReleaseEvent(event);
}

void OCRViewer::keyPressEvent(QKeyEvent* event) {
    if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_C) {
        QStringList selectedParts;

        // 按Y坐标排序文本项（阅读顺序）
        QList<PlaceholderTextItem*> sortedItems = m_textItems;
        std::sort(sortedItems.begin(),  sortedItems.end(),
                  [](PlaceholderTextItem* a, PlaceholderTextItem* b) {
                      return a->sceneBoundingRect().top() < b->sceneBoundingRect().top();
                  });

        // 收集所有选中文本
        for (auto item : sortedItems) {
            if (item->selectionStart() >= 0) {
                selectedParts << item->selectedText();
            }
        }

        // 智能连接文本（行间换行，同行空格）
        QString finalText;
        QRectF prevRect;
        for (int i = 0; i < selectedParts.size();  ++i) {
            if (!finalText.isEmpty())  {
                QRectF currentRect = sortedItems[i]->sceneBoundingRect();
                // 判断是否在同一文本行（Y轴重叠）
                if (prevRect.isValid()  && qAbs(currentRect.top()  - prevRect.top())  < 5) {
                    finalText += " ";
                } else {
                    finalText += "\n";
                }
            }
            finalText += selectedParts[i];
            prevRect = sortedItems[i]->sceneBoundingRect();
        }

        if (!finalText.isEmpty())  {
            QApplication::clipboard()->setText(finalText);
            qDebug() << "已复制跨行文本:" << finalText;
        }
        event->accept();
        return;
    }
    QGraphicsView::keyPressEvent(event);
}

int OCRViewer::charIndexAtPosition(PlaceholderTextItem* item, const QPointF& scenePos) {
    QRectF rect = item->boundingRect();
    QPointF itemPos = item->mapFromScene(scenePos);

    const int charCount = item->fullText().length();
    if (charCount == 0) return -1;

    qreal charWidth = rect.width()  / charCount;
    return qBound(0, static_cast<int>((itemPos.x() - rect.left())  / charWidth), charCount - 1);
}

void OCRViewer::clearAllSelections()
{
    if (!scene()) return;

    foreach (QGraphicsItem* item, scene()->items()) {
        if (auto textItem = dynamic_cast<PlaceholderTextItem*>(item)) {
            textItem->setSelection(-1, -1);  // 清除选择
        }
    }

    m_currentItem = nullptr;    // 重置当前选中项
    m_isSelecting = false;      // 重置选择状态
    m_selectionAnchor = -1;     // 重置选择锚点
}

QString OCRViewer::formatAllText() const
{
    if (m_textItems.isEmpty())
        return QString();

    // 按Y坐标分组（行）
    QMap<qreal, QList<PlaceholderTextItem*>> lines;
    const qreal lineTolerance = 5.0; // 行高容差

    for (auto item : m_textItems) {
        qreal top = item->sceneBoundingRect().top();
        bool foundLine = false;

        // 查找相近Y坐标的行
        for (auto it = lines.begin();  it != lines.end();  ++it) {
            if (qAbs(it.key()  - top) < lineTolerance) {
                it.value().append(item);
                foundLine = true;
                break;
            }
        }

        // 新行处理
        if (!foundLine) {
            lines.insert(top,  {item});
        }
    }

    // 处理每行文本
    QStringList allLines;
    QList<qreal> sortedKeys = lines.keys();
    std::sort(sortedKeys.begin(),  sortedKeys.end());

    for (qreal key : sortedKeys) {
        auto& lineItems = lines[key];

        // 行内按X坐标排序（从左到右）
        std::sort(lineItems.begin(),  lineItems.end(),
                  [](PlaceholderTextItem* a, PlaceholderTextItem* b) {
                      return a->sceneBoundingRect().left() < b->sceneBoundingRect().left();
                  });

        // 连接当前行文本
        QStringList words;
        for (auto item : lineItems) {
            words.append(item->fullText());
        }
        allLines.append(words.join("  ")); // 单词间加空格
    }

    return allLines.join("\n");  // 行间加换行
}

void OCRViewer::updateAreaSelection(const QRectF &rect)
{
    for (auto item : m_textItems) {
        QRectF itemRect = item->mapRectToScene(item->boundingRect());
        if (rect.intersects(itemRect))  {
            applySelectionToItem(item, rect);
        } else {
            item->setSelection(-1, -1);
        }
    }
}

void OCRViewer::applySelectionToItem(PlaceholderTextItem *item, const QRectF &selectRect)
{
    QRectF itemRect = item->boundingRect();
    QRectF sceneItemRect = item->mapRectToScene(itemRect);
    QRectF intersected = selectRect.intersected(sceneItemRect);

    // 转换到项本地坐标系
    QPointF localTopLeft = item->mapFromScene(intersected.topLeft());
    QPointF localBottomRight = item->mapFromScene(intersected.bottomRight());

    // 计算字符索引
    const int charCount = item->fullText().length();
    const qreal charWidth = itemRect.width()  / charCount;

    int startIndex = qBound(0, static_cast<int>((localTopLeft.x() - itemRect.left())  / charWidth), charCount-1);
    int endIndex = qBound(0, static_cast<int>((localBottomRight.x() - itemRect.left())  / charWidth) + 1, charCount);

    item->setSelection(startIndex, endIndex);
}
