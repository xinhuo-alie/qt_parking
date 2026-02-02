#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDebug>
#include <QJsonArray>
#include <QStyleFactory>
#include <QApplication>
#include "parkingspacevisualizer.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_networkManager(new NetworkManager(this)),
    m_parkingSpaceManager(new ParkingSpaceManager(m_networkManager, this))
{
    ui->setupUi(this);
    initUI();
    // 手动连接删除按钮的点击信号到槽函数
    connect(ui->btnDeleteParkingSpace, &QPushButton::clicked, this, &MainWindow::handleDeleteParkingSpace);
}

MainWindow::~MainWindow()
{
    delete m_parkingSpaceManager;
    delete m_networkManager;
    delete ui;
}

void MainWindow::initUI()
{
    // 设置窗口标题
    this->setWindowTitle("停车场管理系统");
    
    // 设置样式表
    QString styleSheet = "" 
        "/* 主窗口背景 - 使用渐变色 */ " 
        "QMainWindow { " 
        "    background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #f5f5f5, stop:1 #e0e0e0); " 
        "    color: #333333; " 
        "    font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; " 
        "}" 
        "" 
        "/* 组框样式 - 不同功能组使用不同主题色 */ " 
        "QGroupBox { " 
        "    font: bold 13px; " 
        "    border: 2px solid #4a90e2; " 
        "    border-radius: 10px; " 
        "    margin-top: 8px; " 
        "    background-color: #ffffff; " 
        "    box-shadow: 0 2px 4px rgba(0, 0, 0, 0.1); " 
        "}" 
        "" 
        "/* 添加停车位组 - 蓝色主题 */ " 
        "QGroupBox[name='groupBoxAdd'] { " 
        "    border-color: #4a90e2; " 
        "}" 
        "QGroupBox[name='groupBoxAdd']::title { " 
        "    color: #4a90e2; " 
        "}" 
        "" 
        "/* 获取单个停车位组 - 绿色主题 */ " 
        "QGroupBox[name='groupBoxGet'] { " 
        "    border-color: #5cb85c; " 
        "}" 
        "QGroupBox[name='groupBoxGet']::title { " 
        "    color: #5cb85c; " 
        "}" 
        "" 
        "/* 更新停车位组 - 橙色主题 */ " 
        "QGroupBox[name='groupBoxUpdate'] { " 
        "    border-color: #f0ad4e; " 
        "}" 
        "QGroupBox[name='groupBoxUpdate']::title { " 
        "    color: #f0ad4e; " 
        "}" 
        "" 
        "/* 删除停车位组 - 红色主题 */ " 
        "QGroupBox[name='groupBoxDelete'] { " 
        "    border-color: #d9534f; " 
        "}" 
        "QGroupBox[name='groupBoxDelete']::title { " 
        "    color: #d9534f; " 
        "}" 
        "" 
        "/* 获取所有停车位组 - 紫色主题 */ " 
        "QGroupBox[name='groupBoxGetAll'] { " 
        "    border-color: #9b59b6; " 
        "}" 
        "QGroupBox[name='groupBoxGetAll']::title { " 
        "    color: #9b59b6; " 
        "}" 
        "" 
        "QGroupBox::title { " 
        "    subcontrol-origin: margin; " 
        "    left: 10px; " 
        "    padding: 0 8px 0 8px; " 
        "    font-weight: bold; " 
        "}" 
        "" 
        "/* 标签样式 - 增强可读性 */ " 
        "QLabel { " 
        "    font: 12px; " 
        "    color: #333333; " 
        "    padding: 2px 0; " 
        "}" 
        "" 
        "/* 输入框样式 - 增强视觉效果 */ " 
        "QLineEdit { " 
        "    font: 12px; " 
        "    padding: 7px 10px; " 
        "    border: 1px solid #cccccc; " 
        "    border-radius: 5px; " 
        "    background-color: #ffffff; " 
        "    transition: all 0.2s ease-in-out; " 
        "}" 
        "" 
        "QLineEdit:focus { " 
        "    border-color: #4a90e2; " 
        "    background-color: #f0f8ff; " 
        "    box-shadow: 0 0 0 2px rgba(74, 144, 226, 0.2); " 
        "}" 
        "" 
        "/* 下拉框样式 - 增强视觉效果 */ " 
        "QComboBox { " 
        "    font: 12px; " 
        "    padding: 7px 10px; " 
        "    border: 1px solid #cccccc; " 
        "    border-radius: 5px; " 
        "    background-color: #ffffff; " 
        "    transition: all 0.2s ease-in-out; " 
        "}" 
        "" 
        "QComboBox:focus { " 
        "    border-color: #4a90e2; " 
        "    box-shadow: 0 0 0 2px rgba(74, 144, 226, 0.2); " 
        "}" 
        "" 
        "QComboBox::down-arrow { " 
        "    border-left: 5px solid transparent; " 
        "    border-right: 5px solid transparent; " 
        "    border-top: 5px solid #666; " 
        "}" 
        "" 
        "/* 按钮样式 - 渐变效果和阴影 */ " 
        "QPushButton { " 
        "    font: bold 12px; " 
        "    padding: 8px 20px; " 
        "    border: none; " 
        "    border-radius: 5px; " 
        "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #4a90e2, stop:1 #357abd); " 
        "    color: #ffffff; " 
        "    box-shadow: 0 2px 5px rgba(0, 0, 0, 0.15); " 
        "    transition: all 0.2s ease-in-out; " 
        "}" 
        "" 
        "QPushButton:hover { " 
        "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #357abd, stop:1 #2a6099); " 
        "    box-shadow: 0 3px 7px rgba(0, 0, 0, 0.2); " 
        "    transform: translateY(-1px); " 
        "}" 
        "" 
        "QPushButton:pressed { " 
        "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #2a6099, stop:1 #1e4372); " 
        "    box-shadow: 0 1px 3px rgba(0, 0, 0, 0.2); " 
        "    transform: translateY(0); " 
        "}" 
        "" 
        "/* 功能特定按钮颜色 */ " 
        "QPushButton[name='btnAddParkingSpace'] { " 
        "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #5cb85c, stop:1 #4cae4c); " 
        "}" 
        "" 
        "QPushButton[name='btnAddParkingSpace']:hover { " 
        "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #4cae4c, stop:1 #3d8b40); " 
        "}" 
        "" 
        "QPushButton[name='btnDeleteParkingSpace'] { " 
        "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #d9534f, stop:1 #c9302c); " 
        "}" 
        "" 
        "QPushButton[name='btnDeleteParkingSpace']:hover { " 
        "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #c9302c, stop:1 #ac2925); " 
        "}" 
        "" 
        "QPushButton[name='btnUpdateParkingSpace'] { " 
        "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #f0ad4e, stop:1 #eea236); " 
        "}" 
        "" 
        "QPushButton[name='btnUpdateParkingSpace']:hover { " 
        "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #eea236, stop:1 #d58512); " 
        "}" 
        "" 
        "QPushButton[name='btnVisualizeParkingSpaces'] { " 
        "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #9b59b6, stop:1 #8e44ad); " 
        "}" 
        "" 
        "QPushButton[name='btnVisualizeParkingSpaces']:hover { " 
        "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #8e44ad, stop:1 #7d3c98); " 
        "}" 
        "" 
        "/* 表格样式 - 增强视觉层次 */ " 
        "QTableWidget { " 
        "    font: 12px; " 
        "    border: 1px solid #e0e0e0; " 
        "    border-radius: 8px; " 
        "    background-color: #ffffff; " 
        "    gridline-color: #f0f0f0; " 
        "    selection-background-color: #b0d4f1; " 
        "}" 
        "" 
        "QTableWidget::header { " 
        "    font: bold 12px; " 
        "    background-color: #4a90e2; " 
        "    color: #ffffff; " 
        "    border: none; " 
        "}" 
        "" 
        "QTableWidget::header:section { " 
        "    border: none; " 
        "    padding: 8px 15px; " 
        "    border-right: 1px solid rgba(255, 255, 255, 0.3); " 
        "}" 
        "" 
        "QTableWidget::item { " 
        "    padding: 8px 15px; " 
        "    border-bottom: 1px solid #f0f0f0; " 
        "}" 
        "" 
        "QTableWidget::item:selected { " 
        "    background-color: #b0d4f1; " 
        "    color: #000000; " 
        "}" 
        "" 
        "QTableWidget::item:alternate { " 
        "    background-color: #f9f9f9; " 
        "}" 
        "" 
        "QTableWidget::item:hover { " 
        "    background-color: #f5f5f5; " 
        "}";
    
    // 仅将样式表应用于当前窗口，避免覆盖MainMenu的样式
    this->setStyleSheet(styleSheet);
    
    // 初始化表格
    ui->tableParkingSpaces->setColumnCount(8);
    QStringList headers;
    headers << "ID" << "位置" << "类型" << "小时费率" << "状态" << "状态文本" << "当前车牌" << "占用时间";
    ui->tableParkingSpaces->setHorizontalHeaderLabels(headers);
    ui->tableParkingSpaces->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableParkingSpaces->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableParkingSpaces->setAlternatingRowColors(true);
    ui->tableParkingSpaces->setSortingEnabled(true);
    ui->tableParkingSpaces->setShowGrid(true);
    ui->tableParkingSpaces->verticalHeader()->setVisible(false);
    ui->tableParkingSpaces->horizontalHeader()->setStretchLastSection(true);
}

