#include <QApplication>
#include <QMainWindow>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTabWidget>
#include <QListWidget>
#include <QLabel>
#include <QPushButton>
#include <QSplitter>
#include <QStringList>
#include <QGroupBox>
#include <QFormLayout>
#include <QTableWidget>
#include <QComboBox>
#include <QLineEdit>
#include <QGridLayout>
#include <QStackedWidget>
#include <QPixmap>
#include <QTimer>

class NavigationSystemUI : public QMainWindow {
public:
    NavigationSystemUI(QWidget *parent = nullptr) : QMainWindow(parent) {
        setWindowTitle("excelim-04 神经外科手术导航系统");

        // 主窗口部件
        QWidget *mainWidget = new QWidget(this);
        QVBoxLayout *mainLayout = new QVBoxLayout(mainWidget);

        // 创建菜单按钮
        QHBoxLayout *menuLayout = new QHBoxLayout();
        QStringList menuTitles = {"数据管理", "序列操作", "计划管理", "系统设置", "导航控制", "数据存储"};
        for (int i = 0; i < menuTitles.size(); ++i) {
            QPushButton *menuBtn = new QPushButton(menuTitles[i]);
            menuBtn->setFixedWidth(100);  // 固定按钮宽度
            menuLayout->addWidget(menuBtn);
            connect(menuBtn, &QPushButton::clicked, [this, i]() {
                taskPanel->setVisible(true);
                tabWidget->setCurrentIndex(i);
            });
        }
        // 创建可隐藏的任务栏
        taskPanel = createTaskPanel();
        taskPanel->setFixedWidth(1000);  // 固定任务栏宽度

        // 创建展开/收起按钮
        toggleTaskPanelBtn = new QPushButton("▼");
        toggleTaskPanelBtn->setFixedWidth(100);   // 设置小箭头按钮大小
        menuLayout->addWidget(toggleTaskPanelBtn, 0, Qt::AlignLeft);
        mainLayout->addLayout(menuLayout);
        mainLayout->addWidget(taskPanel);
        taskPanel->setVisible(true);  // 初始隐藏任务栏

        // 连接显示/隐藏按钮
        QObject::connect(toggleTaskPanelBtn, &QPushButton::clicked, [this]() {
            bool visible = !taskPanel->isVisible();

            // 2.1 在界面显示（隐藏）前强制刷新
            this->adjustSize();
            this->updateGeometry();

            // 显示或隐藏任务栏
            taskPanel->setVisible(visible);
            toggleTaskPanelBtn->setText(visible ? "▲" : "▼");

            // 2.2 在子控件显示（隐藏）后强制刷新
            QTimer::singleShot(0, this, [this]() {
                this->adjustSize();
                this->updateGeometry();
            });
        });

        // 创建图像显示区域
        imagePanel = createImagePanel();
        mainLayout->addWidget(imagePanel, 1);

        setCentralWidget(mainWidget);
    }

private:
    QWidget* createTaskPanel() {
        QWidget *container = new QWidget();
        QVBoxLayout *layout = new QVBoxLayout(container);

        // 创建菜单标签页
        tabWidget = new QTabWidget();
        tabWidget->setTabPosition(QTabWidget::North);
        createDataTab(tabWidget);
        createSequenceTab(tabWidget);
        createPlanTab(tabWidget);
        createSettingsTab(tabWidget);
        createNavigationTab(tabWidget);
        createStorageTab(tabWidget);

        // 创建全局导航按钮
        QHBoxLayout *navLayout = new QHBoxLayout();
        QPushButton *globalPrev = new QPushButton("上一步");
        QPushButton *globalNext = new QPushButton("下一步");
        navLayout->addStretch();
        navLayout->addWidget(globalPrev);
        navLayout->addWidget(globalNext);

        // 连接全局导航信号
        QObject::connect(globalPrev, &QPushButton::clicked, [this]() {
            int current = tabWidget->currentIndex();
            if (current > 0) {
                tabWidget->setCurrentIndex(current - 1);
            }
        });

        QObject::connect(globalNext, &QPushButton::clicked, [this]() {
            int current = tabWidget->currentIndex();
            if (current < tabWidget->count() - 1) {
                tabWidget->setCurrentIndex(current + 1);
            }
        });

        // 整体布局
        layout->addWidget(tabWidget);
        layout->addLayout(navLayout);

        return container;
    }

