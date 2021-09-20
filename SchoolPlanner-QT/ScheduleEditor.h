#ifndef SCHEDULEEDITOR_H
#define SCHEDULEEDITOR_H

#include "ScheduleData.h"

#include <QLabel>
#include <QDialog>
#include <QComboBox>
#include <QStringList>
#include <QStringListModel>

class ScheduleEditor : public QDialog {
    Q_OBJECT

public:
    ScheduleEditor(QWidget* parent = nullptr);
    ~ScheduleEditor(void);

    ScheduleNode getNode(void) const;
    QStringList getRooms(void) const;
    QStringList getGroups(void) const;
    QStringList getCourses(void) const;
    QStringList getTeachers(void) const;

    void setNode(const ScheduleNode& node);
    void setRooms(const QStringList& list);
    void setGroups(const QStringList& list);
    void setCourses(const QStringList& list);
    void setTeachers(const QStringList& list);

    // return dialogue
    static const quint32 Removal = 3;

private slots:
    void Accept_Click(void);
    void Cancel_Click(void);
    void Remove_Click(void);

    void Group_Changed(int index);
    void Course_Changed(int index);
    void Teacher_Changed(int index);

private:
    QLabel* Room;
    QComboBox* Group;
    QComboBox* Course;
    QComboBox* Teacher;

    ScheduleNode Node;

    QStringListModel* Rooms;
    QStringListModel* Groups;
    QStringListModel* Courses;
    QStringListModel* Teachers;

    void initializeComponent(void);
    void disposeComponent(void);
};

#endif // SCHEDULEEDITOR_H
