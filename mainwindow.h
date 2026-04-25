#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>

class QLineEdit;
class QPushButton;
class QTableWidget;

#include "student.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void addStudent();
    void updateBus();
    void deleteStudent();
    void searchStudent();
    void displayStudents();
    void displayBusWise();

private:
    void createUI();
    void loadStudentsFromFile();
    void saveStudentsToFile() const;
    void clearInputs();
    void showTableRows(const QVector<Student> &studentList);
    QString storageFilePath() const;

    QLineEdit *m_idEdit;
    QLineEdit *m_nameEdit;
    QLineEdit *m_busEdit;
    QLineEdit *m_searchIdEdit;
    QLineEdit *m_busFilterEdit;

    QPushButton *m_addButton;
    QPushButton *m_updateButton;
    QPushButton *m_deleteButton;
    QPushButton *m_searchButton;
    QPushButton *m_showAllButton;
    QPushButton *m_busWiseButton;

    QTableWidget *m_tableWidget;
    QVector<Student> m_students;
};

#endif // MAINWINDOW_H