    QWidget* createImagePanel() {
        QWidget *container = new QWidget();
        QVBoxLayout *layout = new QVBoxLayout(container);
        layout->setContentsMargins(0, 0, 0, 0);

        // // 创建图像窗口管理
        // QHBoxLayout *imageWindowLayout = new QHBoxLayout();
        // QPushButton *newWindowBtn = new QPushButton("新建图像窗口");
        // imageWindowLayout->addWidget(newWindowBtn);
        // layout->addLayout(imageWindowLayout);

        // 创建图像显示区域
        imageContainer = new QWidget();
        imageGridLayout = new QGridLayout(imageContainer);
        imageGridLayout->setSpacing(0);  // 去除图像间距
        imageGridLayout->setContentsMargins(0, 0, 0, 0);
        layout->addWidget(imageContainer);
        imageContainer->setVisible(false);  // 初始隐藏

        // // 连接新建窗口按钮
        // QObject::connect(newWindowBtn, &QPushButton::clicked, [this]() {
        //     createImageWindow();
        // });

        return container;
    }

    void createImageWindow() {
        QWidget *window = new QWidget();
        window->setWindowTitle("图像窗口");
        QVBoxLayout *layout = new QVBoxLayout(window);

        QLabel *imageLabel = new QLabel();
        imageLabel->setAlignment(Qt::AlignCenter);
        imageLabel->setStyleSheet("background-color: #FFFFFF; border: 1px solid #1C6EA4;");
        imageLabel->setFixedSize(800, 800);

        // 加载默认图片
        QPixmap defaultImage("./img/1.jpg");
        if(!defaultImage.isNull()) {
            imageLabel->setPixmap(defaultImage.scaled(imageLabel->size(), Qt::KeepAspectRatio));
        } else {
            imageLabel->setText("默认图片加载失败");
        }

        layout->addWidget(imageLabel);
        window->setLayout(layout);
        window->show();
    }

    // 创建各个菜单标签页
    void createDataTab(QTabWidget *tabWidget) {
        QWidget *tab = new QWidget();
        QHBoxLayout *mainLayout = new QHBoxLayout(tab);

        // 左侧：模式切换和硬盘空间显示
        QWidget *leftPanel = new QWidget();
        leftPanel->setFixedWidth(200);  // 固定宽度
        QVBoxLayout *leftLayout = new QVBoxLayout(leftPanel);
        
        // 模式切换按钮
        QHBoxLayout *modeLayout = new QHBoxLayout();
        QPushButton *hardDiskBtn = new QPushButton("硬盘数据");
        QPushButton *cdRomBtn = new QPushButton("光盘数据");
        modeLayout->addWidget(hardDiskBtn);
        modeLayout->addWidget(cdRomBtn);
        leftLayout->addLayout(modeLayout);

        // 硬盘/光盘空间显示
        QLabel *diskSpaceLabel = new QLabel();
        diskSpaceLabel->setWordWrap(true);  // 设置自动换行
        diskSpaceLabel->setStyleSheet("border: 1px solid #ccc; padding: 5px;");  // 添加边框
        diskSpaceLabel->setText("点击上面按钮分别进入到不同的数据管理任务模式下，当前是管理硬盘数据任务模式。\n硬盘空间： 500GB / 1TB");
        QObject::connect(hardDiskBtn, &QPushButton::clicked, [diskSpaceLabel]() {
            diskSpaceLabel->setText("点击上面按钮分别进入到不同的数据管理任务模式下，当前是管理硬盘数据任务模式。\n硬盘空间： 500GB / 1TB");
        });
        QObject::connect(cdRomBtn, &QPushButton::clicked, [diskSpaceLabel]() {
            diskSpaceLabel->setText("点击上面按钮分别进入到不同的数据管理任务模式下，当前是管理光盘数据任务模式。\n光盘空间： 4.7GB / 4.7GB");
        });
        leftLayout->addWidget(diskSpaceLabel);
        leftLayout->addStretch();

        // 中间：序列列表
        QListWidget *sequenceList = new QListWidget();
        sequenceList->setSelectionMode(QAbstractItemView::SingleSelection);
        sequenceList->setFixedWidth(500);  // 固定宽度

        // 右侧：操作按钮
        QWidget *rightPanel = new QWidget();
        rightPanel->setFixedWidth(200);  // 固定宽度
        QVBoxLayout *rightLayout = new QVBoxLayout(rightPanel);
        
        QPushButton *portBtn = new QPushButton("导入序列");
        QPushButton *deleteBtn = new QPushButton("删除序列");
        QPushButton *refreshBtn = new QPushButton("扫描硬盘");
        QComboBox *sortCombo = new QComboBox();
        sortCombo->addItems({"按姓名", "按时间", "按种类", "按数量"});
        rightLayout->addWidget(portBtn);
        rightLayout->addWidget(deleteBtn);
        rightLayout->addWidget(refreshBtn);
        rightLayout->addWidget(sortCombo);
        rightLayout->addStretch();

        // 将三部分添加到主布局
        mainLayout->addWidget(leftPanel);
        mainLayout->addWidget(sequenceList);
        mainLayout->addWidget(rightPanel);

        tabWidget->addTab(tab, "数据管理");
    }

