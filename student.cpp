#include "student.h"
#include <QStringList>

Student::Student(const QString &id, const QString &name, const QString &busNumber)
    : m_id(id), m_name(name), m_busNumber(busNumber)
{
}

QString Student::id() const
{
    return m_id;
}

QString Student::name() const
{
    return m_name;
}

QString Student::busNumber() const
{
    return m_busNumber;
}

void Student::setBusNumber(const QString &busNumber)
{
    m_busNumber = busNumber;
}

QString Student::toDataString() const
{
    // Save each student as a single line separated by '|'
    return m_id + '|' + m_name + '|' + m_busNumber;
}

Student Student::fromDataString(const QString &line)
{
    QStringList parts = line.split('|');
    if (parts.size() != 3) {
        return Student();
    }
    return Student(parts.at(0).trimmed(), parts.at(1).trimmed(), parts.at(2).trimmed());
}
