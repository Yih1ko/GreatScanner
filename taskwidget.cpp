#include "taskwidget.h"

TaskWidget::TaskWidget(QWidget *parent)
    : QWidget{parent}
{
    initUI();
    initSlots();
}

//更新进度条
void TaskWidget::slot_uploadProgress(int progress)
{
    m_progressBar->setValue(progress);
}

//尾插新任务卡片
void TaskWidget::slot_appendTask(quint16 id, const QString &filePath)
{
    // 获取文件信息
    QFileInfo fileInfo(filePath);
    switch (id) {
    case MsgId::ID_TEST_UPLOAD_FILE_REQ:
        for(int i=0;i<11;i++){
            auto *task = new InfoCard(this);
            task->setCardIcon(QIcon(":/image/MicaBase.png"));
            task->appendDes("Test");
            task->setFileInfo(fileInfo);
            QListWidgetItem *item = new QListWidgetItem;

            //qDebug()<<"chat_user_wid sizeHint is " << chat_user_wid->sizeHint();
            item->setSizeHint(task->sizeHint());
            m_taskList->addItem(item);
            m_taskList->setItemWidget(item, task);
            qDebug()<<"appended a new task card,MsgType: ID_TEST_UPLOAD_FILE_REQ, Size:"<<task->getFileInfo().size();
        }
        break;
    default:
        break;
    }
}

//完成一项上传就弹出
void TaskWidget::slot_popTask()
{
    if (m_taskList == nullptr) {
        qWarning("TaskList is null.");
        return;
    }

    m_taskList->popRow();
}


void TaskWidget::initUI()
{
    setMinimumWidth(310);
    m_progressBar = new RoundProgressBar(this);
    quint32 flags = RoundProgressBar::defaultText
                    | RoundProgressBar::decorateDot
                    | RoundProgressBar::linearColor
                    | RoundProgressBar::outterCircle
                    | RoundProgressBar::animation;
    m_progressBar->setControlFlags(flags);
    m_progressBar->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    m_taskList = new TaskList(this);
    m_taskList->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // 总布局
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, m_progressBar->width()/2, 0, 0);

    // 进度条布局
    QHBoxLayout* progressLayout = new QHBoxLayout;
    progressLayout->addWidget(m_progressBar);
    progressLayout->setAlignment(Qt::AlignCenter); // 水平居中

    mainLayout->addLayout(progressLayout);
    mainLayout->addWidget(m_taskList);
    mainLayout->setAlignment(Qt::AlignTop | Qt::AlignCenter);
}

void TaskWidget::initSlots()
{
    //加入了新上传任务：
    connect(FileUploadQueue::getInstance(), &FileUploadQueue::sig_appendTask, this, &TaskWidget::slot_appendTask);
    //上传总进度：
    connect(FileUploadQueue::getInstance(), &FileUploadQueue::uploadTotalProgressChanged, this, &TaskWidget::slot_uploadProgress);
    //完成了一个文件上传弹出顶端的一个卡片：
    connect(FileUploadQueue::getInstance(), &FileUploadQueue::oneFileUploaded, this, &TaskWidget::slot_popTask);
    //全部上传完成：
    connect(FileUploadQueue::getInstance(), &FileUploadQueue::allFilesUploaded, [this](){
        emit sig_allDone(true);//全部上传完成信号
    });
}