    void createSequenceTab(QTabWidget *tabWidget) {
        QWidget *tab = new QWidget();
        QVBoxLayout *layout = new QVBoxLayout(tab);

        // 步骤指示器
        QLabel *stepLabel = new QLabel("步骤 1/3: 读入基本序列");
        stepLabel->setStyleSheet("font-weight: bold; font-size: 14px;");
        layout->addWidget(stepLabel);

        // 主面板
        QStackedWidget *stack = new QStackedWidget();

        // 步骤1：读入基本序列
        QWidget *step1 = new QWidget();
        QVBoxLayout *step1Layout = new QVBoxLayout(step1);
        
        // 序列选择
        QHBoxLayout *selectLayout = new QHBoxLayout();
        QLabel *seqLabel = new QLabel("选择序列:");
        QComboBox *seqCombo = new QComboBox();
        QLabel *typeLabel = new QLabel("序列类型:");
        QComboBox *typeCombo = new QComboBox();
        typeCombo->addItems({"水平面断层", "冠状面断层", "矢状面断层"});
        selectLayout->addWidget(seqLabel);
        selectLayout->addWidget(seqCombo);
        selectLayout->addWidget(typeLabel);
        selectLayout->addWidget(typeCombo);
        step1Layout->addLayout(selectLayout);

        // 序列范围
        QHBoxLayout *rangeLayout = new QHBoxLayout();
        QLabel *startLabel = new QLabel("起始编号:");
        QLineEdit *startEdit = new QLineEdit();
        QLabel *endLabel = new QLabel("终止编号:");
        QLineEdit *endEdit = new QLineEdit();
        rangeLayout->addWidget(startLabel);
        rangeLayout->addWidget(startEdit);
        rangeLayout->addWidget(endLabel);
        rangeLayout->addWidget(endEdit);
        step1Layout->addLayout(rangeLayout);

        // 下一步按钮
        QPushButton *nextStep1Btn = new QPushButton("下一步");
        step1Layout->addWidget(nextStep1Btn, 0, Qt::AlignRight);

        // 连接下一步按钮
        QObject::connect(nextStep1Btn, &QPushButton::clicked, [this, stack, stepLabel]() {
            // 创建新的步骤1_2
            QWidget *step1_2 = new QWidget();
            QVBoxLayout *step1_2Layout = new QVBoxLayout(step1_2);

            // 显示方式子菜单
            QGroupBox *displayGroup = new QGroupBox("显示方式");
            QHBoxLayout *displayLayout = new QHBoxLayout();
            QStringList displayOptions = {"1×1", "2×2", "3×3", "4×4"};
            for (const QString &option : displayOptions) {
                QPushButton *btn = new QPushButton(option);
                displayLayout->addWidget(btn);
                QObject::connect(btn, &QPushButton::clicked, [this, option]() {
                    updateImageDisplay(option);
                });
            }
            displayGroup->setLayout(displayLayout);
            step1_2Layout->addWidget(displayGroup);

            // 图像翻转子菜单
            QGroupBox *transformGroup = new QGroupBox("图像变换");
            QHBoxLayout *transformLayout = new QHBoxLayout();
            QStringList transformOptions = {"序列前后交换", "图像左右交换", "图像上下交换"};
            for (const QString &option : transformOptions) {
                QPushButton *btn = new QPushButton(option);
                transformLayout->addWidget(btn);
                QObject::connect(btn, &QPushButton::clicked, [this, option]() {
                    transformImages(option);
                });
            }
            transformGroup->setLayout(transformLayout);
            step1_2Layout->addWidget(transformGroup);

            // 添加步骤1_2到stack
            stack->insertWidget(1, step1_2);
            stack->setCurrentIndex(1);
            stepLabel->setText("步骤 1/3: 设置显示方式");
        });

        // 步骤2：读入参考序列
        QWidget *step2 = new QWidget();
        QVBoxLayout *step2Layout = new QVBoxLayout(step2);
        
        // 参考序列选择
        QHBoxLayout *refSelectLayout = new QHBoxLayout();
        QLabel *refSeqLabel = new QLabel("选择参考序列:");
        QComboBox *refSeqCombo = new QComboBox();
        QLabel *refTypeLabel = new QLabel("序列类型:");
        QComboBox *refTypeCombo = new QComboBox();
        refTypeCombo->addItems({"水平面断层", "冠状面断层", "矢状面断层"});
        refSelectLayout->addWidget(refSeqLabel);
        refSelectLayout->addWidget(refSeqCombo);
        refSelectLayout->addWidget(refTypeLabel);
        refSelectLayout->addWidget(refTypeCombo);
        step2Layout->addLayout(refSelectLayout);

        // 参考序列范围
        QHBoxLayout *refRangeLayout = new QHBoxLayout();
        QLabel *refStartLabel = new QLabel("起始编号:");
        QLineEdit *refStartEdit = new QLineEdit();
        QLabel *refEndLabel = new QLabel("终止编号:");
        QLineEdit *refEndEdit = new QLineEdit();
        refRangeLayout->addWidget(refStartLabel);
        refRangeLayout->addWidget(refStartEdit);
        refRangeLayout->addWidget(refEndLabel);
        refRangeLayout->addWidget(refEndEdit);
        step2Layout->addLayout(refRangeLayout);

        // 操作按钮
        QHBoxLayout *step2BtnLayout = new QHBoxLayout();
        QPushButton *prevStep2Btn = new QPushButton("上一步");
        QPushButton *nextStep2Btn = new QPushButton("下一步");
        step2BtnLayout->addWidget(prevStep2Btn);
        step2BtnLayout->addWidget(nextStep2Btn);
        step2Layout->addLayout(step2BtnLayout);

        // 步骤3：序列融合
        QWidget *step3 = new QWidget();
        QVBoxLayout *step3Layout = new QVBoxLayout(step3);
        
        // 融合控制
        QHBoxLayout *fusionCtrlLayout = new QHBoxLayout();
        QPushButton *manualFusionBtn = new QPushButton("手动融合");
        QPushButton *autoFusionBtn = new QPushButton("自动融合");
        QPushButton *prevStep3Btn = new QPushButton("上一步");
        fusionCtrlLayout->addWidget(manualFusionBtn);
        fusionCtrlLayout->addWidget(autoFusionBtn);
        fusionCtrlLayout->addWidget(prevStep3Btn);
        step3Layout->addLayout(fusionCtrlLayout);

        // 添加步骤
        stack->addWidget(step1);
        stack->addWidget(step2);
        stack->addWidget(step3);

        
        QObject::connect(prevStep2Btn, &QPushButton::clicked, [stack, stepLabel]() {
            stack->setCurrentIndex(0);
            stepLabel->setText("步骤 1/3: 读入基本序列");
        });

        

        QObject::connect(prevStep3Btn, &QPushButton::clicked, [stack, stepLabel]() {
            stack->setCurrentIndex(1);
            stepLabel->setText("步骤 2/3: 读入参考序列");
        });
        QObject::connect(nextStep2Btn, &QPushButton::clicked, [stack, stepLabel]() {
            stack->setCurrentIndex(2);
            stepLabel->setText("步骤 3/3: 序列融合");
        });

        layout->addWidget(stack);
        tabWidget->addTab(tab, "序列操作");
    }

