#include "ScheduleData.h"

ScheduleData::ScheduleData(void) {

}

ScheduleData::ScheduleData(const ScheduleData& copy) : Nodes(copy.Nodes), Rooms(copy.Rooms), Groups(copy.Groups), Courses(copy.Courses), Teachers(copy.Teachers) {

}

void ScheduleData::SaveJSON(const QString& path) {

    QFile saveFile(path);
    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open save file.");
    }

    QJsonObject jroot;
    QJsonArray jrooms;
    QJsonArray jgroups;
    QJsonArray jcourses;
    QJsonArray jteachers;
    QJsonArray jactivities;

    for (const QString &value : Rooms) {
        jrooms.append(value);
    }

    for (const QString &value : Groups) {
        jgroups.append(value);
    }

    for (const QString &value : Courses) {
        jcourses.append(value);
    }

    for (const QString &value : Teachers) {
        jteachers.append(value);
    }

    for (const ScheduleNode&value : Nodes) {
        QJsonObject jvalue;
        value.SaveJSON(jvalue);
        jactivities.append(jvalue);
    }

    jroot["rooms"] = jrooms;
    jroot["groups"] = jgroups;
    jroot["courses"] = jcourses;
    jroot["teachers"] = jteachers;
    jroot["activities"] = jactivities;

    saveFile.write(QJsonDocument(jroot).toJson());
    saveFile.close();
}

void ScheduleData::LoadJSON(const QString& path) {

    QFile loadFile(path);
    if (!loadFile.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open save file.");
    }

    auto saveData = loadFile.readAll();
    auto jdoc = QJsonDocument::fromJson(saveData);
    loadFile.close();

    QJsonArray jrooms = jdoc["rooms"].toArray();
    QJsonArray jgroups = jdoc["groups"].toArray();
    QJsonArray jcourses = jdoc["courses"].toArray();
    QJsonArray jteachers = jdoc["teachers"].toArray();
    QJsonArray jactivities = jdoc["activities"].toArray();

    for (const QJsonValue &value : jrooms) {
        Rooms.append(value.toString());
    }

    for (const QJsonValue &value : jgroups) {
        Groups.append(value.toString());
    }

    for (const QJsonValue &value : jcourses) {
        Courses.append(value.toString());
    }

    for (const QJsonValue &value : jteachers) {
        Teachers.append(value.toString());
    }

    for (const QJsonValue &value : jactivities) {
        auto jobject = value.toObject();
        ScheduleNode node(jobject);
        Nodes.append(node);
    }
}

ScheduleNode::ScheduleNode(void) {
    Day = Qt::DayOfWeek::Monday;
    Slot = 0;
    Room = QString();
    Group = QString();
    Course = QString();
    Teacher = QString();
}

ScheduleNode::ScheduleNode(const QJsonObject &json) {
    LoadJSON(json);
}

ScheduleNode::ScheduleNode(Qt::DayOfWeek day, quint8 slot, QString room, QString group, QString course, QString teacher) {
    Day = day;
    Slot = slot;
    Room = room;
    Group = group;
    Course = course;
    Teacher = teacher;
}

ScheduleNode::ScheduleNode(const ScheduleNode& copy) {
    Day = copy.Day;
    Slot = copy.Slot;
    Room = copy.Room;
    Group = copy.Group;
    Course = copy.Course;
    Teacher = copy.Teacher;
}

void ScheduleNode::SaveJSON(QJsonObject &json) const {
    json["teacher"] = Teacher;
    json["course"] = Course;
    json["group"] = Group;
    json["room"] = Room;
    json["slot"] = Slot;

    // good solution
    // json["day"] = (int)DayOfWeek;

    // not the smartest solution
    switch (Day) {
        case Qt::DayOfWeek::Monday: json["day"] = "mon"; break;
        case Qt::DayOfWeek::Tuesday: json["day"] = "tue"; break;
        case Qt::DayOfWeek::Wednesday: json["day"] = "wed"; break;
        case Qt::DayOfWeek::Thursday: json["day"] = "th"; break;
        case Qt::DayOfWeek::Friday: json["day"] = "fri"; break;
        case Qt::DayOfWeek::Saturday: json["day"] = "sat"; break;
        case Qt::DayOfWeek::Sunday: json["day"] = "sun"; break;
    }
}

void ScheduleNode::LoadJSON(const QJsonObject &json) {
    Teacher = json["teacher"].toString();
    Course = json["course"].toString();
    Group = json["group"].toString();
    Room = json["room"].toString();
    Slot = json["slot"].toInt();

    // good solution
    // Day = (Qt::DayOfWeek)json["day"].toInt();

    // not the smartest solution
    auto dayStr = json["day"].toString();
    if (dayStr == "mon") {
        Day = Qt::DayOfWeek::Monday;
    } else if (dayStr == "tue") {
        Day = Qt::DayOfWeek::Tuesday;
    } else if (dayStr == "wed") {
        Day = Qt::DayOfWeek::Wednesday;
    } else if (dayStr == "th") {
        Day = Qt::DayOfWeek::Thursday;
    } else if (dayStr == "fri") {
        Day = Qt::DayOfWeek::Friday;
    } else if (dayStr == "sat") {
        Day = Qt::DayOfWeek::Saturday;
    } else if (dayStr == "sun") {
        Day = Qt::DayOfWeek::Sunday;
    }
    //
}
