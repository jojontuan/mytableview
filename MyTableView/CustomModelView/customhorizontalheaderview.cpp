#include <QDebug>
#include <QLabel>
#include <QTableView>
#include "customhorizontalheaderview.h"

const int HEADER_RIGHT_BORDER = 1;

UserHead:: UserHead(QString head_name,QString column_alias,AbstractFilter* filter)
    :m_head_name(head_name),m_column_alias(column_alias),m_filter(filter)
{}
CustomHorizontalHeaderView::CustomHorizontalHeaderView(QWidget *parent)
    : QHeaderView(Qt::Horizontal, parent)
{
    setSectionsMovable(true);
    setSectionsClickable(true);
    this->setStretchLastSection(false);
    this->setObjectName("CustomHorizontalHeaderView");

    connect(this, &QHeaderView::sectionResized, this, &CustomHorizontalHeaderView::handleSectionResized);
    connect(this, &QHeaderView::sectionMoved, this, &CustomHorizontalHeaderView::handleSectionMoved);




    m_head_height = 0;
    m_head_height_has_filter = 0;
    m_is_filter = false;

}
CustomHorizontalHeaderView::~CustomHorizontalHeaderView()
{
    for (auto tableFilter : m_pTableFilterList) {
        delete tableFilter;
        tableFilter = nullptr;
    }

}

void CustomHorizontalHeaderView::setModel(QAbstractItemModel *model)
{
    if (m_headDatas.size() == 0)
    {
        return;
    }
    QHeaderView::setModel(model);
    int t_height = 0;
    for (int i = 0; i < m_headDatas.size(); ++i) {
        auto pTableFilter = new CustomHeader(i,m_headDatas.at(i).m_filter, this);
        connect(pTableFilter, SIGNAL(sort_data(int ,int)),
                this, SLOT(trigger_sort(int,int)));
        pTableFilter->setTitle(this->model()->headerData(i, Qt::Horizontal).toString());

        m_pTableFilterList.append(pTableFilter);
        m_head_height = pTableFilter->get_head_title_height();
        t_height = pTableFilter->get_head_height();
        m_head_height_has_filter = m_head_height_has_filter > t_height ?
                                   m_head_height_has_filter: t_height;
    }
    m_head_height += height();
    m_head_height_has_filter += height();

    setFixedHeight(m_head_height);
}
void CustomHorizontalHeaderView::trigger_sort(int index,int sort_flag)
{

    for (int i = 0; i < m_pTableFilterList.length(); ++i) {
        if(i == index)
        {
            continue;
        }
        m_pTableFilterList.at(i)->clearStatus();
    }
     emit this->sort_data(index,sort_flag);

}
void CustomHorizontalHeaderView::headerDataChanged(Qt::Orientation orientation, int logicalFirst, int logicalLast)
{
    if(logicalFirst < 0 || logicalLast > m_pTableFilterList.length()){return;}
    if(orientation == Qt::Horizontal){
        for (int i=logicalFirst; i<=logicalLast; ++i) {
            m_pTableFilterList.at(i)->setTitle(this->model()->headerData(i, Qt::Horizontal).toString());
        }
    }
}
void CustomHorizontalHeaderView::paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const
{
    // 屏蔽默认绘制表头，避免修改model, model 可以不更改情况下共享给不同的view
    // 表头内容都在showEvent中处理，目前没有其他更好方法
    Q_UNUSED(rect);
    painter->save();
    QHeaderView::paintSection(painter, QRect(), logicalIndex);
    painter->restore();
    auto pTableFilter = m_pTableFilterList.at(logicalIndex);
    pTableFilter->setGeometry(sectionViewportPosition(logicalIndex), 0,
                              sectionSize(logicalIndex) - HEADER_RIGHT_BORDER, height());
    pTableFilter->show();
    // 根据第一个可见视图索引求出逻辑索引, 在这之前的都要隐藏
    int startShowIndex = this->visualIndexAt(0);
    for (int i=0; i<startShowIndex; ++i) {
        m_pTableFilterList.at(i)->hide();
    }
}

void CustomHorizontalHeaderView::handleSectionResized(int i, int oldSize, int newSize)
{
    Q_UNUSED(i)
    Q_UNUSED(oldSize);
    Q_UNUSED(newSize);
    this->fixSectionPositions();
}
void CustomHorizontalHeaderView::handleSectionMoved(int logical, int oldVisualIndex, int newVisualIndex)
{
    Q_UNUSED(logical);
    Q_UNUSED(oldVisualIndex);
    Q_UNUSED(newVisualIndex);
    this->fixSectionPositions();
}
void CustomHorizontalHeaderView::resizeEvent(QResizeEvent *event)
{
    QHeaderView::resizeEvent(event);
    this->fixSectionPositions();
}

void CustomHorizontalHeaderView::fixSectionPositions()
{
    for (int i = 0; i<m_pTableFilterList.length();i++){
        m_pTableFilterList.at(i)->setGeometry(sectionViewportPosition(i), 0,
                                              sectionSize(i) - HEADER_RIGHT_BORDER, height());
    }
}
void CustomHorizontalHeaderView::showFilter()
{
    for (int i = 0; i< m_pTableFilterList.length();i++)
     {
        auto pTableFilter = m_pTableFilterList.at(i);
        if (m_is_filter) //当前显示，则点击隐藏
        {
            pTableFilter->hide_filter(false);
            setFixedHeight(m_head_height);
        }
        else
        {

            pTableFilter->hide_filter(true);
            setFixedHeight(m_head_height_has_filter);

        }
     }

     m_is_filter = !m_is_filter;

}
bool CustomHorizontalHeaderView::isFilter()
{
    return m_is_filter;
}
void CustomHorizontalHeaderView::setHeadDatas(const QVector<UserHead>& headDatas)
{
    m_headDatas = headDatas;
}

void CustomHorizontalHeaderView::getFilterContents(QVector<QString>& filters)
{
    filters.clear();
    if(!m_is_filter)
    {
        return;
    }
    for (CustomHeader* tableFilter : m_pTableFilterList) {
         filters.push_back(tableFilter->getFilterMsg());
    }
}
