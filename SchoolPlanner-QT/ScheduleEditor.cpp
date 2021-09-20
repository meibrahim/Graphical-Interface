#include "ScheduleEditor.h"

ScheduleEditor::ScheduleEditor(QWidget* parent) : QDialog(parent) {
    initializeComponent();
}

ScheduleEditor::~ScheduleEditor(void) {
    disposeComponent();
}

ScheduleNode ScheduleEditor::getNode(void) const {
    return Node;
}

QStringList ScheduleEditor::getRooms(void) const {
    return Rooms->stringList();
}

QStringList ScheduleEditor::getGroups(void) const {
    return Groups->stringList();
}

QStringList ScheduleEditor::getCourses(void) const {
    return Courses->stringList();
}

QStringList ScheduleEditor::getTeachers(void) const {
    return Teachers->stringList();
}

void ScheduleEditor::setNode(const ScheduleNode& node) {
    Node = node;
    Room->setText(Node.Room);
    Group->setCurrentText(Node.Group);
    Course->setCurrentText(Node.Course);
    Teacher->setCurrentText(Node.Teacher);
}

void ScheduleEditor::setRooms(const QStringList& list) {
    Rooms->setStringList(list);
}

void ScheduleEditor::setGroups(const QStringList& list) {
    Groups->setStringList(list);
}

void ScheduleEditor::setCourses(const QStringList& list) {
    Courses->setStringList(list);
}

void ScheduleEditor::setTeachers(const QStringList& list) {
    Teachers->setStringList(list);
}

void ScheduleEditor::Accept_Click(void) {
    done(QDialog::Accepted);
}

void ScheduleEditor::Cancel_Click(void) {
    done(QDialog::Rejected);
}

void ScheduleEditor::Remove_Click(void) {
    done(ScheduleEditor::Removal);
}

void ScheduleEditor::Group_Changed(int index) {
    auto groups = getGroups();
    if (groups.count() > index && index > -1) {
        Node.Group = groups[index];
    }
}

void ScheduleEditor::Course_Changed(int index) {
    auto courses = getCourses();
    if (courses.count() > index && index > -1) {
        Node.Course = courses[index];
    }
}

void ScheduleEditor::Teacher_Changed(int index) {
    auto teachers = getTeachers();
    if (teachers.count() > index && index > -1) {
        Node.Teacher = teachers[index];
    }
}

#include <QHBoxLayout>
#include <QFormLayout>
#include <QPushButton>
#include <QComboBox>
#include <QLabel>

void ScheduleEditor::initializeComponent(void) {
    //
    setWindowTitle(tr("Schedule Node Editor"));
    resize(480, 180);

    // Set-up the root widget and the base layout
    auto Layout = new QFormLayout;
    setLayout(Layout);

    Room = new QLabel;
    Group = new QComboBox;
    Course = new QComboBox;
    Teacher = new QComboBox;
    Layout->addRow(tr("&Room: "), Room);
    Layout->addRow(tr("&Group: "), Group);
    Layout->addRow(tr("&Course: "), Course);
    Layout->addRow(tr("&Teacher: "), Teacher);

    connect(Group, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ScheduleEditor::Group_Changed);
    connect(Course, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ScheduleEditor::Course_Changed);
    connect(Teacher, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ScheduleEditor::Teacher_Changed);
    //

    // Set-up the controls
    auto Buttons = new QHBoxLayout;
    auto Apply = new QPushButton("&Apply");
    auto Cancel = new QPushButton("&Cancel");
    auto Remove = new QPushButton("&Remove");

    Buttons->addWidget(Apply);
    Buttons->addWidget(Cancel);
    Buttons->addWidget(Remove);
    Layout->addRow(Buttons);

    connect(Apply, &QPushButton::released, this, &ScheduleEditor::Accept_Click);
    connect(Cancel, &QPushButton::released, this, &ScheduleEditor::Cancel_Click);
    connect(Remove, &QPushButton::released, this, &ScheduleEditor::Remove_Click);
    //

    // Setup models
    Rooms = new QStringListModel;
    Groups = new QStringListModel;
    Courses = new QStringListModel;
    Teachers = new QStringListModel;

    Group->setModel(Groups);
    Course->setModel(Courses);
    Teacher->setModel(Teachers);
    //
}

void ScheduleEditor::disposeComponent(void) {
    if (Rooms != nullptr) {
        delete Rooms;
        Rooms = nullptr;
    }
    if (Groups != nullptr) {
        delete Groups;
        Groups = nullptr;
    }
    if (Courses != nullptr) {
        delete Courses;
        Courses = nullptr;
    }
    if (Teachers != nullptr) {
        delete Teachers;
        Teachers = nullptr;
    }
}
