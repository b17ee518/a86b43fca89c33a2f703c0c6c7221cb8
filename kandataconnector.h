#ifndef KANDATACONNECTOR_H
#define KANDATACONNECTOR_H

#include <QString>
#include <QColor>
#include "kandata.h"

class KanDataConnector
{
private:
    KanDataConnector(void) {}
public:
    static KanDataConnector& getInstance(void) {
        static KanDataConnector singleton;
        singleton.colWhite = QColor(255, 255, 255);
        singleton.colGray = QColor(153, 153, 153);
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
    void updateInfoTitle();

    void getShipColors(const kcsapi_ship2 *pship, QColor *pcolCond=0, QColor *pcolWound=0);
    void getShipChargeColors(const kcsapi_ship2 *pship, const kcsapi_mst_ship *pmstship, QColor *pcolFuel, QColor *pcolBullet);
    QString getShipWoundStateString(const kcsapi_ship2 *pship);
    bool isShipRepairing(const kcsapi_ship2 *pship);

    bool RemoveShip(int shipno);

    kcsapi_ship2 *findShipFromShipno(int shipno);
    const kcsapi_mst_ship *findMstShipFromShipid(int shipid) const;
    const Api_Mst_Mission *findMstMissionFromMissionid(int missionid) const;

    QColor colWhite;
    QColor colGray;
    QColor colOrange;
    QColor colYellow;
    QColor colRed;
    QColor colBlue;
};

#endif // KANDATACONNECTOR_H
