#include "mydbdelegate.h"
#include "mydbdelegateplugin.h"

#include <QtPlugin>

MyDBDelegatePlugin::MyDBDelegatePlugin(QObject *parent)
    : QObject(parent)
{
    m_initialized = false;
}

void MyDBDelegatePlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (m_initialized)
        return;

    // Add extension registrations, etc. here

    m_initialized = true;
}

bool MyDBDelegatePlugin::isInitialized() const
{
    return m_initialized;
}

QWidget *MyDBDelegatePlugin::createWidget(QWidget *parent)
{
    return new MyDBDelegate(parent);
}

QString MyDBDelegatePlugin::name() const
{
    return QLatin1String("MyDBDelegate");
}

QString MyDBDelegatePlugin::group() const
{
    return QLatin1String("");
}

QIcon MyDBDelegatePlugin::icon() const
{
    return QIcon();
}

QString MyDBDelegatePlugin::toolTip() const
{
    return QLatin1String("");
}

QString MyDBDelegatePlugin::whatsThis() const
{
    return QLatin1String("");
}

bool MyDBDelegatePlugin::isContainer() const
{
    return false;
}

QString MyDBDelegatePlugin::domXml() const
{
    return QLatin1String("<widget class=\"MyDBDelegate\" name=\"myDBDelegate\">\n</widget>\n");
}

QString MyDBDelegatePlugin::includeFile() const
{
    return QLatin1String("mydbdelegate.h");
}
#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(mydbdelegateplugin, MyDBDelegatePlugin)
#endif // QT_VERSION < 0x050000
