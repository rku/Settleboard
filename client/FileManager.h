
#ifndef FILEMANAGER_H
#define FILEMANAGER_H 1

#include <QString>

class FileManager
{
    public:
    static const QString getPathOfStyleSheet(const QString &name);
    static const QString getPathOfMap(const QString &name);
    static const QString getPathOfTexture(const QString &name);
    static const QString getPathOfGLObject(const QString &name);
    static const QString getPathOfResource(const QString &group,
        const QString &name, const QString &type);
    static const QString getPathOfResource(const QString &group,
        const QString &filename);
    static const QString getResourceRootPath() { return QString("Data"); }
};

#endif

