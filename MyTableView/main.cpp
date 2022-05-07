#include "usertable.h"
#include <QApplication>
#include <QTcpSocket>
#include<QDateTime>
QVector<QMap<QString,QString> >  simulate_datas;  //模拟源数据，例如数据库中查出来的数据
int simulate_num = 600;



void generate_simulate_data()
{

    for (int i = 0; i < simulate_num; i++)
    {
        int rand_num = qrand();
        QString age = QString::number(rand_num%5 + 10);
        QString sex = QString::number(rand_num%2) ;
        QString name = QString("TOM_%1_%2").arg(age).arg(sex);
        QMap<QString,QString> data;
        data.insert("age",age);
        data.insert("sex",sex);
        data.insert("name",name);
        simulate_datas.push_back(data);
    }

}

int query_total_records(const QMap<QString,QString>& map_filtes)
{
    int cnt = 0;
    foreach (auto data, simulate_datas) {
        bool fit = true;
        foreach (auto key, map_filtes.keys()) {

            if ( !key.compare("age") ||!key.compare("sex") )  //值比较类型查询
            {
                //有查询条件，且判断不相等
                if ( !map_filtes.value(key).isEmpty() &&
                     map_filtes.value(key).compare(data.value(key)))
                {
                    fit = false;
                    break;
                }
            }
            if ( !key.compare("name") )  //字符串子串比较类型查询
            {
                //有查询条件，且数据里没有搜索子串
                if ( !map_filtes.value(key).isEmpty() &&
                     data.value(key).indexOf(map_filtes.value(key)) == -1 )
                {

                    fit = false;
                    break;
                }
            }

        }
        if ( fit )
        {
             cnt++;
        }
    }
    return cnt;
}
int query_page(ulong start,ulong end,
               const QMap<QString,QString>& map_filtes,
               QVector<QMap<QString,QString> > & datas)
{
    QVector<QMap<QString,QString> >  fit_datas;
    foreach (auto data, simulate_datas) {

        bool fit = true;
        foreach (auto key, map_filtes.keys()) {

            if ( !key.compare("age") ||!key.compare("sex") )  //值比较类型查询
            {
                //有查询条件，且判断不相等
                if ( !map_filtes.value(key).isEmpty() &&
                     map_filtes.value(key).compare(data.value(key)))
                {
                    fit = false;
                    break;
                }
            }
            if ( !key.compare("name") )  //字符串子串比较类型查询
            {
                //有查询条件，且数据里没有搜索子串
                if ( !map_filtes.value(key).isEmpty() &&
                     data.value(key).indexOf(map_filtes.value(key)) == -1 )
                {

                    fit = false;
                    break;
                }
            }

        }
        if (fit )
        {
             fit_datas.push_back(data);
        }
    }
    if (end == ULONG_MAX)
    {
        datas = fit_datas;
        return  datas.size();
    }
    if (start > fit_datas.size() - 1 || start < 1)
    {
        return 0;
    }
    if ( end < start )
    {
        return 0;
    }
    if (end > fit_datas.size())
    {
        end = fit_datas.size();
    }
    for (int i = start - 1; i < end; i ++)
    {
        datas.push_back(fit_datas.at(i));
    }
    return datas.size();
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    generate_simulate_data(); //生成模拟数据，test
    UserTable w;

    /*
    QList<QString> header ;
    header<<QString("姓名")<<QString("性别")<<QString("年龄");

    QList<QString> column_alias;
    column_alias<<"name"<<"sex"<<"age";

    //设置表头显示内容
    w.set_table_header(header);
    //设置列别名，即数据项key值
    w.set_column_alias(column_alias);*/

    //设置表头
    QVector<UserHead> heads;
    heads.push_back(UserHead("姓名","name",new EditFilter));
    QMap<QString,QString> cbx_items;
    cbx_items.insert("","全部");
    cbx_items.insert("0","男");
    cbx_items.insert("1","女");

    heads.push_back(UserHead("性别","sex",new ComboxFilter(cbx_items)));
    heads.push_back(UserHead("年龄","age",NULL));
    w.setHeadDatas(heads);

    //设置需要转化内容参数，针对源数据是a，显示是b场景。如枚举类型.
    QMap<QString,CbxItem> translate;
    translate.insert("sex",cbx_items);
    w.set_content_tranlation(translate);
    //设置查询函数指针
    w.query_data = query_page;
    //设置查询总数函数指针
    w.query_record_total = query_total_records;
    //w.set_add_func(false);    //是否需要新增按钮
    w.set_modify_func(false); //是否需要修改按钮
    w.set_del_func(false);
    w.set_is_paging(true);  //是否需要分页
    //自定义表格初始化
    w.init();
    w.show();

    return a.exec();
}
