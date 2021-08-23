#include "uNoteDbusServer.h"

UNoteDbusServer::UNoteDbusServer()
{

}

QVariantMap UNoteDbusServer::keywordMatch(QStringList keyList)
{
    QVariantMap keyMap;

    qDebug() << keyList.count();
    QString sql;

    if(keyList.count() >= 1) {
        sql = QStringLiteral("SELECT id, md_content, full_title FROM active_notes WHERE md_content LIKE '%%1%'").arg(keyList.at(0));
        for(int i = 1; i < keyList.count(); i++) {
            QString tmpSql = QStringLiteral(" AND md_content LIKE '%%1%'").arg(keyList.at(i));
            sql.append(tmpSql);
        }
    }

    qDebug() << sql <<QSqlDatabase::drivers();

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QDir::homePath() + "/.config/kylin-note/kyNotes.db");
    if(!db.open()){
        //如果数据库打开失败
        qWarning() << "error!" << db.lastError().text();
    }

    QSqlQuery query;
    if(query.exec(sql)) {
        while(query.next()){
            qDebug() << "Dat:::" << query.value(0).toString() << query.value(1).toString() << query.value(2).toString();
            QVariantList content;
            content << query.value(1).toString().mid(query.value(1).toString().indexOf(keyList.at(0)), 20) << query.value(2).toString();
            qDebug() << "Dat###" << query.value(1).toString().mid(query.value(1).toString().indexOf(keyList.at(0)), 20);
            QVariant var = QVariant::fromValue<QVariantList>(content);
            keyMap.insert(query.value(0).toString(), var);
        }
    }
    qDebug() << keyMap;
    return keyMap;
}

QMap<QString, QVariant> UNoteDbusServer::loadSqlDB()
{
    qDebug() << QSqlDatabase::drivers();
    QVariantMap keyMap;
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");

    db.setDatabaseName(QDir::homePath() + "/.config/kylin-note/kyNotes.db");
    if(!db.open()){
        //如果数据库打开失败
        qWarning() << "error!" << db.lastError();
    }

    QSqlQuery query;
    if(query.exec("SELECT id, md_content FROM active_notes")) {
        while(query.next()){
            keyMap.insert(query.value(0).toString(), query.value(1));
            qDebug() << query.value(0).toString() << query.value(1).toString();
        }
    }

    return keyMap;
}
