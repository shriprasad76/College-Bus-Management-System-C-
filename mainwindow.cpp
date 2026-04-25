#include "mainwindow.h"

#include <QApplication>
#include <QDir>
#include <QFile>
#include <QGridLayout>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QAbstractItemView>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      m_idEdit(new QLineEdit(this)),
      m_nameEdit(new QLineEdit(this)),
      m_busEdit(new QLineEdit(this)),
      m_searchIdEdit(new QLineEdit(this)),
      m_busFilterEdit(new QLineEdit(this)),
      m_addButton(new QPushButton("Add Student", this)),
      m_updateButton(new QPushButton("Update Bus", this)),
      m_deleteButton(new QPushButton("Delete Student", this)),
      m_searchButton(new QPushButton("Search by ID", this)),
      m_showAllButton(new QPushButton("Show All", this)),
      m_busWiseButton(new QPushButton("Show Bus-Wise", this)),
      m_tableWidget(new QTableWidget(this))
{
    createUI();
    loadStudentsFromFile();
    displayStudents();
}

MainWindow::~MainWindow() = default;

void MainWindow::createUI()
{
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QLabel *idLabel = new QLabel("Student ID:", this);
    QLabel *nameLabel = new QLabel("Student Name:", this);
    QLabel *busLabel = new QLabel("Bus Number:", this);
    QLabel *searchLabel = new QLabel("Search ID:", this);
    QLabel *busFilterLabel = new QLabel("Filter Bus #:", this);

    m_tableWidget->setColumnCount(3);
    m_tableWidget->setHorizontalHeaderLabels({"Student ID", "Name", "Bus Number"});
    m_tableWidget->horizontalHeader()->setStretchLastSection(true);
    m_tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QGridLayout *gridLayout = new QGridLayout(centralWidget);
    gridLayout->addWidget(idLabel, 0, 0);
    gridLayout->addWidget(m_idEdit, 0, 1);
    gridLayout->addWidget(nameLabel, 0, 2);
    gridLayout->addWidget(m_nameEdit, 0, 3);
    gridLayout->addWidget(busLabel, 0, 4);
    gridLayout->addWidget(m_busEdit, 0, 5);

    gridLayout->addWidget(m_addButton, 1, 0, 1, 2);
    gridLayout->addWidget(m_updateButton, 1, 2, 1, 2);
    gridLayout->addWidget(m_deleteButton, 1, 4, 1, 2);

    gridLayout->addWidget(searchLabel, 2, 0);
    gridLayout->addWidget(m_searchIdEdit, 2, 1);
    gridLayout->addWidget(m_searchButton, 2, 2, 1, 2);

    gridLayout->addWidget(busFilterLabel, 2, 4);
    gridLayout->addWidget(m_busFilterEdit, 2, 5);
    gridLayout->addWidget(m_busWiseButton, 3, 0, 1, 3);
    gridLayout->addWidget(m_showAllButton, 3, 3, 1, 3);

    gridLayout->addWidget(m_tableWidget, 4, 0, 1, 6);
    gridLayout->setColumnStretch(1, 1);
    gridLayout->setColumnStretch(3, 1);
    gridLayout->setColumnStretch(5, 1);

    connect(m_addButton, &QPushButton::clicked, this, &MainWindow::addStudent);
    connect(m_updateButton, &QPushButton::clicked, this, &MainWindow::updateBus);
    connect(m_deleteButton, &QPushButton::clicked, this, &MainWindow::deleteStudent);
    connect(m_searchButton, &QPushButton::clicked, this, &MainWindow::searchStudent);
    connect(m_showAllButton, &QPushButton::clicked, this, &MainWindow::displayStudents);
    connect(m_busWiseButton, &QPushButton::clicked, this, &MainWindow::displayBusWise);

    setWindowTitle("College Bus Management System");
    resize(900, 500);
}

QString MainWindow::storageFilePath() const
{
    return QDir(QCoreApplication::applicationDirPath()).filePath("students.txt");
}

void MainWindow::loadStudentsFromFile()
{
    QFile file(storageFilePath());
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        // No data file yet, continue with an empty list.
        return;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        const QString line = in.readLine().trimmed();
        if (line.isEmpty()) {
            continue;
        }
        Student student = Student::fromDataString(line);
        if (!student.id().isEmpty()) {
            m_students.append(student);
        }
    }
}

