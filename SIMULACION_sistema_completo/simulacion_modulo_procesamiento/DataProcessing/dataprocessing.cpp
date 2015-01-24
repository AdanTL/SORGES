#include "dataprocessing.h"
#include <QDateTime>
#include <iostream>
#include <QString>

DataProcessing::DataProcessing()
{
}

QString DataProcessing::getBlockPick(const QDateTime& firstdatetime, const QDateTime& lastdatetime){
    int posBegin, posEnd;
    QString blockPick;
    posBegin = getPositionBegin(firstdatetime,":/testFiles/scalertes_picks.log");
    posEnd = getPositionEnd(lastdatetime, ":/testFiles/scalertes_picks.log");
    QFile file(":/testFiles/scalertes_picks.log");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        std::cerr << "Problem to find the file: " << std::endl;

    }
    if(posBegin < posEnd){
        file.seek(posBegin);
        blockPick = file.read(posEnd - posBegin);
    }
    return blockPick;

}

QString DataProcessing::getBlockOrigin(const QDateTime& firstdatetime, const QDateTime& lastdatetime){
        int posBegin, posEnd;
        QString blockOrigin;
        posBegin = getPositionBegin(firstdatetime,":/testFiles/scalertes_origenes.log");
        posEnd = getPositionEnd(lastdatetime, ":/testFiles/scalertes_origenes.log");
        QFile file(":/testFiles/scalertes_origenes.log");
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            std::cerr << "Problem to find the file: " << std::endl;

        }
        if(posBegin < posEnd){
            file.seek(posBegin);
            blockOrigin = file.read(posEnd - posBegin);
        }
        return blockOrigin;

    }



    int DataProcessing::getPositionBegin(const QDateTime& firstdatetime, const QString& namefile){
        QRegExp rxDateBlock("\\d+-\\d+-\\d+ \\d+:\\d+:\\d+.\\d");
        QString fileContent;
        QFile file(namefile);
        bool found = false;
        int pos = 0;
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            return -1;
        }

        while(!file.atEnd() && !found){
            pos = file.pos();
            fileContent = file.readLine();
            if(rxDateBlock.indexIn(fileContent) != -1 ){
                if(QDateTime::fromString(rxDateBlock.cap(0).remove("\n"),"yyyy-MM-dd hh:mm:ss.z")
                >= firstdatetime)
                    found = true;
            }
        }
        return pos;

}



    int DataProcessing::getPositionEnd(const QDateTime &lastdatetime, const QString& namefile){
        QRegExp rxDateBlock("\\d+-\\d+-\\d+ \\d+:\\d+:\\d+.\\d");
        QString fileContent;
        QFile file(namefile);
        bool found = false;
        int pos = 0;

        if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            return -1;
        }
        while(!file.atEnd() && !found){
            pos = file.pos();
            fileContent = file.readLine();
            if(rxDateBlock.indexIn(fileContent) != -1 ){
                if(QDateTime::fromString(rxDateBlock.cap(0),"yyyy-MM-dd hh:mm:ss.z")
                > lastdatetime){
                    file.seek(pos);
                    std::cout << QString(file.readLine()).toStdString() << std::endl;
                    found = true;
                }
            }
        }
        return pos;

    }

    std::set<DATABLOCK> DataProcessing::getDateTimeBlocks(QString& block){
        QRegExp rxDateBlock("\\d+-\\d+-\\d+ \\d+:\\d+:\\d+.\\d");
        std::set<DATABLOCK> dataBlocks;
        QStringList blocks = block.split("\n");
        for(int i = 0; i<blocks.size(); i++){
            if (blocks.size() > 0){
                if(rxDateBlock.indexIn(blocks[i]) != -1){
                    DATABLOCK myPair(QStringList()<<blocks[i],QDateTime::fromString(rxDateBlock.cap(0),"yyyy-MM-dd hh:mm:ss.z"));
                    std::set<DATABLOCK>::iterator it = dataBlocks.find(myPair);
                    if (it != dataBlocks.end()){
                        DATABLOCK temp(*it);
                        temp.first = temp.first << myPair.first;
                        dataBlocks.erase(it);
                        dataBlocks.insert(temp);

                    }else
                        dataBlocks.insert(myPair);
                }
            }
        }
        return dataBlocks;

    }

    bool operator < (const DATABLOCK& block1, const DATABLOCK& block2){
        return block1.second < block2.second;
    }
