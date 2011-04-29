#include <QObject>

#include <sobject.h>

class Contact : public QObject
{
    Q_OBJECT

public:
    explicit Contact(const SObject &object, QObject *parent);
    virtual ~Contact() { }

    Q_PROPERTY(QVariant firstName READ firstName WRITE setFirstName NOTIFY firstNameChanged);
    QVariant firstName() const;
    void setFirstName(const QVariant &firstName);

    Q_PROPERTY(QVariant lastName READ lastName WRITE setLastName NOTIFY lastNameChanged);
    QVariant lastName() const;
    void setLastName(const QVariant &lastName);

    Q_PROPERTY(QVariant phoneNumber READ phoneNumber WRITE setPhoneNumber NOTIFY phoneNumberChanged);
    QVariant phoneNumber() const;
    void setPhoneNumber(const QVariant &phoneNumber);

public slots:
    void remove();
    void save();

signals:
    void firstNameChanged();
    void lastNameChanged();
    void phoneNumberChanged();

private:
    SObject mData;
};

