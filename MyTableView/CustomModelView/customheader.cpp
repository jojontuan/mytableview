#include "customheader.h"
#include "ui_customheader.h"
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QDebug>


EditFilter::EditFilter()
{
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint);
    m_hLayout = new QHBoxLayout();
    m_filter_widget = new QLineEdit();
    m_hLayout->addWidget(m_filter_widget);
    this->setLayout(m_hLayout);
    this->setWindowModality(Qt::ApplicationModal);
    m_hLayout->setContentsMargins(0,0,0,0);
    QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    m_filter_widget->setSizePolicy(sizePolicy);
}
EditFilter::~EditFilter()
{
    delete m_hLayout;
    delete m_filter_widget;
}
ComboxFilter::ComboxFilter(QMap<QString,QString> propertis)
{
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint);
    m_hLayout = new QHBoxLayout();
    m_filter_widget = new QComboBox();
    foreach (QString value, propertis) {
        qDebug()<<value<<":"<<propertis.key(value);
        m_filter_widget->addItem(value,QVariant(propertis.key(value)));
    }
    m_hLayout->addWidget(m_filter_widget);
    this->setLayout(m_hLayout);
    this->setWindowModality(Qt::ApplicationModal);
    m_hLayout->setContentsMargins(0,0,0,0);
    QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    m_filter_widget->setSizePolicy(sizePolicy);
}
ComboxFilter::~ComboxFilter()
{
    delete m_hLayout;
    delete m_filter_widget;
}

CustomHeader::CustomHeader(int index, AbstractFilter* filter,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CustomHeader),
    m_index(index),
    m_filter(filter)
{
    m_sort_flag = NONE_SORT;
    ui->setupUi(this);
    this->setObjectName("CustomHeader");
    m_label_title = new QPushButton(this);
    QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    QHBoxLayout * hbox = new QHBoxLayout();
    hbox->addWidget(m_label_title);
    m_label_title->setSizePolicy(sizePolicy);
    ui->gridLayout_2->addLayout(hbox, 0, 0, 1, 1);

    if (m_filter)
    {

        ui->gridLayout_2->addWidget(m_filter, 1, 0, 1, 1);
        m_filter->hide();
    }

    ui->widget_header->installEventFilter(this);
    m_label_title->installEventFilter(this);
    m_label_title->setObjectName("head_button");
    setStyleSheet("#CustomHeader #widget_header{\
                  background-color:rgb(142, 213, 213);\
                  border-right: 1px solid rgb(230, 230, 230);}\
                  #CustomHeader #widget_header #head_button{\
                  background-color:rgb(142, 213, 213);\
                  border:0px;}");

    connect(m_label_title,SIGNAL(clicked()),this,SLOT(trigger_sort()));
}

void CustomHeader::trigger_sort()
{
    m_sort_flag++;
    if ( m_sort_flag > DESCENDING_SORT)
    {
        m_sort_flag = NONE_SORT;
    }
    emit sort_data(m_index,m_sort_flag);
}

CustomHeader::~CustomHeader()
{
    delete ui;
}

void CustomHeader::setTitle(QString text)
{
    m_label_title->setText(text);
}
QString CustomHeader::title()
{
    return m_label_title->text();
}

void CustomHeader::setAlignment(Qt::Alignment align)
{
    //m_label_title->setAlignment(align);
}


void CustomHeader::clearStatus()
{
    m_sort_flag = NONE_SORT;

}
void CustomHeader::clearFilterStatus()
{


}

QString CustomHeader::getFilterMsg()
{
    if ( m_filter )
    {
        return m_filter->filterMsg();
    }
    else
    {
        return "";
    }

}

bool CustomHeader::eventFilter(QObject *obj, QEvent *event)
{
    if(obj == ui->widget_header || obj == m_label_title ){
            setCursor(Qt::PointingHandCursor);
    }

    return QWidget::eventFilter(obj, event);
}

void CustomHeader::hide_filter(bool hidden)
{
    if( m_filter )
      m_filter->setVisible(hidden);
}
int CustomHeader::get_head_title_height()
{
    return 20;
}
int CustomHeader::get_head_height()
{
    if( m_filter )
    {
        return get_head_title_height() + m_filter->get_height();
    }
    else
    {
        return get_head_title_height();
    }

}
