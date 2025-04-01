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

class NavigationSystemUI : public QMainWindow {
public:
    NavigationSystemUI(QWidget *parent = nullptr) : QMainWindow(parent) {
        setWindowTitle("excelim-04 神经外科手术导航系统");
        setGeometry(100, 100, 1280, 720);

        // 主窗口部件
        QWidget *mainWidget = new QWidget(this);
        QVBoxLayout *mainLayout = new QVBoxLayout(mainWidget);

        // 顶部任务面板
        mainLayout->addWidget(createTaskPanel());

        // 图像显示区域
        mainLayout->addWidget(createImageArea(), 1);

        setCentralWidget(mainWidget);
    }

private:
    QWidget* createTaskPanel() {
        QTabWidget *tabWidget = new QTabWidget();
        tabWidget->setTabPosition(QTabWidget::North);

        // 数据管理标签页
        QWidget *dataTab = new QWidget();
        QHBoxLayout *dataLayout = new QHBoxLayout(dataTab);
        QListWidget *dataList = new QListWidget();
        QStringList dataItems = {"预览", "导入", "删除"};
        dataList->addItems(dataItems);
        dataList->setFixedWidth(150);
        dataList->setCurrentRow(0); // 默认选中第一项
        QPushButton *dataPrev = new QPushButton("上一步");
        QPushButton *dataNext = new QPushButton("下一步");
        QVBoxLayout *dataBtnLayout = new QVBoxLayout();
        dataBtnLayout->addWidget(dataPrev);
        dataBtnLayout->addWidget(dataNext);
        dataBtnLayout->setAlignment(Qt::AlignTop);
        dataLayout->addWidget(dataList);
        dataLayout->addLayout(dataBtnLayout);
        dataLayout->addStretch(1);
        tabWidget->addTab(dataTab, "数据管理");

        // 为数据管理标签页添加按钮功能
        int dataCurrentIndex = 0;
        QObject::connect(dataPrev, &QPushButton::clicked, [dataList, &dataCurrentIndex]() {
            if (dataCurrentIndex > 0) {
                dataCurrentIndex--;
                dataList->setCurrentRow(dataCurrentIndex);
            }
        });
        QObject::connect(dataNext, &QPushButton::clicked, [dataList, &dataCurrentIndex, dataItems]() {
            if (dataCurrentIndex < dataItems.size() - 1) {
                dataCurrentIndex++;
                dataList->setCurrentRow(dataCurrentIndex);
            }
        });

        // 读入序列标签页
        QWidget *sequenceTab = new QWidget();
        QHBoxLayout *sequenceLayout = new QHBoxLayout(sequenceTab);
        QListWidget *sequenceList = new QListWidget();
        QStringList sequenceItems = {"基本序列", "参考序列", "序列融合"};
        sequenceList->addItems(sequenceItems);
        sequenceList->setFixedWidth(150);
        sequenceList->setCurrentRow(0); // 默认选中第一项
        QPushButton *sequencePrev = new QPushButton("上一步");
        QPushButton *sequenceNext = new QPushButton("下一步");
        QVBoxLayout *sequenceBtnLayout = new QVBoxLayout();
        sequenceBtnLayout->addWidget(sequencePrev);
        sequenceBtnLayout->addWidget(sequenceNext);
        sequenceBtnLayout->setAlignment(Qt::AlignTop);
        sequenceLayout->addWidget(sequenceList);
        sequenceLayout->addLayout(sequenceBtnLayout);
        sequenceLayout->addStretch(1);
        tabWidget->addTab(sequenceTab, "读入序列");
        
        // 为读入序列标签页添加按钮功能
        int sequenceCurrentIndex = 0;
        QObject::connect(sequencePrev, &QPushButton::clicked, [sequenceList, &sequenceCurrentIndex]() {
            if (sequenceCurrentIndex > 0) {
                sequenceCurrentIndex--;
                sequenceList->setCurrentRow(sequenceCurrentIndex);
            }
        });
        QObject::connect(sequenceNext, &QPushButton::clicked, [sequenceList, &sequenceCurrentIndex, sequenceItems]() {
            if (sequenceCurrentIndex < sequenceItems.size() - 1) {
                sequenceCurrentIndex++;
                sequenceList->setCurrentRow(sequenceCurrentIndex);
            }
        });

        // 手术计划标签页
        QWidget *planTab = new QWidget();
        QHBoxLayout *planLayout = new QHBoxLayout(planTab);
        QListWidget *planList = new QListWidget();
        QStringList planItems = {"三维重建", "标记点", "路径规划"};
        planList->addItems(planItems);
        planList->setFixedWidth(150);
        planList->setCurrentRow(0); // 默认选中第一项
        QPushButton *planPrev = new QPushButton("上一步");
        QPushButton *planNext = new QPushButton("下一步");
        QVBoxLayout *planBtnLayout = new QVBoxLayout();
        planBtnLayout->addWidget(planPrev);
        planBtnLayout->addWidget(planNext);
        planBtnLayout->setAlignment(Qt::AlignTop);
        planLayout->addWidget(planList);
        planLayout->addLayout(planBtnLayout);
        planLayout->addStretch(1);
        tabWidget->addTab(planTab, "手术计划");

        // 为手术计划标签页添加按钮功能
        int planCurrentIndex = 0;
        QObject::connect(planPrev, &QPushButton::clicked, [planList, &planCurrentIndex]() {
            if (planCurrentIndex > 0) {
                planCurrentIndex--;
                planList->setCurrentRow(planCurrentIndex);
            }
        });
        QObject::connect(planNext, &QPushButton::clicked, [planList, &planCurrentIndex, planItems]() {
            if (planCurrentIndex < planItems.size() - 1) {
                planCurrentIndex++;
                planList->setCurrentRow(planCurrentIndex);
            }
        });

        // 系统设置标签页
        QWidget *settingsTab = new QWidget();
        QHBoxLayout *settingsLayout = new QHBoxLayout(settingsTab);
        QListWidget *settingsList = new QListWidget();
        QStringList settingsItems = {"设备调试", "注册", "验证点"};
        settingsList->addItems(settingsItems);
        settingsList->setFixedWidth(150);
        settingsList->setCurrentRow(0); // 默认选中第一项
        QPushButton *settingsPrev = new QPushButton("上一步");
        QPushButton *settingsNext = new QPushButton("下一步");
        QVBoxLayout *settingsBtnLayout = new QVBoxLayout();
        settingsBtnLayout->addWidget(settingsPrev);
        settingsBtnLayout->addWidget(settingsNext);
        settingsBtnLayout->setAlignment(Qt::AlignTop);
        settingsLayout->addWidget(settingsList);
        settingsLayout->addLayout(settingsBtnLayout);
        settingsLayout->addStretch(1);
        tabWidget->addTab(settingsTab, "系统设置");

        // 为系统设置标签页添加按钮功能
        int settingsCurrentIndex = 0;
        QObject::connect(settingsPrev, &QPushButton::clicked, [settingsList, &settingsCurrentIndex]() {
            if (settingsCurrentIndex > 0) {
                settingsCurrentIndex--;
                settingsList->setCurrentRow(settingsCurrentIndex);
            }
        });
        QObject::connect(settingsNext, &QPushButton::clicked, [settingsList, &settingsCurrentIndex, settingsItems]() {
            if (settingsCurrentIndex < settingsItems.size() - 1) {
                settingsCurrentIndex++;
                settingsList->setCurrentRow(settingsCurrentIndex);
            }
        });

        // 术中导航标签页
        QWidget *navigationTab = new QWidget();
        QHBoxLayout *navigationLayout = new QHBoxLayout(navigationTab);
        QListWidget *navigationList = new QListWidget();
        QStringList navigationItems = {"标准模式", "轨迹模式", "前视模式"};
        navigationList->addItems(navigationItems);
        navigationList->setFixedWidth(150);
        navigationList->setCurrentRow(0); // 默认选中第一项
        QPushButton *navigationPrev = new QPushButton("上一步");
        QPushButton *navigationNext = new QPushButton("下一步");
        QVBoxLayout *navigationBtnLayout = new QVBoxLayout();
        navigationBtnLayout->addWidget(navigationPrev);
        navigationBtnLayout->addWidget(navigationNext);
        navigationBtnLayout->setAlignment(Qt::AlignTop);
        navigationLayout->addWidget(navigationList);
        navigationLayout->addLayout(navigationBtnLayout);
        navigationLayout->addStretch(1);
        tabWidget->addTab(navigationTab, "术中导航");

        // 为术中导航标签页添加按钮功能
        int navigationCurrentIndex = 0;
        QObject::connect(navigationPrev, &QPushButton::clicked, [navigationList, &navigationCurrentIndex]() {
            if (navigationCurrentIndex > 0) {
                navigationCurrentIndex--;
                navigationList->setCurrentRow(navigationCurrentIndex);
            }
        });
        QObject::connect(navigationNext, &QPushButton::clicked, [navigationList, &navigationCurrentIndex, navigationItems]() {
            if (navigationCurrentIndex < navigationItems.size() - 1) {
                navigationCurrentIndex++;
                navigationList->setCurrentRow(navigationCurrentIndex);
            }
        });

        // 存储标签页
        QWidget *storageTab = new QWidget();
        QHBoxLayout *storageLayout = new QHBoxLayout(storageTab);
        QListWidget *storageList = new QListWidget();
        QStringList storageItems = {"保存图像", "数据备份", "退出系统"};
        storageList->addItems(storageItems);
        storageList->setFixedWidth(150);
        storageList->setCurrentRow(0); // 默认选中第一项
        QPushButton *storagePrev = new QPushButton("上一步");
        QPushButton *storageNext = new QPushButton("下一步");
        QVBoxLayout *storageBtnLayout = new QVBoxLayout();
        storageBtnLayout->addWidget(storagePrev);
        storageBtnLayout->addWidget(storageNext);
        storageBtnLayout->setAlignment(Qt::AlignTop);
        storageLayout->addWidget(storageList);
        storageLayout->addLayout(storageBtnLayout);
        storageLayout->addStretch(1);
        tabWidget->addTab(storageTab, "存储");

        // 为存储标签页添加按钮功能
        int storageCurrentIndex = 0;
        QObject::connect(storagePrev, &QPushButton::clicked, [storageList, &storageCurrentIndex]() {
            if (storageCurrentIndex > 0) {
                storageCurrentIndex--;
                storageList->setCurrentRow(storageCurrentIndex);
            }
        });
        QObject::connect(storageNext, &QPushButton::clicked, [storageList, &storageCurrentIndex, storageItems]() {
            if (storageCurrentIndex < storageItems.size() - 1) {
                storageCurrentIndex++;
                storageList->setCurrentRow(storageCurrentIndex);
            }
        });

        return tabWidget;
    }

