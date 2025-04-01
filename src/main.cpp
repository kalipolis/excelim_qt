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

        // 创建任务栏版面
        QWidget *taskPanel = createTaskPanel();
        mainLayout->addWidget(taskPanel);

        // 创建图像显示区域
        QWidget *imagePanel = createImagePanel();
        mainLayout->addWidget(imagePanel, 1);

        setCentralWidget(mainWidget);
    }

private:
    QWidget* createTaskPanel() {
        QWidget *container = new QWidget();
        QVBoxLayout *layout = new QVBoxLayout(container);

        // 创建菜单标签页
        QTabWidget *tabWidget = new QTabWidget();
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
        QObject::connect(globalPrev, &QPushButton::clicked, [tabWidget]() {
            int current = tabWidget->currentIndex();
            if (current > 0) {
                tabWidget->setCurrentIndex(current - 1);
            }
        });

        QObject::connect(globalNext, &QPushButton::clicked, [tabWidget]() {
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

        // 图像显示区域（暂时留空）
        QLabel *imageView = new QLabel("图像显示区域");
        imageView->setAlignment(Qt::AlignCenter);
        imageView->setStyleSheet("background-color: #FFFFFF; border: 1px solid #1C6EA4;");
        imageView->setMinimumSize(800, 400);
        layout->addWidget(imageView);

        return container;
    }

    // 创建各个菜单标签页
    void createDataTab(QTabWidget *tabWidget) {
        QWidget *tab = new QWidget();
        QVBoxLayout *layout = new QVBoxLayout(tab);
        layout->addWidget(new QLabel("数据管理功能区域"));
        tabWidget->addTab(tab, "数据管理");
    }

    void createSequenceTab(QTabWidget *tabWidget) {
        QWidget *tab = new QWidget();
        QVBoxLayout *layout = new QVBoxLayout(tab);
        layout->addWidget(new QLabel("序列管理功能区域"));
        tabWidget->addTab(tab, "序列管理");
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