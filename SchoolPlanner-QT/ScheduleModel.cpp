#include "ScheduleModel.h"
#include <QBrush>

ScheduleModel::ScheduleModel(QObject *parent) : QAbstractTableModel(parent) {
    MinDay = Qt::DayOfWeek::Monday;
    MaxDay = Qt::DayOfWeek::Friday;
    MinHour = 8;
    MaxHour = 20;
    Room = QString();
}

int ScheduleModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return MaxHour - MinHour;
}

int ScheduleModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return getMaxDay() - getMinDay() + 1;
}

QVariant ScheduleModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid()) {
        return QVariant();
    }

    if (role == Qt::DisplayRole) {
        auto node = getNode(index);
        return node.Group;
    } else if (role == Qt::TextAlignmentRole) {
        return int(Qt::AlignHCenter | Qt::AlignVCenter);
    }

    return QVariant();
}

QVariant ScheduleModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role == Qt::DisplayRole) {
        if (orientation == Qt::Horizontal) {
            auto day = section + Qt::DayOfWeek::Monday;
            return toString((Qt::DayOfWeek)day);
        } else if (orientation == Qt::Vertical) {
            return QString(section % 2 == 0 ? "%1:15 - %2:00" : "%1:00 - %2:45")
                    .arg(getMinHour() + section)
                    .arg(getMinHour() + section + 1);
        }
    }
    return QVariant();
}

Qt::ItemFlags ScheduleModel::flags(const QModelIndex &index) const  {
    if (!index.isValid()) {
        return Qt::ItemIsEnabled;
    } else {
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    }
}

QString ScheduleModel::getRoom(void) const {
    return Room;
}

NodeList ScheduleModel::getNodes(void) const {
    return Nodes;
}

Qt::DayOfWeek ScheduleModel::getMinDay(void) const {
    return MinDay;
}

Qt::DayOfWeek ScheduleModel::getMaxDay(void) const {
    return MaxDay;
}

quint8 ScheduleModel::getMinHour(void) const {
    return MinHour;
}

quint8 ScheduleModel::getMaxHour(void) const {
    return MaxHour;
}

void ScheduleModel::setNodes(const NodeList& nodes) {
    Nodes = nodes;
    Refresh();
}

void ScheduleModel::setMinDay(Qt::DayOfWeek value) {
    MinDay = value;
    Refresh();
}

void ScheduleModel::setMaxDay(Qt::DayOfWeek value) {
    MaxDay = value;
    Refresh();
}

void ScheduleModel::setMinHour(quint8 value) {
    MinHour = value;
    Refresh();
}

void ScheduleModel::setMaxHour(quint8 value) {
    MaxHour = value;
    Refresh();
}

void ScheduleModel::setRoom(const QString& value) {
    Room = value;
    Refresh();
}

ScheduleNode ScheduleModel::getNode(const QModelIndex &index) const {
    for(auto iter = Nodes.begin(); iter != Nodes.end(); ++iter) {
        if (iter->Room == getRoom() && iter->Slot == index.row() && (iter->Day - getMinDay()) == index.column()) {
            return (*iter);
        }
    }
    return ScheduleNode((Qt::DayOfWeek)(getMinDay() + index.column()), (quint8)index.row(), Room, QString(), QString(), QString());
}

QString ScheduleModel::toString(Qt::DayOfWeek day) const {
    switch (day) {
        case Qt::DayOfWeek::Monday: return tr("Monday");
        case Qt::DayOfWeek::Tuesday: return tr("Tuesday");
        case Qt::DayOfWeek::Wednesday: return tr("Wednesday");
        case Qt::DayOfWeek::Thursday: return tr("Thursday");
        case Qt::DayOfWeek::Friday: return tr("Friday");
        case Qt::DayOfWeek::Saturday: return tr("Saturday");
        case Qt::DayOfWeek::Sunday: return tr("Sunday");
    }
    return QString("&Invalid");
}

void ScheduleModel::Refresh(void) {
    emit dataChanged(index(0, 0), index(rowCount(QModelIndex()), columnCount(QModelIndex())), QVector<int>());
}

