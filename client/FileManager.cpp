
#include "FileManager.h"

const QString FileManager::getPathOfStyleSheet(const QString &name)
{
    return getPathOfResource("Styles", name, "qss");
}

const QString FileManager::getPathOfMap(const QString &name)
{
    return getPathOfResource("Maps", name, "rsm");
}

const QString FileManager::getPathOfTexture(const QString &name)
{
    return getPathOfResource("Textures", name, "obj");
}

const QString FileManager::getPathOfGLObject(const QString &name)
{
    return getPathOfResource("Objects", name, "obj");
}

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

