#include "uNoteDbusServer.h"

UNoteDbusServer::UNoteDbusServer()
{

}

QVariantMap UNoteDbusServer::keywordMatch(QString key)
{
    QVariantMap keyMap;
    keyMap = loadSqlDB();
    if(keyMap.empty()) {
       return  keyMap;
    } else {
        QMap<QString, QVariant>::iterator iter = keyMap.begin();
        while (iter != keyMap.end())
        {
            qDebug() << "Iterator### " << iter.key() << ":" << iter.value(); // 迭代器
            if(iter.value().toString().contains(key)) {
                keywordsMatched.insert(iter.key(), iter.value());
            }
            iter++;
        }
    }
    qDebug() << "keywordMatch" << keywordsMatched;
    return keywordsMatched;
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
