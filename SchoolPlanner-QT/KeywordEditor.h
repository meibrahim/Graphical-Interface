#ifndef KEYWORDEDITOR_H
#define KEYWORDEDITOR_H

#include <QDialog>
#include <QListView>
#include <QStringList>
#include <QStringListModel>

class KeywordEditor : public QDialog {
    Q_OBJECT

public:
    KeywordEditor(QWidget* parent = nullptr);
    ~KeywordEditor(void);

    QStringList getKeywords(void) const;
    void setKeywords(const QStringList& list);

private slots:
    void Add_Click(void);
    void Remove_Click(void);
    void Accept_Click(void);
    void Cancel_Click(void);

private:
    QListView* View;
    QStringListModel* Model;

    void initializeComponent(void);
    void disposeComponent(void);
};


#endif // KEYWORDEDITOR_H
