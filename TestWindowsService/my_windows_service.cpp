#include <QDebug>
#include <QDir>
#include <QStandardPaths>

#include "message_handler.h"
#include "my_windows_service.h"

#ifdef Q_OS_WIN
#include <windows.h>
#include <Wtsapi32.h>
#endif

#include <QProcess>


MyWindowsService::MyWindowsService(int argc, char** argv)
    : QtService<QCoreApplication>(argc, argv, "My Windows Service")
{
    setServiceDescription("This is my Windows Service");
    setServiceFlags(QtServiceBase::CanBeSuspended);
}


void MyWindowsService::installMessageHandler()
{
    // Install Message Handler
    QString logDirName = "C:/ProgramData/MyWindowsService";
    QDir logDir(logDirName);

    if (!logDir.exists())
    {
        qDebug() << "Creating folder: " << logDirName;
        QDir().mkdir(logDirName);
    }

    QString logFile = logDirName + "/mws.log";

    // Remove the Macro defined in winbase.h
    #ifdef FormatMessage
    #undef FormatMessage
        message_handler::MessageHandler::setLogFile(logFile);
        qInstallMessageHandler(message_handler::MessageHandler::FormatMessage);
    #endif
}


void MyWindowsService::way1_GetUserNameByQT()
{
    // In the system level process/service,
    // the UserName is "<HostName>$", e.g. "FINIX-LAPTOP$".

    qInfo() << "Way 1 - Get User Name via QT...";
    qInfo() << "User Name = " << qgetenv("UserName");
    qInfo() << "---------------------------";
}


void MyWindowsService::way2_GetUserNameByWindowsAPI()
{
    #ifdef Q_OS_WIN
    // Output: Windows User Name = "SYSTEM"

        qInfo() << "Way 2 - Get User Name via Windows API GetUserName()...";

        wchar_t acUserName[200];
        DWORD nUserName = sizeof(acUserName);
        if (GetUserName(acUserName, &nUserName))
        {
            // Translate wchar_t * to QString
            qInfo() << "Windows User Name = " << QString::fromWCharArray(acUserName);
        }
    #elif Q_OS_UNIX
        QCoreApplication coreApplication(argc, argv);
        QProcess process;
        QObject::connect(&process, &QProcess::finished, [&coreApplication, &process](int exitCode, QProcess::ExitStatus exitStatus) {
            qDebug() << process.readAllStandardOutput();
            coreApplication.quit();
        });
        process.start("whoami");
    #endif
    qInfo() << "---------------------------";
}



void MyWindowsService::way3_GetActiveUserNameByWindowsAPIInSystemProcess()
{
    // Output: Windows User Name = "Finix"

    qInfo() << "Way 3 - Get Active User Name from a system process...";
    DWORD sessionId = WTSGetActiveConsoleSessionId();
    qInfo() << "session id = " << sessionId;

    wchar_t* ppBuffer[100];
    DWORD bufferSize;
    WTSQuerySessionInformation(WTS_CURRENT_SERVER_HANDLE, sessionId, WTSUserName, ppBuffer, &bufferSize);
    qInfo() << "Windows User Name = " << QString::fromWCharArray(*ppBuffer);

    qInfo() << "---------------------------";
}


void MyWindowsService::way4_GetHomeLocationByQStandardPaths()
{
    // Output: "C:/Windows/system32/config/systemprofile"

    qInfo() << "Way 4 - Get Home Location via QStandardPaths...";
    QStringList homePath = QStandardPaths::standardLocations(QStandardPaths::HomeLocation);
    qInfo() << homePath.first().split(QDir::separator()).last();
    qInfo() << "---------------------------";
}


void MyWindowsService::way5_GetHomeLocationByWindowsAPI()
{
    // Output: "C:\\Windows\\system32\\config\\systemprofile"

    qInfo() << "Way 5 - Get Home Location via Windows API...";
    wchar_t envVar[] = L"UserProfile";
    wchar_t result[100];
    GetEnvironmentVariable(envVar, result, sizeof(result)/sizeof(wchar_t));
    qInfo() << "USER PROFILE = " << QString::fromWCharArray(result);
    qInfo() << "---------------------------";
}


void MyWindowsService::way6_GetHomeLoactioneByWTS()
{
    // Output:

    qInfo() << "Way 6 - Get Home Location via WTS technique...";
    DWORD sessionId = WTSGetActiveConsoleSessionId();
    qInfo() << "Session ID = " << sessionId;
    wchar_t* ppBuffer[100];
    DWORD bufferSize;

    WTSQuerySessionInformation(WTS_CURRENT_SERVER_HANDLE, sessionId, WTSWorkingDirectory, ppBuffer, &bufferSize);
    qInfo() << "Working Directory = " << QString::fromWCharArray(*ppBuffer);  // Empty ""

    WTSQuerySessionInformation(WTS_CURRENT_SERVER_HANDLE, sessionId, WTSClientDirectory, ppBuffer, &bufferSize);
    qInfo() << "Client Directory = " << QString::fromWCharArray(*ppBuffer);  // Empty ""

    WTSQuerySessionInformation(WTS_CURRENT_SERVER_HANDLE, sessionId, WTSUserName, ppBuffer, &bufferSize);
    qInfo() << "Windows User Name = " << QString::fromWCharArray(*ppBuffer);  // Right user name

    qInfo() << "---------------------------";
}


void MyWindowsService::start()
{
    // Only for "-e" option, as message handler has not been installed
    qDebug() << "Get into start().......";

    installMessageHandler();

    qInfo() << "Start MyWindowsService...";

    way1_GetUserNameByQT();
    way2_GetUserNameByWindowsAPI();
    way3_GetActiveUserNameByWindowsAPIInSystemProcess();
    way4_GetHomeLocationByQStandardPaths();
    way5_GetHomeLocationByWindowsAPI();
    way6_GetHomeLoactioneByWTS();
}

void MyWindowsService::stop()
{
    qInfo() << "Stop MyWindowsService...";
}

void MyWindowsService::resume()
{
    qInfo() << "Resume MyWindowsService...";
}

void MyWindowsService::pause()
{
    qInfo() << "Pause MyWindowsService...";
}