void MainWindow::showSuccessMessage(const QString &message)
{
    QMessageBox::information(this, "成功", message);
}

void MainWindow::showErrorMessage(const QString &message)
{
    QMessageBox::critical(this, "错误", message);
}

void MainWindow::on_btnAddParkingSpace_clicked()
{
    QString location = ui->editLocation->text();
    QString type = ui->comboType->currentText();
    double hourlyRate = ui->comboHourlyRate->currentText().toDouble();

    if (location.isEmpty()) {
        showErrorMessage("位置不能为空");
        return;
    }

    m_parkingSpaceManager->addParkingSpace(
        location,
        type,
        hourlyRate,
        [this](QJsonObject data) {
            // 确保在UI线程中更新UI
            QMetaObject::invokeMethod(this, [this]() {
                showSuccessMessage("添加停车位成功");
                // 清空输入框
                ui->editLocation->clear();
                ui->comboType->setCurrentIndex(0);
                ui->comboHourlyRate->setCurrentIndex(0);
            });
        },
        [this](QString error) {
            // 确保在UI线程中显示错误信息
            QMetaObject::invokeMethod(this, [this, error]() {
                showErrorMessage("添加停车位失败: " + error);
            });
        }
    );
}

void MainWindow::on_btnGetParkingSpace_clicked()
{
    int id = ui->editGetId->text().toInt();
    if (id <= 0) {
        showErrorMessage("请输入有效的停车位ID");
        return;
    }

    // 添加调试信息
    qDebug() << "正在获取停车位ID: " << id;
    
    m_parkingSpaceManager->getParkingSpace(
        id,
        [this](QJsonObject data) {
            // 确保在UI线程中更新UI
            QMetaObject::invokeMethod(this, [this, data]() {
                // 检查响应数据格式
                qDebug() << "单个停车位API响应: " << data;
                QJsonObject spaceData;
                
                // 与获取列表逻辑一致，检查data字段
                if (data.contains("data")) {
                    qDebug() << "响应包含data字段，类型: " << data["data"].type();
                    if (data["data"].isObject()) {
                        spaceData = data["data"].toObject();
                        qDebug() << "第一层data对象内容: " << spaceData;
                        
                        // 检查是否存在嵌套的data字段（处理API响应的嵌套结构）
                        if (spaceData.contains("data")) {
                            qDebug() << "第一层data包含嵌套的data字段，类型: " << spaceData["data"].type();
                            if (spaceData["data"].isObject()) {
                                spaceData = spaceData["data"].toObject();
                                qDebug() << "第二层data对象内容: " << spaceData;
                            } else if (spaceData["data"].isArray() && spaceData["data"].toArray().size() > 0) {
                                spaceData = spaceData["data"].toArray()[0].toObject();
                                qDebug() << "使用嵌套数组中的第一个对象: " << spaceData;
                            }
                        }
                    } else if (data["data"].isArray() && data["data"].toArray().size() > 0) {
                        // 处理可能的数组情况
                        spaceData = data["data"].toArray()[0].toObject();
                        qDebug() << "使用数组中的第一个对象: " << spaceData;
                    } else {
                        qDebug() << "data字段类型不符合预期";
                        showErrorMessage("获取停车位信息失败: 数据格式错误");
                        return;
                    }
                } else {
                    // 如果没有data字段，直接使用返回的数据
                    spaceData = data;
                    qDebug() << "直接使用返回的数据: " << spaceData;
                }
                
                // 检查spaceData是否为空
                if (spaceData.isEmpty()) {
                    qDebug() << "spaceData为空对象";
                    showErrorMessage("获取停车位信息失败: 未找到停车位数据");
                    return;
                }
                
                // 在表格中显示单个停车位信息
                qDebug() << "开始更新表格";
                ui->tableParkingSpaces->setRowCount(0); // 先清空表格
                ui->tableParkingSpaces->setRowCount(1); // 设置为1行
                
                // 设置表格列数
                ui->tableParkingSpaces->setColumnCount(8);
                QStringList headers;
                headers << "ID" << "位置" << "类型" << "小时费率" << "状态" << "状态文本" << "当前车牌" << "占用时间";
                ui->tableParkingSpaces->setHorizontalHeaderLabels(headers);
                
                // 为了确保表格可见，调整列宽
                ui->tableParkingSpaces->resizeColumnsToContents();
                ui->tableParkingSpaces->setVisible(true);
                
                // 显示数据到控制台以便调试
                qDebug() << "准备显示的数据:";
                qDebug() << "ID: " << (spaceData.contains("id") ? QString::number(spaceData["id"].toInt()) : "");
                qDebug() << "位置: " << (spaceData.contains("location") ? spaceData["location"].toString() : "");
                qDebug() << "类型: " << (spaceData.contains("type") ? spaceData["type"].toString() : "");
                qDebug() << "小时费率: " << (spaceData.contains("hourlyRate") ? QString::number(spaceData["hourlyRate"].toDouble()) : "");
                
                // 添加错误检查，确保字段存在且类型正确
                ui->tableParkingSpaces->setItem(0, 0, new QTableWidgetItem(spaceData.contains("id") ? QString::number(spaceData["id"].toInt()) : ""));
                ui->tableParkingSpaces->setItem(0, 1, new QTableWidgetItem(spaceData.contains("location") ? spaceData["location"].toString() : ""));
                
                // 类型列 - 根据类型显示不同颜色
                QString type = spaceData.contains("type") ? spaceData["type"].toString() : "";
                QTableWidgetItem *typeItem = new QTableWidgetItem(type);
                if (type == "VIP") {
                    typeItem->setBackgroundColor(QColor(237, 201, 255)); // 浅紫色背景 - 表示VIP类型的车位
                    typeItem->setForeground(Qt::darkMagenta);
                }
                ui->tableParkingSpaces->setItem(0, 2, typeItem);
                
                ui->tableParkingSpaces->setItem(0, 3, new QTableWidgetItem(spaceData.contains("hourlyRate") ? QString::number(spaceData["hourlyRate"].toDouble()) : ""));
                
                // 状态列 - 根据状态显示不同颜色
                QString status = spaceData.contains("status") ? spaceData["status"].toString() : "";
                QTableWidgetItem *statusItem = new QTableWidgetItem(status);
                if (status == "available") {
                    statusItem->setBackgroundColor(QColor(187, 255, 153)); // 浅绿色 - 表示该车位当前处于空闲可用状态
                    statusItem->setForeground(Qt::darkGreen);
                } else if (status == "occupied") {
                    statusItem->setBackgroundColor(QColor(255, 178, 178)); // 浅红色 - 表示该车位当前已被占用
                    statusItem->setForeground(Qt::darkRed);
                } else if (status == "maintenance") {
                    statusItem->setBackgroundColor(QColor(255, 241, 153)); // 浅黄色 - 表示该车位当前处于维护状态
                    statusItem->setForeground(Qt::darkYellow);
                }
                ui->tableParkingSpaces->setItem(0, 4, statusItem);
                
                ui->tableParkingSpaces->setItem(0, 5, new QTableWidgetItem(spaceData.contains("statusText") ? spaceData["statusText"].toString() : ""));
                ui->tableParkingSpaces->setItem(0, 6, new QTableWidgetItem(spaceData.contains("currentPlate") ? spaceData["currentPlate"].toString() : ""));
                ui->tableParkingSpaces->setItem(0, 7, new QTableWidgetItem(spaceData.contains("occupiedTime") ? spaceData["occupiedTime"].toString() : ""));
                
                // 再次调整列宽以确保所有内容可见
                ui->tableParkingSpaces->resizeColumnsToContents();
                
                qDebug() << "表格更新完成";
            });
        },
        [this](QString error) {
            // 确保在UI线程中显示错误信息
            QMetaObject::invokeMethod(this, [this, error]() {
                qDebug() << "获取停车位信息错误: " << error;
                showErrorMessage("获取停车位信息失败: " + error);
            });
        }
    );
}

