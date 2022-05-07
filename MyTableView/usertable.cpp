#include "usertable.h"
#include "ui_usertable.h"
#include"tabledelegate.h"
#include<qDebug>
UserTable::UserTable(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserTable)
{
    m_is_paging = true;
    ui->setupUi(this);
    m_model = new QStandardItemModel(ui->tableView);
    m_pHHeaderView = new CustomHorizontalHeaderView(this);
    m_pSortFilterModel = new QSortFilterProxyModel(this);
}
void UserTable::init()
{
    m_page_total = 0;
    m_record_size = 0;
    m_current_page = 1;
    m_page_row_num = 500;
    ui->first_btn->resize(32,32);
    ui->last_btn->resize(32,32);
    ui->pre_btn->resize(32,32);
    ui->next_btn->resize(32,32);
    ui->setPageNumCbx->addItem(tr("200条/页"),200);
    ui->setPageNumCbx->addItem(tr("500条/页"),500);
    ui->setPageNumCbx->addItem(tr("1000条/页"),1000);
    ui->setPageNumCbx->addItem(tr("2000条/页"),2000);
    ui->setPageNumCbx->setCurrentIndex(1);//默认500条
    ui->page_edit->setMinimum(1);

    //只能改变系统提供的headerview样式,自定义表头的样式不在此处修改
    //ui->tableView->setStyleSheet("QHeaderView::section{padding:0px}");
    ui->tableView->resizeRowsToContents();//自适应行高
    //ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);//设置QTableWidget为不可编辑
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);//单击选中行
    ui->tableView->setItemDelegate(new TableDelegate(this)); //设置代理
    m_model->setHorizontalHeaderLabels(m_table_header);
    //ui->tableView->setModel(m_model);
    m_pSortFilterModel->setSourceModel(m_model);
    ui->tableView->setModel(m_pSortFilterModel);
    ui->tableView->setHorizontalHeader(m_pHHeaderView);
    ui->tableView->horizontalHeader()->setDefaultSectionSize(180);

    //设置排序 表头自定义，需要使用QSortFilterProxyModel代理
    connect(m_pHHeaderView,SIGNAL(sort_data(int,int)),this,SLOT(sort_data_click(int,int)));
    //m_pHHeaderView->setSortIndicatorShown(true);
    //connect(m_pHHeaderView, SIGNAL(sectionClicked(int)), ui->tableView, SLOT (sortByColumn(int)));
    //其他表格用户自定义设置可以通过提供接口获取tablewidget对象再次设置

    //表格操作相关事件
    connect(ui->query_btn, SIGNAL(clicked()), this, SLOT(queryBtn_clicked()));
    connect(ui->del_btn, SIGNAL(clicked()), this, SLOT(delBtn_clicked()));
    connect(ui->modify_btn, SIGNAL(clicked()), this, SLOT(modifyBtn_clicked()));
    connect(ui->new_btn, SIGNAL(clicked()), this, SLOT(addBtn_clicked()));
    connect(ui->search_btn, SIGNAL(clicked()), this, SLOT(searchBtn_clicked()));
    //分页相关事件
    connect(ui->first_btn, SIGNAL(clicked()), this, SLOT(first_btn_click()));
    connect(ui->last_btn, SIGNAL(clicked()), this, SLOT(last_btn_click()));
    connect(ui->pre_btn, SIGNAL(clicked()), this, SLOT(pre_btn_click()));
    connect(ui->next_btn, SIGNAL(clicked()), this, SLOT(next_btn_click()));
    connect(ui->page_edit, SIGNAL(editingFinished()), this, SLOT(page_edit_finish()));
    connect(ui->setPageNumCbx, SIGNAL(currentIndexChanged(int)), this, SLOT(page_row_num_changed(int)));
    update_page_status();
}
void UserTable::sort_data_click(int index,int sort_flag)
{
    m_pSortFilterModel->setFilterKeyColumn(index);
    if ( sort_flag == ASCENDING_SORT )
    {

         m_pSortFilterModel->sort(index, Qt::AscendingOrder);
    }
    else if ( sort_flag == DESCENDING_SORT )
    {
         m_pSortFilterModel->sort(index, Qt::DescendingOrder);
    }
    else
    {
         m_pSortFilterModel->sort(-1, Qt::DescendingOrder);  // -1则是还原默认顺序
    }


}
void UserTable::queryBtn_clicked()
{
    m_current_page = 1;
    search_data();
}
void UserTable::delBtn_clicked()
{

}
void UserTable::modifyBtn_clicked()
{

}
void UserTable::addBtn_clicked()
{

}

