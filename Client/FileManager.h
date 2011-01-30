
#ifndef FILEMANAGER_H
#define FILEMANAGER_H 1

#include <QString>

class FileManager
{
    public:
    FileManager();

    static const QString getPathOfResource(const QString &group,
        const QString &name, const QString &type);
    static const QString getPathOfResource(const QString &group,
        const QString &filename);
    static const QString getResourceRootPath() { return QString("Data"); }
};

#endif

