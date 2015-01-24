#include "dataprocessing.h"
#include <QDateTime>
#include <iostream>
#include <QString>

DataProcessing::DataProcessing()
{
}   
/*
    QString DataProcessing::getBlockPick(const QDateTime firstdatetime, const QDateTime lastdatetime){
        int posBegin, posEnd;
        QString blockPick;
        posBegin = getPositionPickBegin(firstdatetime,":/testFiles/scalertes_picks.log");
        posEnd = getPositionPickEnd(lastdatetime, ":/testFiles/scalertes_picks.log");
        QFile file(":/testFiles/scalertes_picks.log");
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            std::cerr << "Problem to find the file: " << std::endl;

        }
        file.seek(posBegin);
        blockPick = file.read(posEnd - posBegin);

        return blockPick;

    }

    QString DataProcessing::getBlockOrigin(const QDateTime firstdatetime, const QDateTime lastdatetime){
            int posBegin, posEnd;
            QString blockOrigin;
            posBegin = getPositionOriginBegin(firstdatetime,":/testFiles/scalertes_origenes.log");
            posEnd = getPositionOriginEnd(lastdatetime, ":/testFiles/scalertes_origenes.log");
            QFile file(":/testFiles/scalertes_origenes.log");
            if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                std::cerr << "Problem to find the file: " << std::endl;

            }
            file.seek(posBegin);
            blockOrigin = file.read(posEnd - posBegin);

            return blockOrigin;

        }


*/


    int DataProcessing::getPositionPickBegin(const QDateTime firstdatetime, const QString& namefile){

        QRegExp rxBeginDateTime("\n\\d+-\\d+-\\d+ \\d+:\\d+:\\d+.\\d");
        //QRegExp rxExitDateTime("\\d+-\\d+-\\d+ \\d+:\\d+:\\d+.\\d");
        QString fileContent;
        QFile file(namefile);
        bool found = false;
        bool foundRemove = false;
        bool overflow = false;
        int pos=file.size();

        if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            std::cerr << "Problem to find the file: " << namefile.toStdString() << std::endl;
            return -1;
        }

        // Get Fragment with the same Date and Time (it contents some rubbish data, but it is not important).

        do{
            pos -= 1171;
            //it is not a empty file.
            if(pos < 0){
                overflow = true;
                pos = 0;
            }
            file.seek(pos);
            fileContent = file.readAll();

            if(rxBeginDateTime.indexIn(fileContent) != -1){
                if(QDateTime::fromString(rxBeginDateTime.cap(0).remove("\n"), "yyyy-MM-dd hh:mm:ss.z") < firstdatetime){
                found = true;

                std::cout << rxBeginDateTime.cap(0).toStdString() <<std::endl;
                std::cout << "-------------------------------------------"<<std::endl;
                std::cout << fileContent.toStdString() <<std::endl;
                }
            }
        }while(found == false && overflow == false);

        // if is overflowed return.
        if(found == false){
            //cerr
            return -1;
        }

        do{
             if(rxBeginDateTime.indexIn(fileContent) != -1){
                 if(QDateTime::fromString(rxBeginDateTime.cap(0).remove("\n"), "yyyy-MM-dd hh:mm:ss.z") > firstdatetime){
                     std::cout << "-------------------------------------------"<<std::endl;
                     foundRemove = true;
                     fileContent.remove(0, fileContent.indexOf(rxBeginDateTime.cap(0),0));

                 }
               }
           }while(foundRemove == false);



        std::cout << "-------------------------------------------"<<std::endl;
        std::cout << "-------------------------------------------"<<std::endl;
        std::cout << fileContent.toStdString() <<std::endl;

        return pos;
}


/*
    int DataProcessing::getPositionPickEnd(const QDateTime lastdatetime, const QString& namefile){

        return int();

    }


*/