void UserTable::first_btn_click()
{
    m_current_page = 1;
    search_data();
}
void UserTable::last_btn_click()
{
    m_current_page = m_page_total;
    search_data();
}
void UserTable::pre_btn_click()
{

    if ( m_current_page <= 1 )
    {
        m_current_page = 1;
    }
    else
    {
        m_current_page--;
    }
    search_data();
}
void UserTable::next_btn_click()
{
    if (  m_current_page >= m_page_total )
    {
        m_current_page = m_page_total;
    }
    else
    {
        m_current_page++;
    }

    search_data();
}
void UserTable::page_edit_finish()
{

    m_current_page = ui->page_edit->value();
    search_data();
}
void UserTable::page_row_num_changed(int cmb_index)
{
    m_current_page = 1;
    m_page_row_num = ui->setPageNumCbx->currentData().toInt();
    search_data();
}
void UserTable::update_page_status()
{
    //每次更新下页码信息
    ui->page_info_label->setText(QString(tr("共%1条数据,%2页").arg(m_record_size).arg(m_page_total)));
    ui->page_edit->setMaximum(m_page_total == 0 ? 1 :m_page_total );

    ui->page_edit->setValue(m_current_page);



    if (m_page_total == 0)
    {
        ui->pre_btn->setEnabled(false);
        ui->next_btn->setEnabled(false);
        ui->first_btn->setEnabled(false);
        ui->last_btn->setEnabled(false);
        ui->first_btn->setIcon(QPixmap(icon_dir + FIRST_BTN_DISABLE_ICON));
        ui->last_btn->setIcon(QPixmap(icon_dir + LAST_BTN_DISABLE_ICON));
        ui->pre_btn->setIcon(QPixmap(icon_dir + PRE_BTN_DISABLE_ICON));
        ui->next_btn->setIcon(QPixmap(icon_dir + NEXT_BTN_DISABLE_ICON));

    }
    else
    {
        ui->first_btn->setEnabled(true);
        ui->last_btn->setEnabled(true);
        ui->first_btn->setIcon(QPixmap(icon_dir + FIRST_BTN_ENABLE_ICON));
        ui->last_btn->setIcon(QPixmap(icon_dir + LAST_BTN_ENABLE_ICON));
    }
    if ( m_current_page <= 1 )
    {
        ui->pre_btn->setEnabled(false);
        ui->pre_btn->setIcon(QPixmap(icon_dir + PRE_BTN_DISABLE_ICON));
    }
    else
    {
        ui->pre_btn->setEnabled(true);
         ui->pre_btn->setIcon(QPixmap(icon_dir + PRE_BTN_ENABLE_ICON));
    }
    if ( m_current_page >= m_page_total )
    {
        ui->next_btn->setEnabled(false);
        ui->next_btn->setIcon(QPixmap(icon_dir + NEXT_BTN_DISABLE_ICON));
    }
    else
    {
        ui->next_btn->setEnabled(true);
        ui->next_btn->setIcon(QPixmap(icon_dir + NEXT_BTN_ENABLE_ICON));
    }
}
void UserTable::search_data()
{
    foreach (auto data, m_datas) {
        data.clear();
    }
    m_datas.clear();
    QVector<QString> filters;
    m_pHHeaderView->getFilterContents(filters);
    //查询条件组装成map形式返回给应用
    QMap<QString,QString> map_filtes;
    for (int index = 0; index < filters.size(); index++) {
        map_filtes.insert(m_column_defines.at(index),filters.at(index));
    }
    if (m_is_paging)
    {
        m_record_size = (*query_record_total)(map_filtes);
        m_page_total = m_record_size % m_page_row_num == 0 ?
                       m_record_size / m_page_row_num : m_record_size / m_page_row_num + 1;
        int start = (m_current_page -1) * m_page_row_num + 1;
        int end = start + m_page_row_num - 1;

        (*query_data)(start,end,map_filtes,m_datas);

        update_page_status();
    }
    else
    {
        (*query_data)(1,ULONG_MAX,map_filtes,m_datas);

    }
    m_model->disconnect(SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)));
    data_to_view();
    connect(m_model, SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)), this, SLOT(dataChangedSlot(QModelIndex,QModelIndex,QVector<int>)));

}