void MainWindow::on_btnGetParkingSpaces_clicked()
{
    int page = 1;  // 使用固定页码1
    int limit = 20;  // 使用固定每页数量20
    
    // 验证页码和每页数量
    if (page <= 0) page = 1;
    if (limit <= 0) limit = 20;

    // 调用接口获取停车位列表
    m_parkingSpaceManager->getParkingSpaces(
        page,
        limit,
        "",  // 不使用状态筛选
        "",  // 不使用类型筛选
        [this](QJsonObject data) {
            // 确保在UI线程中更新UI
            QMetaObject::invokeMethod(this, [this, data]() {
                // 检查响应数据格式
                qDebug() << "完整API响应: " << data;
                qDebug() << "响应包含data字段: " << data.contains("data");
                if (data.contains("data")) {
                    qDebug() << "data字段类型: " << data["data"].type();
                    if (data["data"].isObject()) {
                        QJsonObject tempDataObj = data["data"].toObject();
                        qDebug() << "data对象包含的键: " << tempDataObj.keys();
                        qDebug() << "data对象内容: " << tempDataObj;
                    }
                }
                
                if (!data.contains("data") || !data["data"].isObject()) {
                    showErrorMessage("获取停车位列表失败: 数据格式错误");
                    return;
                }
                
                QJsonObject dataObj = data["data"].toObject();
                QJsonArray spacesArray;
                qDebug() << "dataObj包含spaces字段: " << dataObj.contains("spaces");
                if (dataObj.contains("spaces")) {
                    qDebug() << "spaces字段类型: " << dataObj["spaces"].type();
                }
                qDebug() << "dataObj包含items字段: " << dataObj.contains("items");
                if (dataObj.contains("items")) {
                    qDebug() << "items字段类型: " << dataObj["items"].type();
                }
                
                // 兼容多种格式：
                // 1. spaces（API文档指定）
                // 2. items（通用分页格式）
                // 3. data直接是数组（简化格式）
                if (dataObj.contains("spaces") && dataObj["spaces"].isArray()) {
                    spacesArray = dataObj["spaces"].toArray();
                } else if (dataObj.contains("items") && dataObj["items"].isArray()) {
                    spacesArray = dataObj["items"].toArray();
                } else if (data["data"].isArray()) {
                    // 特殊情况：data本身就是数组
                    spacesArray = data["data"].toArray();
                } else {
                    // 尝试查找其他可能的数组字段
                    QStringList arrayFields;
                    for (const QString &key : dataObj.keys()) {
                        if (dataObj[key].isArray()) {
                            arrayFields << key;
                        }
                    }
                    
                    if (!arrayFields.isEmpty()) {
                        // 使用找到的第一个数组字段
                        spacesArray = dataObj[arrayFields.first()].toArray();
                        qDebug() << "使用备用数组字段: " << arrayFields.first();
                    } else {
                        showErrorMessage("获取停车位列表失败: 缺少有效的数据数组字段");
                        return;
                    }
                }
                
                // 清空表格
                ui->tableParkingSpaces->setRowCount(0);
                
                // 设置表格列数
                ui->tableParkingSpaces->setColumnCount(8);
                QStringList headers;
                headers << "ID" << "位置" << "类型" << "小时费率" << "状态" << "状态文本" << "当前车牌" << "占用时间";
                ui->tableParkingSpaces->setHorizontalHeaderLabels(headers);
                
                // 填充表格数据
                for (int i = 0; i < spacesArray.size(); ++i) {
                    QJsonObject spaceObj = spacesArray[i].toObject();
                    ui->tableParkingSpaces->insertRow(i);
                    
                    // 添加错误检查，确保字段存在且类型正确
                    ui->tableParkingSpaces->setItem(i, 0, new QTableWidgetItem(spaceObj.contains("id") ? QString::number(spaceObj["id"].toInt()) : ""));
                    ui->tableParkingSpaces->setItem(i, 1, new QTableWidgetItem(spaceObj.contains("location") ? spaceObj["location"].toString() : ""));
                    
                    // 类型列 - 根据类型显示不同颜色
                    QTableWidgetItem *typeItem = new QTableWidgetItem(spaceObj.contains("type") ? spaceObj["type"].toString() : "");
                    if (spaceObj.contains("type") && spaceObj["type"].toString() == "VIP") {
                        typeItem->setBackgroundColor(QColor(237, 201, 255)); // 浅紫色背景 - 表示VIP类型的车位
                        typeItem->setForeground(Qt::darkMagenta);
                    }
                    ui->tableParkingSpaces->setItem(i, 2, typeItem);
                    
                    // 小时费率
                    ui->tableParkingSpaces->setItem(i, 3, new QTableWidgetItem(spaceObj.contains("hourlyRate") ? QString::number(spaceObj["hourlyRate"].toDouble()) : ""));
                    
                    // 状态列 - 根据状态显示不同颜色
                    QString status = spaceObj.contains("status") ? spaceObj["status"].toString() : "";
                    QTableWidgetItem *statusItem = new QTableWidgetItem(status);
                    if (status == "available") {
                        statusItem->setBackgroundColor(QColor(187, 255, 153)); // 浅绿色 - 表示该车位当前处于空闲可用状态
                        statusItem->setForeground(Qt::darkGreen);
                    } else if (status == "occupied") {
                        statusItem->setBackgroundColor(QColor(255, 178, 178)); // 浅红色 - 表示该车位当前已被占用
                        statusItem->setForeground(Qt::darkRed);
                    } else if (status == "maintenance") {
                        statusItem->setBackgroundColor(QColor(255, 241, 153)); // 浅黄色 - 表示该车位当前处于维护状态
                        statusItem->setForeground(Qt::darkYellow);
                    }
                    ui->tableParkingSpaces->setItem(i, 4, statusItem);
                    
                    ui->tableParkingSpaces->setItem(i, 5, new QTableWidgetItem(spaceObj.contains("statusText") ? spaceObj["statusText"].toString() : ""));
                    ui->tableParkingSpaces->setItem(i, 6, new QTableWidgetItem(spaceObj.contains("currentPlate") ? spaceObj["currentPlate"].toString() : ""));
                    ui->tableParkingSpaces->setItem(i, 7, new QTableWidgetItem(spaceObj.contains("occupiedTime") ? spaceObj["occupiedTime"].toString() : ""));
                }
                
                showSuccessMessage("获取停车位列表成功");
            });
        },
        [this](QString error) {
            // 确保在UI线程中显示错误信息
            QMetaObject::invokeMethod(this, [this, error]() {
                showErrorMessage("获取停车位列表失败: " + error);
            });
        }
    );
}

