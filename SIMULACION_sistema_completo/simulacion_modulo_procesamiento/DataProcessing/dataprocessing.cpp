#include "dataprocessing.h"
#include <QDateTime>
#include <iostream>
#include <QString>
#include <QObject>

DataProcessing::DataProcessing()
{
}


QString DataProcessing::getBlockPick(const QDateTime& firstdatetime, const QDateTime& lastdatetime){
    duration = firstdatetime.msecsTo(lastdatetime);
    lastDate = lastdatetime;
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
        duration = firstdatetime.msecsTo(lastdatetime);
        lastDate = lastdatetime;
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
                    found = true;
                }
            }
        }
        return pos;

    }

    QList<ANIMATIONBLOCK> DataProcessing::getSecuence(const std::set<DATEBLOCK>& blocks){
        QString blockString;
        QList<ANIMATIONBLOCK> animationBlock;
        std::set<DATEBLOCK>::iterator it2;
        ANIMATIONBLOCK mySecuence;
        for(std::set<DATEBLOCK>::iterator it = blocks.begin(); it != blocks.end(); ++it){
            blockString.clear();
            for(int i=0; i<it->first.size(); i++){
                blockString += "\n" + it->first.at(i);
            }

            it2 = it;
            if(it != blocks.begin())
                --it2;

            mySecuence = ANIMATIONBLOCK(blockString,it2->second.msecsTo(it->second));
            animationBlock.push_back(mySecuence);
        }
        if(blocks.size() > 0){
            it2 = blocks.end();
            --it2;
            if(it2->second.msecsTo(lastDate) < duration){
                duration = it2->second.msecsTo(lastDate);
            }
        }
        secuence = animationBlock;
        return animationBlock;
    }


    std::set<DATEBLOCK> DataProcessing::getDateTimeBlocks(const QString &block){
        QRegExp rxDateBlock("\\d+-\\d+-\\d+ \\d+:\\d+:\\d+.\\d");
        std::set<DATEBLOCK> dataBlocks;
        QStringList blocks = block.split("\n");
        for(int i = 0; i<blocks.size(); i++){
            if (blocks.size() > 0){
                if(rxDateBlock.indexIn(blocks[i]) != -1){
                    DATEBLOCK myPair(QStringList()<<blocks[i],QDateTime::fromString(rxDateBlock.cap(0),"yyyy-MM-dd hh:mm:ss.z"));
                    std::set<DATEBLOCK>::iterator it = dataBlocks.find(myPair);
                    if (it != dataBlocks.end()){
                        DATEBLOCK temp(*it);
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

    bool operator < (const DATEBLOCK& block1, const DATEBLOCK& block2){
        return block1.second < block2.second;
    }

    /*void DataProcessing::writeSecuence(const QString& block){
        QFile file("C:/Users/People/Desktop/test.txt");
        file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
        QTextStream out(&file);
        out << endl << block;
        file.close();
    }*/
