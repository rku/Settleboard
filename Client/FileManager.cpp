
#include "FileManager.h"

const QString FileManager::getPathOfResource(const QString &group,
        const QString &name, const QString &type)
{
    QString filename("%1.%2");
    return FileManager::getPathOfResource(group, filename.arg(name, type));
}

const QString FileManager::getPathOfResource(const QString &group,
    const QString &filename)
{
    QString path("%1/%2/%3");
    return path.arg(FileManager::getResourceRootPath(), group, filename);
}