void MainWindow::on_btnVisualizeParkingSpaces_clicked()
{
    // 获取所有停车位数据（设置较大的limit以获取所有数据）
    m_parkingSpaceManager->getParkingSpaces(
        1,
        100,  // 设置一个较大的limit以获取所有停车位
        "",   // 不使用状态筛选
        "",   // 不使用类型筛选
        [this](QJsonObject data) {
            // 确保在UI线程中更新UI
            QMetaObject::invokeMethod(this, [this, data]() {
                // 检查响应数据格式
                qDebug() << "完整API响应(可视化): " << data;
                qDebug() << "响应包含data字段(可视化): " << data.contains("data");
                if (data.contains("data")) {
                    qDebug() << "data字段类型(可视化): " << data["data"].type();
                    if (data["data"].isObject()) {
                        QJsonObject tempDataObj = data["data"].toObject();
                        qDebug() << "data对象包含的键(可视化): " << tempDataObj.keys();
                        qDebug() << "data对象内容(可视化): " << tempDataObj;
                    }
                }
                
                if (!data.contains("data") || !data["data"].isObject()) {
                    showErrorMessage("获取停车位列表失败: 数据格式错误");
                    return;
                }
                
                QJsonObject dataObj = data["data"].toObject();
                QJsonArray spacesArray;
                qDebug() << "dataObj包含spaces字段(可视化): " << dataObj.contains("spaces");
                if (dataObj.contains("spaces")) {
                    qDebug() << "spaces字段类型(可视化): " << dataObj["spaces"].type();
                }
                qDebug() << "dataObj包含items字段(可视化): " << dataObj.contains("items");
                if (dataObj.contains("items")) {
                    qDebug() << "items字段类型(可视化): " << dataObj["items"].type();
                }
                
                // 兼容多种格式：
                // 1. spaces（API文档指定）
                // 2. items（通用分页格式）
                // 3. data直接是数组（简化格式）
                if (dataObj.contains("spaces") && dataObj["spaces"].isArray()) {
                    spacesArray = dataObj["spaces"].toArray();
                } else if (dataObj.contains("items") && dataObj["items"].isArray()) {
                    spacesArray = dataObj["items"].toArray();
                } else if (data["data"].isArray()) {
                    // 特殊情况：data本身就是数组
                    spacesArray = data["data"].toArray();
                } else {
                    // 尝试查找其他可能的数组字段
                    QStringList arrayFields;
                    for (const QString &key : dataObj.keys()) {
                        if (dataObj[key].isArray()) {
                            arrayFields << key;
                        }
                    }
                    
                    if (!arrayFields.isEmpty()) {
                        // 使用找到的第一个数组字段
                        spacesArray = dataObj[arrayFields.first()].toArray();
                        qDebug() << "使用备用数组字段(可视化): " << arrayFields.first();
                    } else {
                        showErrorMessage("获取停车位列表失败: 缺少有效的数据数组字段");
                        return;
                    }
                }
                
                // 创建并显示可视化窗口（非模态）
                ParkingSpaceVisualizer *visualizer = new ParkingSpaceVisualizer(this);
                visualizer->setParkingSpaces(spacesArray);
                visualizer->setAttribute(Qt::WA_DeleteOnClose); // 关闭时自动删除
                visualizer->setWindowFlags(Qt::Window); // 设置为独立窗口
                
                // 优化窗口位置（居中显示）
                visualizer->move(
                    this->x() + (this->width() - visualizer->width()) / 2,
                    this->y() + (this->height() - visualizer->height()) / 2
                );
                
                visualizer->show();
            });
        },
        [this](QString error) {
            // 确保在UI线程中显示错误信息
            QMetaObject::invokeMethod(this, [this, error]() {
                showErrorMessage("获取停车位列表失败: " + error);
            });
        }
    );
}

