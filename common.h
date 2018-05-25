#ifndef COMMON_H
#define COMMON_H

#include <QObject>

struct RawHeader
{
    QByteArray name;
    QByteArray value;
};

class Common
{
public:
    Common();
};

#endif // COMMON_H