void UserTable::data_to_view()
{
    m_model->removeColumns(0,m_model->rowCount());
    int num_rows = m_datas.size();
    m_model->setRowCount(num_rows);
    int rowno = (m_current_page - 1) * m_page_row_num + 1;
    for(int row = 0; row < num_rows; row ++)
    {

        m_model->setHeaderData(row,Qt::Vertical,rowno++);
        auto data = m_datas.at(row);
        foreach ( QString key,data.keys() )
        {
            QString content;
            if ( m_content_tranlation.contains(key) )
            {
               content = m_content_tranlation.value(key).value(data.value(key),"");
            }
            else
            {
               content = data.value(key);
            }
            m_model->setItem(row,
                             m_column_defines.indexOf(key),
                             new QStandardItem(content));
        }

    }


}
void UserTable::dataChangedSlot(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles)
{
    qDebug() << m_model->data(topLeft).toString() << endl;
}
UserTable::~UserTable()
{
    delete ui;

}
void UserTable::set_content_tranlation(const QMap<QString,CbxItem >& content_tranlation)
{
    m_content_tranlation = content_tranlation;
}
void UserTable::set_table_header(const QList<QString>& header)
{
    m_table_header = header;
}
void UserTable::set_column_alias(const QList<QString>& column_alias)
{
    m_column_defines = column_alias;
}
const QTableView * UserTable::get_table_view()
{
    return ui->tableView;
}
void UserTable::set_query_func(bool isShow){
    ui->query_btn->setVisible(isShow);

}
void UserTable::set_add_func(bool isShow){

    ui->new_btn->setVisible(isShow);
}
void UserTable::set_modify_func(bool isShow){
    ui->modify_btn->setVisible(isShow);
}
void UserTable::set_del_func(bool isShow){
    ui->del_btn->setVisible(isShow);
}
void UserTable::set_is_paging(bool isShow){
   ui->first_btn->setVisible(isShow);
   ui->pre_btn->setVisible(isShow);
   ui->next_btn->setVisible(isShow);
   ui->last_btn->setVisible(isShow);
   ui->page_edit->setVisible(isShow);
   ui->page_info_label->setVisible(isShow);
   ui->setPageNumCbx->setVisible(isShow);
   m_is_paging = isShow;
}

void UserTable::setHeadDatas(QVector<UserHead>& headDatas)
{
    m_pHHeaderView->setHeadDatas(headDatas);
    m_table_header.clear();
    m_column_defines.clear();
    foreach (UserHead head, headDatas) {
        m_table_header.append(head.m_head_name);
        m_column_defines.append(head.m_column_alias);
    }
}

void UserTable::searchBtn_clicked()
{
    m_pHHeaderView->showFilter();

    //重新调整下控件位置，否则表头高度的动态变化会导致表体覆盖或者出现空白
    ui->tableView->adjustSize();
    ui->tableView->updateGeometry();
}
