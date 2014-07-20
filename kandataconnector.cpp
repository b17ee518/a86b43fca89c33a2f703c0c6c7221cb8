#include "kandataconnector.h"
#include "kandata.h"
#include "kansavedata.h"
#include "kandatacalc.h"
#include <QJsonDocument>
#include <QJsonObject>

#include "infomainwindow.h"
#include "mainwindow.h"
#include "kqtime.h"

/*
 * End points
    "/kcsapi/api_start2",
    "/kcsapi/api_port/port",
    "/kcsapi/api_get_member/basic",
    "/kcsapi/api_get_member/ship",
    "/kcsapi/api_get_member/ship2",
    "/kcsapi/api_get_member/ship3",
    "/kcsapi/api_get_member/slot_item",
    "/kcsapi/api_get_member/useitem",
    "/kcsapi/api_get_member/deck",
    "/kcsapi/api_get_member/deck_port",
    "/kcsapi/api_get_member/ndock",
    "/kcsapi/api_get_member/kdock",
    "/kcsapi/api_get_member/material",
    "/kcsapi/api_get_member/questlist",
    "/kcsapi/api_req_hensei/change",
    "/kcsapi/api_req_kousyou/getship",
    "/kcsapi/api_req_kousyou/createitem",
    "/kcsapi/api_req_kousyou/createship",
    "/kcsapi/api_req_kousyou/createship_speedchange",
    "/kcsapi/api_req_kousyou/destroyship",
    "/kcsapi/api_req_kousyou/destroyitem2",
    "/kcsapi/api_req_nyukyo/start",
    "/kcsapi/api_req_nyukyo/speedchange",
    "/kcsapi/api_req_hokyu/charge",
    "/kcsapi/api_req_kaisou/powerup",
    "/kcsapi/api_req_member/updatedeckname",
    "/kcsapi/api_req_member/updatecomment",
    "/kcsapi/api_req_sortie/battle",
    "/kcsapi/api_req_sortie/battleresult",
*/


void KanDataConnector::Parse(QString pathAndQuery, QString requestBody, QString responseBody)
{
    // svdata=
    responseBody = responseBody.right(responseBody.length()-7);
    QJsonDocument jdoc = QJsonDocument::fromJson(responseBody.toLocal8Bit());
    QJsonObject jobj = jdoc.object()["api_data"].toObject();

    KanSaveData * pksd = &KanSaveData::getInstance();

    if (pathAndQuery == "/kcsapi/api_start2")
    {
        pksd->start2data.ReadFromJObj(jobj);
        // mst table no update
    }
    else if (pathAndQuery == "/kcsapi/api_port/port")
    {
        pksd->portdata.ReadFromJObj(jobj);
        // material, deck, ndock, ship2, basic
        updateOverviewTable();
        updateMissionTable();
        updateFleetTable();
        updateRepairTable();

        updateRepairDockTable();
        updateExpeditionTable();
    }
    else if (pathAndQuery == "/kcsapi/api_get_member/basic")
    {
        pksd->portdata.ReadFromJObj(jobj);
        updateOverviewTable();
    }
    else if (pathAndQuery == "/kcsapi/api_get_member/slot_item")
    {
        pksd->slotitemdata.clear();
        QJsonArray jarray = jdoc.object()["api_data"].toArray();
        for (int i=0; i<jarray.count(); i++)
        {
            kcsapi_slotitem api_slotitem;
            api_slotitem.ReadFromJObj(jarray[i].toObject());
            pksd->slotitemdata.append(api_slotitem);
        }

        updateOverviewTable();
        // update?
    }
    else if (pathAndQuery == "/kcsapi/api_get_member/kdock")
    {
        pksd->kenzoudata.ReadFromJObj(jobj);

        updateBuildDockTable();
    }
    else if (pathAndQuery == "/kcsapi/api_get_member/material")
    {
        pksd->materialdata.clear();
        QJsonArray jarray = jdoc.object()["api_data"].toArray();
        for (int i=0; i<jarray.count(); i++)
        {
            kcsapi_material api_material;
            api_material.ReadFromJObj(jarray[i].toObject());
            pksd->materialdata.append(api_material);
        }

        updateOverviewTable();
    }
    else if (pathAndQuery == "/kcsapi/api_get_member/questlist")
    {
        kcsapi_questlist api_questlist;
        api_questlist.ReadFromJObj(jobj);

        int questcount = api_questlist.api_count;
        if (questcount > api_questlist.api_list.count())
        {
            questcount = api_questlist.api_list.count();
        }

        for (int i=0; i<questcount; i++)
        {
            QList<kcsapi_quest>::iterator it;
            for (it=pksd->questdata.begin(); it!=pksd->questdata.end(); ++it)
            {
                if (it->api_no == api_questlist.api_list[i].api_no)
                {
                    pksd->questdata.erase(it);
                    break;
                }
            }
            if (api_questlist.api_list[i].api_state > 1)
            {
                pksd->questdata.append(api_questlist.api_list[i]);
            }
        }

        updateMissionTable();
    }
}

