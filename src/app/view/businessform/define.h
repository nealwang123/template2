#ifndef DEFINE_H
#define DEFINE_H

#include <QVector>

//学生信息
typedef struct _STUDENT{
    QString name;   //索引
    QString time;     //时间

//    _STUDENT()
//    {
//        name = "";
//        time = "";
//    }
}STUDENT,*PSTUDENT;

//班级信息
typedef struct _CLASS{
    QString name;   //班级
    QVector<STUDENT*> students;
//    _CLASS()
//    {
//        name = "";
//    }
}CLASS;


#endif // DEFINE_H
