#ifndef CUSTOMHEADERVIEW_H
#define CUSTOMHEADERVIEW_H

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include<QHBoxLayout>
#include<QLabel>
namespace Ui {
class CustomHeader;
}

class AbstractFilter;
enum SortModel
{
  NONE_SORT,
  ASCENDING_SORT,
  DESCENDING_SORT

};
/*
 * 自定义横向表头内容框
 */
class CustomHeader : public QWidget
{
    Q_OBJECT

public:
    explicit CustomHeader(int index,
                              AbstractFilter* filter = nullptr,
                              QWidget *parent = nullptr);
    ~CustomHeader();

    void setTitle(QString text);
    QString title();
    void setAlignment(Qt::Alignment align);

    void clearStatus();
    void clearFilterStatus();
    QString getFilterMsg();
    int get_head_title_height();
    int get_head_height();
    void hide_filter(bool hidden);

signals:
    void sort_data(int index,int sort_flag);

public slots:
    void trigger_sort();

protected:
    virtual bool eventFilter(QObject *obj, QEvent *event);

private:
    Ui::CustomHeader *ui;
    // 列索引
    int m_index = 0;

    int m_sort_flag = NONE_SORT ; // 0 不排序 1升序 2 降序
    QPushButton * m_label_title;
    AbstractFilter * m_filter;
};




class AbstractFilter:public QWidget
{
    Q_OBJECT
 public:
    virtual QString filterMsg() = 0;
    virtual int get_height() = 0;

};

class EditFilter : public AbstractFilter
{
    Q_OBJECT

public:
    EditFilter();
    ~EditFilter();
    QString filterMsg(){
        return this->m_filter_widget->text().trimmed();
    }
    int get_height(){
        return 20;
    }

private:
    QLineEdit *m_filter_widget;
    QHBoxLayout* m_hLayout;
};

class ComboxFilter : public AbstractFilter
{
    Q_OBJECT

public:
    ComboxFilter(QMap<QString,QString> propertis);
    ~ComboxFilter();
    QString filterMsg(){
        return m_filter_widget->currentData().toString();
    }
    int get_height(){
        return 20;
    }

private:
    QComboBox *m_filter_widget;
    QHBoxLayout* m_hLayout;
};
#endif // CUSTOMHEADERVIEW_H
