#ifndef STUDENT_H
#define STUDENT_H

#include <QString>

class Student {
public:
    Student() = default;
    Student(const QString &id, const QString &name, const QString &busNumber);

    QString id() const;
    QString name() const;
    QString busNumber() const;

    void setBusNumber(const QString &busNumber);

    QString toDataString() const; // Format for file storage
    static Student fromDataString(const QString &line);

private:
    QString m_id;
    QString m_name;
    QString m_busNumber;
};

#endif // STUDENT_H
