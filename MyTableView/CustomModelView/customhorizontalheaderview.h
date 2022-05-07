#ifndef CUSTOMHORIZONTALHEADERVIEW_H
#define CUSTOMHORIZONTALHEADERVIEW_H

#include <QObject>
#include <QHeaderView>
#include <QPainter>

#include "customheader.h"
class UserHead
{
public:
    UserHead(QString head_name,QString column_alias,AbstractFilter* filter);
    UserHead()
    {}
    UserHead(const UserHead& head)
    {
        m_head_name = head.m_head_name;
        m_column_alias = head.m_column_alias;
        m_filter = head.m_filter;
    }
    QString m_head_name;
    QString m_column_alias;
    AbstractFilter *m_filter;

};
class CustomHorizontalHeaderView : public QHeaderView
{
    Q_OBJECT
public:
    explicit CustomHorizontalHeaderView(QWidget *parent = nullptr);
    virtual ~CustomHorizontalHeaderView() override;
    void showFilter();
    bool isFilter();
    void setHeadDatas(const QVector<UserHead> &headDatas);
    void getFilterContents(QVector<QString>& filters);
signals:
    void sort_data(int index,int sort_flag);

public slots:
    void trigger_sort(int index,int sort_flag);
protected:
    void paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const override;
    void resizeEvent(QResizeEvent *event) override;
    void handleSectionMoved(int logical, int oldVisualIndex, int newVisualIndex);
    void handleSectionResized(int i, int oldSize, int newSize);
    void setModel(QAbstractItemModel *model) override;
    void headerDataChanged(Qt::Orientation orientation, int logicalFirst, int logicalLast);
    void fixSectionPositions();

private:
    QVector<CustomHeader*> m_pTableFilterList;
    QVector<UserHead> m_headDatas;//提供给应用设置的属性
    int m_head_height;
    int m_head_height_has_filter;
    bool m_is_filter;

};

#endif // CUSTOMHORIZONTALHEADERVIEW_H
