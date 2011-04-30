#include <QObject>
#include <QUrl>
#include <QImage>

#include <sobject.h>

class Contact : public QObject
{
    Q_OBJECT

public:
    explicit Contact(QObject *parent = 0);
    explicit Contact(const SObject &object, QObject *parent);
    virtual ~Contact() { }

    Q_PROPERTY(QString firstName READ firstName WRITE setFirstName NOTIFY firstNameChanged);
    QString firstName() const;
    void setFirstName(const QString &firstName);

    Q_PROPERTY(QString lastName READ lastName WRITE setLastName NOTIFY lastNameChanged);
    QString lastName() const;
    void setLastName(const QString &lastName);

    Q_PROPERTY(QString phoneNumber READ phoneNumber WRITE setPhoneNumber NOTIFY phoneNumberChanged);
    QString phoneNumber() const;
    void setPhoneNumber(const QString &phoneNumber);

    Q_PROPERTY(QImage avatar READ avatar WRITE setAvatar NOTIFY avatarChanged);
    QImage avatar() const;
    void setAvatar(const QImage &avatar);
    Q_INVOKABLE void setAvatar(const QUrl &avatarPath);

    Q_PROPERTY(QString localId READ localId);
    QString localId() const;

public slots:
    void remove();
    void save();

signals:
    void firstNameChanged();
    void lastNameChanged();
    void phoneNumberChanged();
    void avatarChanged();

private:
    SObject mData;
};

