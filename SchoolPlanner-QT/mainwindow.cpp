#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // DEBUG/TEST DATA
    Data.Courses.append(QString("EADS"));
    Data.Courses.append(QString("EGUI"));
    Data.Courses.append(QString("ELAN"));
    Data.Courses.append(QString("ECOAR"));

    Data.Rooms.append(QString("128-B"));
    Data.Rooms.append(QString("128-A"));
    Data.Rooms.append(QString("62"));
    Data.Rooms.append(QString("11"));

    Data.Teachers.append(QString("John Doe"));
    Data.Teachers.append(QString("Jane Doe"));

    Data.Groups.append(QString("1a"));
    Data.Groups.append(QString("1b"));
    Data.Groups.append(QString("2a"));
    Data.Groups.append(QString("2b"));
    // !DEBUG/TEST DATA

    initializeComponent();
}

MainWindow::~MainWindow(void)
{
    disposeComponent();
}

#include <QWidget>
#include <QMenuBar>
#include <QComboBox>
#include <QTableView>
#include <QFileDialog>
#include <QFormLayout>
#include <QVBoxLayout>
#include "KeywordEditor.h"
#include "ScheduleEditor.h"

void MainWindow::Cell_DblClicked(const QModelIndex& cell) {
    ScheduleEditor editor(this);
    auto node = NodesModel->getNode(cell);
    auto index = Data.Nodes.indexOf(node);

    if (index == -1) {
        if (Data.Groups.count() > 0) {
            node.Group = Data.Groups.first();
        }
        if (Data.Courses.count() > 0) {
            node.Course = Data.Courses.first();
        }
        if (Data.Teachers.count() > 0) {
            node.Teacher = Data.Teachers.first();
        }
    }

    editor.setTeachers(Data.Teachers);
    editor.setCourses(Data.Courses);
    editor.setGroups(Data.Groups);
    editor.setNode(node);
    auto result = editor.exec();

    switch (result) {
        case ScheduleEditor::Accepted:
            node = editor.getNode();
            if (index > -1) {
                Data.Nodes.removeAt(index);
            }
            Data.Nodes.append(node);
            NodesModel->setNodes(Data.Nodes);
            break;
        case ScheduleEditor::Removal:
            if (index > -1) {
                Data.Nodes.removeAt(index);
            }
            NodesModel->setNodes(Data.Nodes);
            break;
    }
}

void MainWindow::Room_Changed(int index) {
    if (Data.Rooms.count() > index && index > -1) {
        NodesModel->setRoom(Data.Rooms[index]);
    } else {
        NodesModel->setRoom(QString());
    }
}

void MainWindow::File_Load(void) {
    resetModels();

    auto path = QFileDialog::getOpenFileName(this);
    Data.LoadJSON(path);

    if (Data.Rooms.count() > 0) {
        NodesModel->setRoom(Data.Rooms.first());
    }
    NodesModel->setNodes(Data.Nodes);
    RoomsModel->setStringList(Data.Rooms);
}

void MainWindow::File_Save(void) {
    auto path = QFileDialog::getSaveFileName(this);
    Data.SaveJSON(path);
}

void MainWindow::File_Drop(void) {
    resetModels();
}

void MainWindow::Clip_Cut(void) {
    auto Selection = NodesView->selectionModel()->selection();
    if (Selection.count() == 1) {
        auto indices = Selection.first().indexes();
        if (indices.count() == 1) {
            auto cell = indices.first();
            auto node = NodesModel->getNode(cell);

            QJsonObject obj;
            node.SaveJSON(obj);

            auto *clipboard = QApplication::clipboard();

            QJsonDocument doc(obj);
            auto bytes = doc.toJson();
            clipboard->setText(bytes);

            Data.Nodes.removeAt(Data.Nodes.indexOf(node));
            NodesModel->setNodes(Data.Nodes);
        }
    }
}

void MainWindow::Clip_Copy(void) {
    auto Selection = NodesView->selectionModel()->selection();
    if (Selection.count() == 1) {
        auto indices = Selection.first().indexes();
        if (indices.count() == 1) {
            auto cell = indices.first();
            auto node = NodesModel->getNode(cell);

            QJsonObject obj;
            node.SaveJSON(obj);

            auto *clipboard = QApplication::clipboard();

            QJsonDocument doc(obj);
            auto bytes = doc.toJson();
            clipboard->setText(bytes);
        }
    }
}

void MainWindow::Clip_Paste(void) {
    auto Selection = NodesView->selectionModel()->selection();
    if (Selection.count() == 1) {
         auto indices = Selection.first().indexes();

         if (indices.count() == 1) {
             auto cell = indices.first();
             auto node = NodesModel->getNode(cell);
             auto index = Data.Nodes.indexOf(node);

             const QClipboard *clipboard = QApplication::clipboard();
             auto data = clipboard->text();
             auto json = QJsonDocument::fromJson(data.toUtf8());
             auto obj = json.object();
             node = ScheduleNode(obj);

             node.Slot = cell.row();
             node.Room = NodesModel->getRoom();
             node.Day = (Qt::DayOfWeek)(cell.column() + NodesModel->getMinDay());

             if (index > -1 && index < Data.Nodes.count()) {
                 Data.Nodes[index] = node;
             } else {
                 Data.Nodes.append(node);
             }

             NodesModel->setNodes(Data.Nodes);
         }
    }
}

void MainWindow::Edit_Rooms(void) {
    KeywordEditor editor(this);
    editor.setKeywords(Data.Rooms);
    if (editor.exec() == QDialog::Accepted) {
        Data.Rooms = editor.getKeywords();
        if (RoomsModel->rowCount() > 0) {
            RoomsModel->removeRows(0, RoomsModel->rowCount());
        }
        RoomsModel->setStringList(Data.Rooms);
    }
}