void KanDataConnector::updateOverviewTable()
{
    QList<QString> lst;
    QList<QColor> cols;
    KanSaveData * pksd = &KanSaveData::getInstance();

    int kancount = pksd->portdata.api_ship.count();
    int kanmaxcount = pksd->portdata.api_basic.api_max_chara;
    int slotitemcount = pksd->slotitemdata.count();
    int slotitemmaxcount = pksd->portdata.api_basic.api_max_slotitem;
    int instantrepaircount = pksd->materialdata[MATERIALDATAINDEX_INSTANDREPAIR].api_value;
    int instantbuildcount = pksd->materialdata[MATERIALDATAINDEX_INSTANTBUILD].api_value;
    int lv = pksd->portdata.api_basic.api_level;
    int nextexp = KanDataCalc::GetAdmiralNextLevelExp(pksd->portdata.api_basic.api_experience, lv);
    int fcoin = pksd->portdata.api_basic.api_fcoin;

    lst.append(QString::fromLocal8Bit("所有艦娘数: %1/%2").arg(kancount).arg(kanmaxcount));
    lst.append(QString::fromLocal8Bit("所有装備数: %1/%2").arg(slotitemcount).arg(slotitemmaxcount));
    lst.append(QString::fromLocal8Bit("高速修復材: %1").arg(instantrepaircount));
    lst.append(QString::fromLocal8Bit("高速建造材: %1").arg(instantbuildcount));
    lst.append(QString::fromLocal8Bit("提督Lv. %1(次:%2)").arg(lv).arg(nextexp));
    lst.append(QString::fromLocal8Bit("家具コイン: %1").arg(fcoin));

    //
    if (kanmaxcount == kancount)
    {
        cols.append(colRed);
    }
    else if (kanmaxcount - kancount < 5)
    {
        cols.append(colOrange);
    }
    else
    {
        cols.append(colWhite);
    }
    //
    if (slotitemmaxcount == slotitemcount)
    {
        cols.append(colRed);
    }
    else if (slotitemmaxcount - slotitemcount < 20)
    {
        cols.append(colOrange);
    }
    else
    {
        cols.append(colWhite);
    }
    //
    cols.append(colWhite);
    cols.append(colWhite);
    cols.append(colWhite);
    cols.append(colWhite);

    MainWindow::infoWindow()->updateOverviewTable(lst, cols);
}

void KanDataConnector::updateMissionTable()
{
    KanSaveData * pksd = &KanSaveData::getInstance();

    QList<KQRowData> rows;

    int questcount = pksd->questdata.count();
    for (int i=0; i<questcount; i++)
    {
        KQRowData rd;
        rd.appendCell(KQRowCellData(" - "));
        rd.appendCell(KQRowCellData(pksd->questdata[i].api_title, pksd->questdata[i].api_detail, colWhite));

        QString progress;
        bool bDone = false;
        switch (pksd->questdata[i].api_state)
        {
        case 1:
            qDebug("Quest not taken.");
            break;
        case 2:
            break;
        case 3:
            bDone = true;
            progress = QString::fromLocal8Bit("(完遂)");
            break;
        }
        if (!bDone)
        {
            switch (pksd->questdata[i].api_progress_flag)
            {
            case 0:
                progress = "(0%)";
                break;
            case 1:
                progress = "(50%)";
                break;
            case 2:
                progress = "(80%)";
                break;
            }
        }

        rd.appendCell(KQRowCellData(progress));
        rows.append(rd);
    }

    MainWindow::infoWindow()->updateMissionTable(QString::fromLocal8Bit("遂行中任務(%1)").arg(questcount), rows);
}

bool repairTableSort(const kcsapi_ship2 &left, const kcsapi_ship2 &right)
{
    return left.api_ndock_time <= right.api_ndock_time;
}

void KanDataConnector::updateRepairTable()
{
    KanSaveData * pksd = &KanSaveData::getInstance();
    QList<kcsapi_ship2> lstShips;
    foreach(const kcsapi_ship2 &ship2, pksd->portdata.api_ship)
    {
        if (ship2.api_ndock_time)
        {
            lstShips.append(ship2);
        }
    }
    qSort(lstShips.begin(), lstShips.end(), repairTableSort);

    int repaircount = lstShips.count();
    QList<KQRowData> rows;
    foreach(const kcsapi_ship2 &v, lstShips)
    {
        // Name
        // Lv
        // Cond
        // Time
        // HP
        // State
        int shipid = v.api_ship_id;
        kcsapi_mst_ship * pship = &(pksd->start2data.api_mst_ship[shipid]);

        KQRowData rd;
        rd.appendCell(KQRowCellData(pship->api_name));
        rd.appendCell(KQRowCellData(QString::fromLocal8Bit("Lv.%1").arg(v.api_lv)));
        rd.appendCell(KQRowCellData(QString::fromLocal8Bit("(%1)").arg(v.api_cond)));
        KQTime t;
        t.setTotalMS(v.api_ndock_time);
        rd.appendCell(KQRowCellData(t.toString()));
        rd.appendCell(KQRowCellData(QString::fromLocal8Bit("%1/%2").arg(v.api_nowhp).arg(v.api_maxhp)));

        rd.appendCell(KQRowCellData(""));
//        rd.appendCell(KQRowCellData(KanDataCalc::GetWoundState(v.api_nowhp, v.api_maxhp));

        rows.append(rd);
    }

    MainWindow::infoWindow()->updateRepairTable(QString::fromLocal8Bit("入渠まちーズ(%1)").arg(repaircount), rows);
}

void KanDataConnector::updateFleetTable()
{

}

void KanDataConnector::updateExpeditionTable()
{

}

void KanDataConnector::updateRepairDockTable()
{

}

void KanDataConnector::updateBuildDockTable()
{

}