    void createPlanTab(QTabWidget *tabWidget) {
        QWidget *tab = new QWidget();
        QVBoxLayout *layout = new QVBoxLayout(tab);
        layout->addWidget(new QLabel("计划管理功能区域"));
        tabWidget->addTab(tab, "计划管理");
    }

    void createSettingsTab(QTabWidget *tabWidget) {
        QWidget *tab = new QWidget();
        QVBoxLayout *layout = new QVBoxLayout(tab);
        layout->addWidget(new QLabel("系统设置功能区域"));
        tabWidget->addTab(tab, "系统设置");
    }

    void createNavigationTab(QTabWidget *tabWidget) {
        QWidget *tab = new QWidget();
        QVBoxLayout *layout = new QVBoxLayout(tab);
        layout->addWidget(new QLabel("导航控制功能区域"));
        tabWidget->addTab(tab, "导航控制");
    }

    void createStorageTab(QTabWidget *tabWidget) {
        QWidget *tab = new QWidget();
        QVBoxLayout *layout = new QVBoxLayout(tab);
        layout->addWidget(new QLabel("数据存储功能区域"));
        tabWidget->addTab(tab, "数据存储");
    }

    QWidget *taskPanel;  // 任务栏面板
    QWidget *imagePanel; // 图像显示面板
    QPushButton *toggleTaskPanelBtn; // 展开/收起按钮
    QTabWidget *tabWidget; // 标签页控件

