#ifndef SCHEDULEMODEL_H
#define SCHEDULEMODEL_H

#include "ScheduleData.h"

#include <QTime>
#include <QAbstractTableModel>

class ScheduleModel : public QAbstractTableModel {
    Q_OBJECT

public:
    ScheduleModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    QString getRoom(void) const;
    NodeList getNodes(void) const;
    quint8 getMinHour(void) const;
    quint8 getMaxHour(void) const;
    Qt::DayOfWeek getMinDay(void) const;
    Qt::DayOfWeek getMaxDay(void) const;

    void setNodes(const NodeList& nodes);
    void setMinDay(Qt::DayOfWeek value);
    void setMaxDay(Qt::DayOfWeek value);
    void setRoom(const QString& value);
    void setMinHour(quint8 value);
    void setMaxHour(quint8 value);

    ScheduleNode getNode(const QModelIndex &index) const;

protected:
    QString toString(Qt::DayOfWeek day) const;

private:
    QString Room;
    NodeList Nodes;
    quint8 MinHour;
    quint8 MaxHour;
    Qt::DayOfWeek MinDay;
    Qt::DayOfWeek MaxDay;

    void Refresh(void);
};

#endif // SCHEDULEMODEL_H
