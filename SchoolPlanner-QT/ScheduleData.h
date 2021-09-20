#ifndef SCHEDULEDATA_H
#define SCHEDULEDATA_H

#include <QList>
#include <QFile>
#include <QJsonArray>
#include <QJsonObject>
#include <QStringList>
#include <QJsonDocument>

class ScheduleNode {
public:
    ScheduleNode(void);
    ScheduleNode(const QJsonObject &json);
    ScheduleNode(Qt::DayOfWeek day, quint8 slot, QString room, QString group, QString course, QString teacher);
    ScheduleNode(const ScheduleNode& copy);

    bool operator ==(const ScheduleNode& other) {
        return Day == other.Day
            && Slot == other.Slot
            && Room == other.Room
            && Group == other.Group
            && Course == other.Course
            && Teacher == other.Teacher;
    }

    void SaveJSON(QJsonObject &json) const;
    void LoadJSON(const QJsonObject &json);

    quint8 Slot;
    QString Room;
    QString Group;
    QString Course;
    QString Teacher;
    Qt::DayOfWeek Day;
};

typedef QList<ScheduleNode> NodeList;

class ScheduleData {
public:
    ScheduleData(void);
    ScheduleData(const ScheduleData& copy);

    void SaveJSON(const QString& path);
    void LoadJSON(const QString& path);

    NodeList Nodes;
    QStringList Rooms;
    QStringList Groups;
    QStringList Courses;
    QStringList Teachers;
};

#endif // SCHEDULEDATA_H