    // 新增成员变量
    QGridLayout *imageGridLayout;
    QWidget *imageContainer;
    QVector<QLabel*> imageLabels;

    // 新增方法：更新图像显示
    void updateImageDisplay(const QString &option) {
        // 解析显示方式
        int rows = option.split("×")[0].toInt();
        int cols = option.split("×")[1].toInt();

        // 清除原有布局
        QLayoutItem *child;
        while ((child = imageGridLayout->takeAt(0)) != nullptr) {
            delete child->widget();
            delete child;
        }

        // 创建新的图像显示
        int imageSize = 800 / qMax(rows, cols);  // 800是固定图像区域大小
        for (int i = 0; i < rows * cols; ++i) {
            QLabel *imageLabel = new QLabel();
            imageLabel->setFixedSize(imageSize, imageSize);
            imageLabel->setAlignment(Qt::AlignCenter);
            imageLabel->setStyleSheet("background-color: #FFFFFF; border: 1px solid #1C6EA4;");

            // 加载默认图片
            QPixmap defaultImage("./img/1.jpg");
            if(!defaultImage.isNull()) {
                imageLabel->setPixmap(defaultImage.scaled(imageSize, imageSize, Qt::KeepAspectRatio));
            } else {
                imageLabel->setText("图片加载失败");
            }

            imageGridLayout->addWidget(imageLabel, i / cols, i % cols);
            imageLabels.append(imageLabel);
        }

        // 显示图像区域
        imageContainer->setVisible(true);
    }

    // 新增方法：图像变换
    void transformImages(const QString &option) {
        if (option == "序列前后交换") {
            // 实现序列前后交换逻辑
            // ...
        } else if (option == "图像左右交换") {
            // 实现图像左右交换逻辑
            // ...
        } else if (option == "图像上下交换") {
            // 实现图像上下交换逻辑
            // ...
        }
    }
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // 设置全局样式
    QString globalStyle = R"(
        QMainWindow {
            background-color: #F0F8FF; 
        }
        QTabWidget::pane {
            border: 1px solid #87CEEB; 
            background-color: #FFFFFF;
        }
        QTabBar::tab {
            background: #87CEEB; 
            color: #000000;
            padding: 8px;
            border: 1px solid #87CEEB;
        }
        QTabBar::tab:selected {
            background: #ADD8E6; 
            color: #000000; 
        }
        QListWidget {
            background-color: #87CEEB;
            color: #000000;
            border: 1px solid #ADD8E6;
        }
        QPushButton {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #87CEEB, stop:1 #ADD8E6); 
            color: #000000;
            border: 1px solid #000000;
            padding: 5px;
            width: 100px;  
            height: 30px;  
            font-size: 12px;
            border-radius: 5px;
        }
        QPushButton:hover {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #ADD8E6, stop:1 #87CEEB);
            border: 2px solid #00BFFF;
        }
        QPushButton:pressed {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #87CEEB, stop:1 #87CEEB);
            border: 2px solid #FFA500;
            color: #000000;
        }
        QLabel {
            color: #000000;
        }
        QSplitter {
            background-color: #FFFFFF;
        }
        QListWidget::item:selected {
            border: 2px solid #00BFFF;
            background-color: #87CEEB;
            color: #000000;
        }
        QListWidget::item:hover {
            background-color: #ADD8E6;
        }
    )";
    app.setStyleSheet(globalStyle);

    NavigationSystemUI window;
    window.show();
    return app.exec();
}