void MainWindow::on_btnUpdateParkingSpace_clicked()
{
    int id = ui->editUpdateId->text().toInt();
    if (id <= 0) {
        showErrorMessage("请输入有效的停车位ID");
        return;
    }

    QJsonObject updateData;
    QString updateStatus = ui->comboUpdateStatus->currentText();
    // 添加调试信息，查看更新时的status值
    qDebug() << "更新时的status值: " << updateStatus;
    
    // 清理和验证status值
    updateStatus = updateStatus.trimmed(); // 移除前后空格
    
    // 确保status值的正确性
    if (updateStatus != "available" && updateStatus != "occupied" && updateStatus != "maintenance") {
        qDebug() << "发现无效的status值: " << updateStatus << "，将其重置为available";
        updateStatus = "available";
    }
    
    qDebug() << "将清理后的status添加到更新数据中: " << updateStatus;
    updateData["status"] = updateStatus;
    
    // 添加调试信息，查看最终构造的updateData
    qDebug() << "最终构造的updateData: " << updateData;
    qDebug() << "updateData包含的键: " << updateData.keys();
    for (const QString &key : updateData.keys()) {
        qDebug() << "键" << key << "的值: " << updateData[key] << "，类型: " << updateData[key].type();
    }

    if (updateData.isEmpty()) {
        showErrorMessage("请至少修改一项信息");
        return;
    }

    m_parkingSpaceManager->updateParkingSpace(
        id,
        updateData,
        [this](QJsonObject data) {
            // 确保在UI线程中更新UI
            QMetaObject::invokeMethod(this, [this]() {
                showSuccessMessage("更新停车位成功");
                // 清空输入框
                ui->editUpdateId->clear();
                ui->comboUpdateStatus->setCurrentIndex(0);

            });
        },
        [this](QString error) {
            // 确保在UI线程中显示错误信息
            QMetaObject::invokeMethod(this, [this, error]() {
                showErrorMessage("更新停车位失败: " + error);
            });
        }
    );
}