void MainWindow::saveStudentsToFile() const
{
    QFile file(storageFilePath());
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Save Error", "Unable to save student data.");
        return;
    }

    QTextStream out(&file);
    for (const Student &student : m_students) {
        out << student.toDataString() << '\n';
    }
}

void MainWindow::clearInputs()
{
    m_idEdit->clear();
    m_nameEdit->clear();
    m_busEdit->clear();
    m_searchIdEdit->clear();
    m_busFilterEdit->clear();
}

void MainWindow::showTableRows(const QVector<Student> &studentList)
{
    m_tableWidget->setRowCount(studentList.size());
    for (int row = 0; row < studentList.size(); ++row) {
        const Student &student = studentList.at(row);
        m_tableWidget->setItem(row, 0, new QTableWidgetItem(student.id()));
        m_tableWidget->setItem(row, 1, new QTableWidgetItem(student.name()));
        m_tableWidget->setItem(row, 2, new QTableWidgetItem(student.busNumber()));
    }
    m_tableWidget->resizeColumnsToContents();
}

void MainWindow::displayStudents()
{
    showTableRows(m_students);
}

void MainWindow::displayBusWise()
{
    const QString busFilter = m_busFilterEdit->text().trimmed();
    if (busFilter.isEmpty()) {
        QMessageBox::information(this, "Input Required", "Please enter a bus number to filter.");
        return;
    }

    QVector<Student> filtered;
    for (const Student &student : qAsConst(m_students)) {
        if (student.busNumber().compare(busFilter, Qt::CaseInsensitive) == 0) {
            filtered.append(student);
        }
    }

    if (filtered.isEmpty()) {
        QMessageBox::information(this, "No Results", "No students found for the selected bus number.");
    }
    showTableRows(filtered);
}

void MainWindow::addStudent()
{
    const QString id = m_idEdit->text().trimmed();
    const QString name = m_nameEdit->text().trimmed();
    const QString busNumber = m_busEdit->text().trimmed();

    if (id.isEmpty() || name.isEmpty() || busNumber.isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Please fill in all fields before adding a student.");
        return;
    }

    for (const Student &student : qAsConst(m_students)) {
        if (student.id().compare(id, Qt::CaseInsensitive) == 0) {
            QMessageBox::warning(this, "Duplicate ID", "A student with this ID already exists.");
            return;
        }
    }

    m_students.append(Student(id, name, busNumber));
    saveStudentsToFile();
    displayStudents();
    clearInputs();
    QMessageBox::information(this, "Success", "Student added successfully.");
}

void MainWindow::updateBus()
{
    const QString id = m_idEdit->text().trimmed();
    const QString newBus = m_busEdit->text().trimmed();

    if (id.isEmpty() || newBus.isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Please enter both Student ID and new Bus Number.");
        return;
    }

    bool updated = false;
    for (Student &student : m_students) {
        if (student.id().compare(id, Qt::CaseInsensitive) == 0) {
            student.setBusNumber(newBus);
            updated = true;
            break;
        }
    }

    if (!updated) {
        QMessageBox::warning(this, "Not Found", "Student ID not found for update.");
        return;
    }

    saveStudentsToFile();
    displayStudents();
    clearInputs();
    QMessageBox::information(this, "Success", "Bus number updated successfully.");
}

void MainWindow::deleteStudent()
{
    const QString id = m_idEdit->text().trimmed();
    if (id.isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Enter Student ID to delete.");
        return;
    }

    int index = -1;
    for (int i = 0; i < m_students.size(); ++i) {
        if (m_students.at(i).id().compare(id, Qt::CaseInsensitive) == 0) {
            index = i;
            break;
        }
    }

    if (index < 0) {
        QMessageBox::warning(this, "Not Found", "Student ID not found.");
        return;
    }

    m_students.remove(index);
    saveStudentsToFile();
    displayStudents();
    clearInputs();
    QMessageBox::information(this, "Deleted", "Student record deleted successfully.");
}

void MainWindow::searchStudent()
{
    const QString id = m_searchIdEdit->text().trimmed();
    if (id.isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Enter Student ID to search.");
        return;
    }

    QVector<Student> results;
    for (const Student &student : qAsConst(m_students)) {
        if (student.id().compare(id, Qt::CaseInsensitive) == 0) {
            results.append(student);
            break;
        }
    }

    if (results.isEmpty()) {
        QMessageBox::information(this, "Not Found", "No student found with the entered ID.");
        return;
    }

    showTableRows(results);
}
