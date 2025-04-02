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
#include <QTransform>
#include <QRadioButton>
#include <QHeaderView> 
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
            menuBtn->setFixedWidth(75);  // 固定按钮宽度
            menuLayout->addWidget(menuBtn);
            connect(menuBtn, &QPushButton::clicked, [this, i]() {
                taskPanel->setVisible(true);
                tabWidget->setCurrentIndex(i);
            });
        }
        // 创建可隐藏的任务栏
        taskPanel = createTaskPanel();
        taskPanel->setFixedWidth(750);  // 固定任务栏宽度

        // 创建展开/收起按钮
        toggleTaskPanelBtn = new QPushButton("▼");
        toggleTaskPanelBtn->setFixedWidth(75);   // 设置小箭头按钮大小
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

        // 创建图像显示区域
        imageContainer = new QWidget();
        imageContainer->setFixedSize(600, 600);  // 固定为正方形区域
        imageGridLayout = new QGridLayout(imageContainer);
        imageGridLayout->setSpacing(0);  // 去除图像间距
        imageGridLayout->setContentsMargins(0, 0, 0, 0);  // 去除边距
        layout->addWidget(imageContainer, 0, Qt::AlignCenter);  // 居中显示
        imageContainer->setVisible(false);  // 初始隐藏

        return container;
    }

    void createImageWindow() {
        QWidget *window = new QWidget();
        window->setWindowTitle("图像窗口");
        QVBoxLayout *layout = new QVBoxLayout(window);

        QLabel *imageLabel = new QLabel();
        imageLabel->setAlignment(Qt::AlignCenter);
        imageLabel->setStyleSheet("background-color: #FFFFFF; border: 1px solid #1C6EA4;");
        imageLabel->setFixedSize(600, 600);

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
        leftPanel->setFixedWidth(150);  // 固定宽度
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
        sequenceList->setFixedWidth(375);  // 固定宽度

        // 右侧：操作按钮
        QWidget *rightPanel = new QWidget();
        rightPanel->setFixedWidth(150);  // 固定宽度
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
        
        // 基本序列选择
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

        // 基本序列范围
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

        // 读入序列按钮
        QPushButton *loadSequenceBtn = new QPushButton("读入序列");
        step1Layout->addWidget(loadSequenceBtn, 0, Qt::AlignRight);

        // 连接读入序列按钮
        QObject::connect(loadSequenceBtn, &QPushButton::clicked, [this, stack, stepLabel]() {
            // 创建步骤1_2：设置显示方式
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

            // 添加隐藏图像按钮
            QPushButton *hideImageBtn = new QPushButton("隐藏图像");
            displayLayout->addWidget(hideImageBtn);
            QObject::connect(hideImageBtn, &QPushButton::clicked, [this]() {
                imageContainer->setVisible(false);
            });

            displayGroup->setLayout(displayLayout);
            step1_2Layout->addWidget(displayGroup);

            // 下一步按钮
            QPushButton *nextStep1_2Btn = new QPushButton("下一步");
            step1_2Layout->addWidget(nextStep1_2Btn, 0, Qt::AlignRight);

            // 连接下一步按钮
            QObject::connect(nextStep1_2Btn, &QPushButton::clicked, [this, stack, stepLabel]() {
                // 切换到步骤2
                stack->setCurrentIndex(2); // 步骤2的索引为2
                stepLabel->setText("步骤 2/3: 读入参考序列");
            });

            // 添加步骤1_2到stack
            stack->insertWidget(1, step1_2); // 步骤1_2的索引为1
            stack->setCurrentIndex(1); // 切换到步骤1_2
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

        // 读入序列按钮
        QPushButton *loadRefSequenceBtn = new QPushButton("读入序列");
        step2Layout->addWidget(loadRefSequenceBtn, 0, Qt::AlignRight);

        // 连接读入序列按钮
        QObject::connect(loadRefSequenceBtn, &QPushButton::clicked, [this, stack, stepLabel]() {
            // 创建步骤2_2：设置显示方式
            QWidget *step2_2 = new QWidget();
            QVBoxLayout *step2_2Layout = new QVBoxLayout(step2_2);

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

            // 添加隐藏图像按钮
            QPushButton *hideImageBtn = new QPushButton("隐藏图像");
            displayLayout->addWidget(hideImageBtn);
            QObject::connect(hideImageBtn, &QPushButton::clicked, [this]() {
                imageContainer->setVisible(false);
            });

            displayGroup->setLayout(displayLayout);
            step2_2Layout->addWidget(displayGroup);
            // 下一步按钮
            QPushButton *nextStep1_2Btns = new QPushButton("上一步");
            step2_2Layout->addWidget(nextStep1_2Btns, 0, Qt::AlignRight);

            // 连接下一步按钮
            QObject::connect(nextStep1_2Btns, &QPushButton::clicked, [this, stack, stepLabel]() {
                stack->setCurrentIndex(1); // 切换到步骤1_2
                stepLabel->setText("步骤 1/3: 设置显示方式");
            });

            // 下一步按钮
            QPushButton *nextStep2_2Btn = new QPushButton("下一步");
            step2_2Layout->addWidget(nextStep2_2Btn, 0, Qt::AlignRight);

            // 连接下一步按钮
            QObject::connect(nextStep2_2Btn, &QPushButton::clicked, [stack, stepLabel]() {
                stack->setCurrentIndex(4); // 切换到步骤3
                stepLabel->setText("步骤 3/3: 序列融合");
            });

            // 添加步骤2_2到stack
            stack->insertWidget(3, step2_2); // 步骤2_2的索引为3
            stack->setCurrentIndex(3); // 切换到步骤2_2
            stepLabel->setText("步骤 2/3: 设置显示方式");
        });

        // 步骤3：序列融合
        QWidget *step3 = new QWidget();
        QHBoxLayout *step3Layout = new QHBoxLayout(step3);  // 设置step3的布局

        // 左侧：任务选单
        QWidget *taskPanel = new QWidget();
        QVBoxLayout *taskLayout = new QVBoxLayout(taskPanel);

        // 手动融合/自动融合选择
        QGroupBox *fusionModeGroup = new QGroupBox("融合方式");
        QVBoxLayout *fusionModeLayout = new QVBoxLayout();
        QRadioButton *manualFusionBtn = new QRadioButton("手动融合");
        QRadioButton *autoFusionBtn = new QRadioButton("自动融合");
        fusionModeLayout->addWidget(manualFusionBtn);
        fusionModeLayout->addWidget(autoFusionBtn);
        fusionModeGroup->setLayout(fusionModeLayout);
        taskLayout->addWidget(fusionModeGroup);

        // 手动融合功能
        QWidget *manualFusionPanel = new QWidget();
        QVBoxLayout *manualFusionLayout = new QVBoxLayout(manualFusionPanel);

        // 基本序列/参考序列/融合按钮
        QHBoxLayout *sequenceCtrlLayout = new QHBoxLayout();
        QPushButton *baseSeqBtn = new QPushButton("基本序列");
        QPushButton *refSeqBtn = new QPushButton("参考序列");
        QPushButton *fusionBtn = new QPushButton("融合");
        sequenceCtrlLayout->addWidget(baseSeqBtn);
        sequenceCtrlLayout->addWidget(refSeqBtn);
        sequenceCtrlLayout->addWidget(fusionBtn);
        manualFusionLayout->addLayout(sequenceCtrlLayout);

        // 标记点选取面板
        QWidget *markerPanel = new QWidget();
        QHBoxLayout *markerLayout = new QHBoxLayout(markerPanel);

        // 基本序列标记点
        QWidget *baseMarkerPanel = new QWidget();
        QVBoxLayout *baseMarkerLayout = new QVBoxLayout(baseMarkerPanel);
        QLabel *baseMarkerLabel = new QLabel("基本序列标记点");
        QListWidget *baseMarkerList = new QListWidget();
        baseMarkerList->addItems({"标记点1", "标记点2", "标记点3", "标记点4"});
        baseMarkerLayout->addWidget(baseMarkerLabel);
        baseMarkerLayout->addWidget(baseMarkerList);

        // 基本序列操作按钮
        QWidget *baseMarkerCtrlPanel = new QWidget();
        QVBoxLayout *baseMarkerCtrlLayout = new QVBoxLayout(baseMarkerCtrlPanel);
        QPushButton *addBaseMarkerBtn = new QPushButton("添加");
        QPushButton *replaceBaseMarkerBtn = new QPushButton("替换");
        QPushButton *deleteBaseMarkerBtn = new QPushButton("删除");
        QPushButton *clearBaseMarkerBtn = new QPushButton("删除全部");
        baseMarkerCtrlLayout->addWidget(addBaseMarkerBtn);
        baseMarkerCtrlLayout->addWidget(replaceBaseMarkerBtn);
        baseMarkerCtrlLayout->addWidget(deleteBaseMarkerBtn);
        baseMarkerCtrlLayout->addWidget(clearBaseMarkerBtn);
        baseMarkerCtrlPanel->setVisible(false); // 初始隐藏

        // 参考序列标记点
        QWidget *refMarkerPanel = new QWidget();
        QVBoxLayout *refMarkerLayout = new QVBoxLayout(refMarkerPanel);
        QLabel *refMarkerLabel = new QLabel("参考序列标记点");
        QListWidget *refMarkerList = new QListWidget();
        refMarkerList->addItems({"标记点1", "标记点2", "标记点3", "标记点4"});
        refMarkerLayout->addWidget(refMarkerLabel);
        refMarkerLayout->addWidget(refMarkerList);

        // 参考序列操作按钮
        QWidget *refMarkerCtrlPanel = new QWidget();
        QVBoxLayout *refMarkerCtrlLayout = new QVBoxLayout(refMarkerCtrlPanel);
        QPushButton *addRefMarkerBtn = new QPushButton("添加");
        QPushButton *replaceRefMarkerBtn = new QPushButton("替换");
        QPushButton *deleteRefMarkerBtn = new QPushButton("删除");
        QPushButton *clearRefMarkerBtn = new QPushButton("删除全部");
        refMarkerCtrlLayout->addWidget(addRefMarkerBtn);
        refMarkerCtrlLayout->addWidget(replaceRefMarkerBtn);
        refMarkerCtrlLayout->addWidget(deleteRefMarkerBtn);
        refMarkerCtrlLayout->addWidget(clearRefMarkerBtn);
        refMarkerCtrlPanel->setVisible(false); // 初始隐藏

        // 将标记点面板和操作按钮添加到布局
        markerLayout->addWidget(baseMarkerPanel);
        markerLayout->addWidget(baseMarkerCtrlPanel);
        markerLayout->addWidget(refMarkerPanel);
        markerLayout->addWidget(refMarkerCtrlPanel);
        manualFusionLayout->addWidget(markerPanel);

        // 连接基本序列按钮
        QObject::connect(baseSeqBtn, &QPushButton::clicked, [baseMarkerCtrlPanel, refMarkerCtrlPanel]() {
            baseMarkerCtrlPanel->setVisible(true);
            refMarkerCtrlPanel->setVisible(false);
        });

        // 连接参考序列按钮
        QObject::connect(refSeqBtn, &QPushButton::clicked, [baseMarkerCtrlPanel, refMarkerCtrlPanel]() {
            baseMarkerCtrlPanel->setVisible(false);
            refMarkerCtrlPanel->setVisible(true);
        });

        // 将手动融合面板添加到任务选单
        taskLayout->addWidget(manualFusionPanel);
        manualFusionPanel->setVisible(false); // 初始隐藏

        // 连接手动融合按钮
        QObject::connect(manualFusionBtn, &QRadioButton::toggled, [manualFusionPanel]() {
            manualFusionPanel->setVisible(true);
        });

        // 连接自动融合按钮
        QObject::connect(autoFusionBtn, &QRadioButton::toggled, [manualFusionPanel]() {
            manualFusionPanel->setVisible(false);
        });

        // 上一步按钮
        QPushButton *prevStep3Btn = new QPushButton("上一步");
        taskLayout->addWidget(prevStep3Btn);

        // 连接上一步按钮
        QObject::connect(prevStep3Btn, &QPushButton::clicked, [stack, stepLabel]() {
            stack->setCurrentIndex(3); // 切换到步骤2_2
            stepLabel->setText("步骤 2/3: 设置显示方式");
        });

        // 将任务选单添加到步骤3布局
        step3Layout->addWidget(taskPanel);

        // 添加所有步骤
        stack->addWidget(step1); // 步骤1的索引为0
        stack->addWidget(step2); // 步骤2的索引为2
        stack->addWidget(step3); // 步骤3的索引为4

        // 初始显示步骤1
        stack->setCurrentIndex(0);

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
        imageLabels.clear();

        // 计算每个图像的大小
        int imageSize = 600 / qMax(rows, cols);  // 600是固定图像区域大小

        // 创建新的图像显示
        for (int i = 0; i < rows * cols; ++i) {
            QLabel *imageLabel = new QLabel();
            imageLabel->setFixedSize(imageSize, imageSize);
            imageLabel->setAlignment(Qt::AlignCenter);
            imageLabel->setStyleSheet("background-color: #FFFFFF; border: 0px;");  // 去除边框

            // 加载对应编号的图片
            QString imagePath = QString("./img/%1.jpg").arg(i + 1);
            QPixmap image(imagePath);
            if(!image.isNull()) {
                imageLabel->setPixmap(image.scaled(imageSize, imageSize, Qt::KeepAspectRatio, Qt::SmoothTransformation));
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
            // 序列前后交换：将图像序列的前半部分和后半部分交换
            int half = imageLabels.size() / 2;
            for (int i = 0; i < half; ++i) {
                QPixmap temp = *imageLabels[i]->pixmap();
                imageLabels[i]->setPixmap(*imageLabels[i + half]->pixmap());
                imageLabels[i + half]->setPixmap(temp);
            }
        } else if (option == "图像左右交换") {
            // 图像左右镜像：将每张图像左右翻转
            for (QLabel *imageLabel : imageLabels) {
                QPixmap pixmap = *imageLabel->pixmap();
                QPixmap mirrored = pixmap.transformed(QTransform().scale(-1, 1)); // 水平镜像
                imageLabel->setPixmap(mirrored);
            }
        } else if (option == "图像上下交换") {
            // 图像上下镜像：将每张图像上下翻转
            for (QLabel *imageLabel : imageLabels) {
                QPixmap pixmap = *imageLabel->pixmap();
                QPixmap mirrored = pixmap.transformed(QTransform().scale(1, -1)); // 垂直镜像
                imageLabel->setPixmap(mirrored);
            }
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