void MainWindow::handleDeleteParkingSpace()
{
    qDebug() << "handleDeleteParkingSpace() called";
    int id = ui->editDeleteId->text().toInt();
    if (id <= 0) {
        showErrorMessage("请输入有效的停车位ID");
        return;
    }

    if (QMessageBox::question(this, "确认删除", "确定要删除该停车位吗？") != QMessageBox::Yes) {
        return;
    }

    m_parkingSpaceManager->deleteParkingSpace(
        id,
        [this](QJsonObject data) {
            // 确保在UI线程中更新UI
            QMetaObject::invokeMethod(this, [this]() {
                showSuccessMessage("删除停车位成功");
                // 清空输入框
                ui->editDeleteId->clear();

            });
        },
        [this](QString error) {
            // 确保在UI线程中显示错误信息
            QMetaObject::invokeMethod(this, [this, error]() {
                showErrorMessage("删除停车位失败: " + error);
            });
        }
    );
}

void MainWindow::on_tableParkingSpaces_itemClicked(QTableWidgetItem *item)
{
    // 获取点击的行号
    int row = item->row();
    
    // 从表格中获取数据
    QString id = ui->tableParkingSpaces->item(row, 0)->text();
    QString location = ui->tableParkingSpaces->item(row, 1)->text();
    QString type = ui->tableParkingSpaces->item(row, 2)->text();
    QString hourlyRate = ui->tableParkingSpaces->item(row, 3)->text();
    QString status = ui->tableParkingSpaces->item(row, 4)->text();
    
    // 填充到删除信息栏
    ui->editDeleteId->setText(id);
    
    // 填充到更新信息栏
    ui->editUpdateId->setText(id);
    
    // 设置状态下拉框
    int statusIndex = ui->comboUpdateStatus->findText(status);
    if (statusIndex >= 0) {
        ui->comboUpdateStatus->setCurrentIndex(statusIndex);
    } else {
        ui->comboUpdateStatus->setCurrentIndex(0); // 默认为"保持不变"
    }
}


    