void MainWindow::Edit_Groups(void) {
    KeywordEditor editor(this);
    editor.setKeywords(Data.Groups);
    if (editor.exec() == QDialog::Accepted) {
        Data.Groups = editor.getKeywords();
    }
}

void MainWindow::Edit_Courses(void) {
    KeywordEditor editor(this);
    editor.setKeywords(Data.Courses);
    if (editor.exec() == QDialog::Accepted) {
        Data.Courses = editor.getKeywords();
    }
}

void MainWindow::Edit_Teachers(void) {
    KeywordEditor editor(this);
    editor.setKeywords(Data.Teachers);
    if (editor.exec() == QDialog::Accepted) {
        Data.Teachers = editor.getKeywords();
    }
}

#ifndef QT_NO_CONTEXTMENU
void MainWindow::Edit_Context(const QPoint &pos)
{
    QMenu menu(NodesView);
    menu.addAction(EditCut);
    menu.addAction(EditCopy);
    menu.addAction(EditPaste);
    menu.exec(NodesView->viewport()->mapToGlobal(pos));
}
#endif // QT_NO_CONTEXTMENU

void MainWindow::initializeComponent(void) {
    //
    setWindowTitle(tr("Schedule Editor"));
    resize(800, 600);

    // Host all the widgets on the form
    auto Layout = new QVBoxLayout;
    auto Host = new QWidget;
    Host->setLayout(Layout);
    this->setCentralWidget(Host);
    //

    // Room Selector
    auto Room = new QComboBox;
    auto RoomForm = new QFormLayout;
    RoomsModel = new QStringListModel;
    RoomsModel->setStringList(Data.Rooms);
    RoomForm->addRow(tr("&Room:"), Room);
    Layout->addLayout(RoomForm);
    Room->setModel(RoomsModel);

    connect(Room, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::Room_Changed);
    //

    //
    // Schedule Table
    NodesView = new QTableView;
    NodesView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(NodesView, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(Edit_Context(const QPoint &)));

    NodesModel = new ScheduleModel;
    NodesView->setModel(NodesModel);
    NodesView->setSelectionMode(QAbstractItemView::SingleSelection);
    Layout->addWidget(NodesView);

    if (Data.Rooms.count() > 0) {
        NodesModel->setRoom(Data.Rooms.first());
    }

    connect(NodesView, &QTableView::doubleClicked, this, &MainWindow::Cell_DblClicked);
    //

    //
    // Create the menu bar and it's items
    //
    // File Menu
    auto FileMenu = menuBar()->addMenu(tr("&File"));
    auto FileLoad = new QAction(tr("&Load"), this);
    auto FileSave = new QAction(tr("&Save"), this);
    auto FileDrop = new QAction(tr("&Drop"), this);
    auto FileExit = new QAction(tr("&Exit"), this);
    FileMenu->addAction(FileLoad);
    FileMenu->addAction(FileSave);
    FileMenu->addAction(FileDrop);
    FileMenu->addAction(FileExit);
    connect(FileLoad, &QAction::triggered, this, &MainWindow::File_Load);
    connect(FileSave, &QAction::triggered, this, &MainWindow::File_Save);
    connect(FileDrop, &QAction::triggered, this, &MainWindow::File_Drop);
    connect(FileExit, &QAction::triggered, this, &QWidget::close);
    //
    // Edit Menu
    auto EditMenu = menuBar()->addMenu(tr("&Edit"));
    EditCut = new QAction(tr("&Cut"), this);
    EditCopy = new QAction(tr("&Copy"), this);
    EditPaste = new QAction(tr("&Paste"), this);
    EditMenu->addAction(EditCut);
    EditMenu->addAction(EditCopy);
    EditMenu->addAction(EditPaste);
    EditCut->setShortcuts(QKeySequence::Cut);
    EditCopy->setShortcuts(QKeySequence::Copy);
    EditPaste->setShortcuts(QKeySequence::Paste);
    connect(EditCut, &QAction::triggered, this, &MainWindow::Clip_Cut);
    connect(EditCopy, &QAction::triggered, this, &MainWindow::Clip_Copy);
    connect(EditPaste, &QAction::triggered, this, &MainWindow::Clip_Paste);
    //
    // Keywords sub-menu
    auto DictMenu = EditMenu->addMenu(tr("&Keywords"));
    auto DictRooms = new QAction(tr("&Rooms"), this);
    auto DictGroups = new QAction(tr("&Groups"), this);
    auto DictCourses = new QAction(tr("&Courses"), this);
    auto DictTeachers = new QAction(tr("&Teachers"), this);
    DictMenu->addAction(DictRooms);
    DictMenu->addAction(DictGroups);
    DictMenu->addAction(DictCourses);
    DictMenu->addAction(DictTeachers);
    connect(DictRooms, &QAction::triggered, this, &MainWindow::Edit_Rooms);
    connect(DictGroups, &QAction::triggered, this, &MainWindow::Edit_Groups);
    connect(DictCourses, &QAction::triggered, this, &MainWindow::Edit_Courses);
    connect(DictTeachers, &QAction::triggered, this, &MainWindow::Edit_Teachers);
    //
}

void MainWindow::disposeComponent(void) {
    if (NodesModel != nullptr) {
        delete NodesModel;
        NodesModel = nullptr;
    }
    if (RoomsModel != nullptr) {
        delete RoomsModel;
        RoomsModel = nullptr;
    }
}

void MainWindow::resetModels(void) {
    Data = ScheduleData();
    NodesModel->setRoom(QString());
    NodesModel->setNodes(Data.Nodes);
    RoomsModel->removeRows(0, RoomsModel->rowCount());
}