    QWidget* createImageArea() {
        QWidget *container = new QWidget();
        QHBoxLayout *layout = new QHBoxLayout(container);

        // 左侧三维视图
        QLabel *view3D = new QLabel("三维模型视图");
        view3D->setAlignment(Qt::AlignCenter);

        // 右侧多平面视图
        QSplitter *splitter = new QSplitter(Qt::Vertical);
        
        QVector<QPair<QString, QString>> views = {
            {"冠状面", "#000000"},
            {"矢状面", "#000000"},
            {"水平面", "#000000"},
            {"三维辅助视图", "#000000"}
        };

        for (const auto &view : views) {
            QLabel *label = new QLabel(view.first);
            label->setAlignment(Qt::AlignCenter);
            splitter->addWidget(label);
        }

        layout->addWidget(view3D, 3);
        layout->addWidget(splitter, 2);

        return container;
    }
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // 设置全局样式
    QString globalStyle = R"(
        QMainWindow {
            background-color: #E6F4FF;
        }
        QTabWidget::pane {
            border: 1px solid #1C6EA4;
            background-color: #FFFFFF;
        }
        QTabBar::tab {
            background: #1C6EA4;
            color: #FFFFFF;
            padding: 8px;
            border: 1px solid #1C6EA4;
        }
        QTabBar::tab:selected {
            background: #2A8FBD;
            color: #0000FF;
        }
        QListWidget {
            background-color: #1C6EA4;
            color: #FFFFFF;
            border: 1px solid #2A8FBD;
        }
        QPushButton {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #1C6EA4, stop:1 #2A8FBD);
            color: #FFFFFF;
            border: 1px solid #000000;
            padding: 5px;
            min-width: 80px;
        }
        QPushButton:hover {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #2A8FBD, stop:1 #1C6EA4);
        }
        QLabel {
            color: #000000;
        }
        QSplitter {
            background-color: #FFFFFF;
        }
        QListWidget::item:selected {
            border: 2px solid #00BFFF;
            background-color: #1C6EA4;
            color: #FFFFFF;
        }
        QListWidget::item:hover {
            background-color: #2A8FBD;
        }
    )";
    app.setStyleSheet(globalStyle);

    NavigationSystemUI window;
    window.show();
    return app.exec();
}