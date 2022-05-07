#ifndef USERTABLE_H
#define USERTABLE_H

#include <QWidget>
#include <QStandardItemModel>
#include<QSortFilterProxyModel>
#include"CustomModelView/customhorizontalheaderview.h"
namespace Ui {
class UserTable;
}
const static QString PRE_BTN_DISABLE_ICON = "pre_bt_disable.png";
const static QString PRE_BTN_ENABLE_ICON = "pre_bt_enable.png";
const static QString NEXT_BTN_DISABLE_ICON = "next_bt_disable.png";
const static QString NEXT_BTN_ENABLE_ICON = "next_bt_enable.png";
const static QString FIRST_BTN_DISABLE_ICON = "first_bt_disable.png";
const static QString FIRST_BTN_ENABLE_ICON = "first_bt_enable.png";
const static QString LAST_BTN_DISABLE_ICON = "last_bt_disable.png";
const static QString LAST_BTN_ENABLE_ICON = "last_bt_enable.png";
const static QString icon_dir= "/images/pictures/button/" ;

class QTableView;

typedef QMap<QString,QString> CbxItem;
class UserTable : public QWidget
{
    Q_OBJECT

public:
    explicit UserTable(QWidget *parent = 0);
    ~UserTable();
 protected:
    void search_data();
    void update_page_status();
    void data_to_view();
public:
    void init();
    void set_table_header(const QList<QString>& header);
    void set_column_alias(const QList<QString>& column_alias);
    const QTableView* get_table_view();
    void set_query_func(bool isShow);
    void set_add_func(bool isShow);
    void set_modify_func(bool isShow);

    void set_del_func(bool isShow);
    void set_is_paging(bool isShow);
    void set_head_render_type(int index,int type);
    void set_content_tranlation(const QMap<QString, CbxItem> &content_tranlation);
    /*
    * 应用需要实现，查询数据总条数
    * 返回值为总条数
    */
    int (*query_record_total)(const QMap<QString,QString>& map_filtes);
    /*应用需要实现，根据起始值和结束值查询数据，并返回数据集合
    *入参，start数据查询起始值，
    *入参，end数据查询结束值
    * 引用入参，datas，数据返回结果
    * return 查询的记录数
    */
    int (*query_data) (ulong start, ulong end,const QMap<QString,QString>& map_filtes,
                       QVector<QMap<QString,QString> > & datas); // 应用需要实现
    void setHeadDatas(QVector<UserHead> &headDatas);
public slots:
    void first_btn_click();
    void last_btn_click();
    void pre_btn_click();
    void next_btn_click();
    void page_edit_finish();
    void page_row_num_changed(int cmb_index);
    void queryBtn_clicked();
    void delBtn_clicked();
    void modifyBtn_clicked();
    void addBtn_clicked();
    void searchBtn_clicked();
    void dataChangedSlot(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles);
    void sort_data_click(int index,int sort_flag);
private:
    Ui::UserTable *ui;
    //分页
    QAction *action_cfg;
    int m_page_total;  //总页数
    int m_current_page;//当前页
    int m_page_row_num;//每页显示条目
    int m_record_size; // 总条目
    QVector<QMap<QString,QString> > m_datas;  //查询的数据 ,map<列别名:列值>
    QList <QString> m_column_defines; // 列别名   应用需要设定,列别名和m_datas 中map  key值对应
    QList <QString> m_table_header;   //列显示名   应用需要设定，和m_column_defines顺序一一对应
    QMap<QString,CbxItem > m_content_tranlation;
    bool m_is_paging;
    QStandardItemModel * m_model;
    QSortFilterProxyModel *m_pSortFilterModel;
    CustomHorizontalHeaderView* m_pHHeaderView;


};

#endif // USERTABLE_H
