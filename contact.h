#include <QObject>

#include <sobject.h>

class Contact : public QObject
{
    Q_OBJECT

public:
    explicit Contact(QObject *parent = 0);
    explicit Contact(const SObject &object, QObject *parent);
    virtual ~Contact() { }

    QString displayLabel() const;

    SObject data() const;

    Q_PROPERTY(QString firstName READ firstName WRITE setFirstName NOTIFY firstNameChanged);
    QString firstName() const;
    void setFirstName(const QString &firstName);

    Q_PROPERTY(QString lastName READ lastName WRITE setLastName NOTIFY lastNameChanged);
    QString lastName() const;
    void setLastName(const QString &lastName);

    Q_PROPERTY(QString phoneNumber READ phoneNumber WRITE setPhoneNumber NOTIFY phoneNumberChanged);
    QString phoneNumber() const;
    void setPhoneNumber(const QString &phoneNumber);

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

