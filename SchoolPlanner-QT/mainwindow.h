#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ScheduleModel.h"

#include <QContextMenuEvent>
#include <QStringListModel>
#include <QApplication>
#include <QMainWindow>
#include <QTableView>
#include <QClipboard>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void Cell_DblClicked(const QModelIndex& index);
    void Room_Changed(int index);

    void File_Load(void);
    void File_Save(void);
    void File_Drop(void);

    void Clip_Cut(void);
    void Clip_Copy(void);
    void Clip_Paste(void);

    void Edit_Rooms(void);
    void Edit_Groups(void);
    void Edit_Courses(void);
    void Edit_Teachers(void);

#ifndef QT_NO_CONTEXTMENU
    void Edit_Context(const QPoint &pos);
#endif

private:
    void initializeComponent(void);
    void disposeComponent(void);
    void resetModels(void);

    QAction* EditCut;
    QAction* EditCopy;
    QAction* EditPaste;

    ScheduleData Data;
    QTableView* NodesView;
    ScheduleModel* NodesModel;
    QStringListModel* RoomsModel;
};
#endif // MAINWINDOW_H
