#ifndef KANDATACONNECTOR_H
#define KANDATACONNECTOR_H

#include <QString>
#include <QColor>

class KanDataConnector
{
private:
    KanDataConnector(void) {}
public:
    static KanDataConnector& getInstance(void) {
        static KanDataConnector singleton;
        singleton.colWhite = QColor(255, 255, 255);
        singleton.colGray = QColor(128, 128, 128);
        singleton.colOrange = QColor(255, 153, 0);
        singleton.colYellow = QColor(255, 255, 0);
        singleton.colRed = QColor(255, 0, 0);
        singleton.colBlue = QColor(0, 0, 255);
        return singleton;
    }

public:
    void Parse(QString pathAndQuery, QString requestBody, QString responseBody);

private:
    void updateOverviewTable();
    void updateMissionTable();
    void updateRepairTable();
    void updateFleetTable();
    void updateExpeditionTable();
    void updateRepairDockTable();
    void updateBuildDockTable();

    QColor colWhite;
    QColor colGray;
    QColor colOrange;
    QColor colYellow;
    QColor colRed;
    QColor colBlue;
};

#endif // KANDATACONNECTOR_H
