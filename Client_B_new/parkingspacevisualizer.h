#ifndef PARKINGSPACEVISUALIZER_H
#define PARKINGSPACEVISUALIZER_H

#include <QDialog>
#include <QJsonObject>
#include <QJsonArray>
#include <QMap>

class ParkingSpaceVisualizer : public QDialog
{
    Q_OBJECT

public:
    explicit ParkingSpaceVisualizer(QWidget *parent = nullptr);
    ~ParkingSpaceVisualizer();

    // 设置停车位数据
    void setParkingSpaces(const QJsonArray &spaces);

protected:
    // 重写绘图事件
    void paintEvent(QPaintEvent *event) override;
    // 重写鼠标点击事件
    void mousePressEvent(QMouseEvent *event) override;
    // 重写窗口大小变化事件
    void resizeEvent(QResizeEvent *event) override;

private:
    // 停车位数据
    QJsonArray m_spaces;
    // 停车位矩形映射 (ID -> 矩形)
    QMap<int, QRect> m_spaceRects;
    
    // 计算停车位位置
    void calculateSpacePositions();
    // 显示停车位详细信息
    void showSpaceDetails(int spaceId);
    // 计算停车时长和费用
    QString calculateParkingInfo(const QJsonObject &spaceObj);
};

#endif // PARKINGSPACEVISUALIZER_H
