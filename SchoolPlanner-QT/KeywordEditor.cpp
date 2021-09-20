#include "KeywordEditor.h"

#include <QInputDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>

KeywordEditor::KeywordEditor(QWidget* parent) : QDialog(parent) {
    initializeComponent();
}

KeywordEditor::~KeywordEditor(void) {
    disposeComponent();
}

QStringList KeywordEditor::getKeywords(void) const {
    return Model->stringList();
}

void KeywordEditor::setKeywords(const QStringList& list) {
    Model->setStringList(list);
}

void KeywordEditor::Add_Click(void) {
    bool success;
    auto Title = QString("Add Keyword");
    auto Label = QString("Keyword:");
    auto Keyword = QInputDialog::getText(this, Title, Label, QLineEdit::Normal, QString(), &success);
    if (success) {
        if (Model->insertRow(Model->rowCount())) {
            auto index = Model->index(Model->rowCount() - 1, 0);
            Model->setData(index, Keyword);
        }
    }
}

void KeywordEditor::Remove_Click(void) {
     auto Selection = View->selectionModel()->selectedIndexes();
     if (Selection.count() == 1) {
         Model->removeRow(Selection[0].row());
     }
}

void KeywordEditor::Accept_Click(void) {
    accept();
}

void KeywordEditor::Cancel_Click(void) {
    reject();
}

void KeywordEditor::initializeComponent(void) {
    setWindowTitle(tr("Keyword Editor"));

    Model = new QStringListModel;
    View = new QListView;

    auto VLayout = new QVBoxLayout;
    auto HLayout = new QHBoxLayout;
    auto AddButton = new QPushButton(tr("&Add"));
    auto RemoveButton = new QPushButton(tr("&Remove"));
    auto AcceptButton = new QPushButton(tr("&Accept"));
    auto CancelButton = new QPushButton(tr("&Cancel"));

    View->setModel(Model);

    HLayout->addWidget(AddButton);
    HLayout->addWidget(RemoveButton);
    HLayout->addWidget(AcceptButton);
    HLayout->addWidget(CancelButton);

    VLayout->addWidget(View);
    VLayout->addLayout(HLayout);

    setLayout(VLayout);

    connect(AddButton, &QPushButton::released, this, &KeywordEditor::Add_Click);
    connect(RemoveButton, &QPushButton::released, this, &KeywordEditor::Remove_Click);
    connect(AcceptButton, &QPushButton::released, this, &KeywordEditor::Accept_Click);
    connect(CancelButton, &QPushButton::released, this, &KeywordEditor::Cancel_Click);
}

void KeywordEditor::disposeComponent(void) {
    if (Model != nullptr) {
        delete Model;
        Model = nullptr;
    }
}
