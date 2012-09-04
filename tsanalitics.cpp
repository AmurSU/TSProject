#include "tsanalitics.h"
#include <QDebug>
tsanalitics::tsanalitics(QObject *parent) :QObject(parent){
    QVector<extremum> *extrem = new QVector<extremum>;
    ts_extremums=extrem;
}

QVector<int> tsanalitics::getMovingAverages(int Period){
    int i=0,j=0,sum=0;
    QVector<int> *movavgs = new QVector<int>;
    for(i=0;i<Period;i++){
        movavgs->push_back(ts_row_data->at(i));
    }
    for( i=Period; i<ts_row_data->size(); i++){
        for(j=i-Period;j<i;j++){
            sum += ts_row_data->at(j);
        }
        movavgs->push_back(sum/Period);
        sum=0;
    }

    qDebug()<<*ts_row_data;
    qDebug()<<"--------------";
    return *movavgs;
}

float tsanalitics::getMaxAvgs(){
    int i=0, sum = 0, cntr=0;
    for(i=0; i<ts_extremums->size();i++){
        if( ts_extremums->at(i).type == 1 ){
            sum += ts_extremums->at(i).y;
            cntr++;
        }
    }
    if ( cntr == 0 )
        return 0;
    else
        return float(sum/cntr);
}

float tsanalitics::getdMinAvgs(){
    int i=0, sum = 0, cntr=0;
    for(i=0; i<ts_extremums->size();i++){
        if( ts_extremums->at(i).type == -1 ){
            sum += ts_extremums->at(i).y;
            cntr++;
        }
    }
    if ( cntr == 0 )
        return 0;
    else
        return float(sum/cntr);
}

float tsanalitics::getFrequency(){
    int maxc = getMaxsCount();
    int minc = getMinsCount();
    int time = getTime();
    return ((float)maxc+(float)minc)/2/time*60;
}

int tsanalitics::getMaxsCount(){
    int i=0, cntr=0;
    for(i=0; i<ts_extremums->size();i++)
        if( ts_extremums->at(i).type == 1 )
            cntr++;
    return cntr;
}

int tsanalitics::getMinsCount(){
    int i=0, cntr=0;
    for(i=0; i<ts_extremums->size();i++)
        if( ts_extremums->at(i).type == -1 )
            cntr++;
    return cntr;
}

int tsanalitics::getMinMaxAvgTime(){
    int i=0, sum = 0, cntr=0;
    for(i=1; i<ts_extremums->size();i++){
        if( ts_extremums->at(i).type == -1 && ts_extremums->at(i-1).type == 1 ){
            sum += ts_extremums->at(i).x-ts_extremums->at(i-1).x;
            cntr++;
        }
    }
    if ( cntr == 0 )
        return 0;
    else
        return sum/cntr;
}

int tsanalitics::getMaxMinAvgTime(){
    int i=0, sum = 0, cntr=0;
    for(i=1; i<ts_extremums->size();i++){
        if( ts_extremums->at(i).type == 1 && ts_extremums->at(i-1).type == -1 ){
            sum += ts_extremums->at(i).x-ts_extremums->at(i-1).x;
            cntr++;
        }
    }
    if ( cntr == 0 )
        return 0;
    else
        return sum/cntr;
}

int tsanalitics::getTime(){
    return ts_row_data->size();
}


int tsanalitics::setupData(QVector<int> *row_d){
    ts_row_data=row_d;
}

int tsanalitics::findExtremums(){
    int i=0;
    qDebug()<<ts_row_data->size();
    for (int i = 1; i < ts_row_data->size()-1; ++i) {
        ts_row_data->at(i);
        if( (ts_row_data->at(i-1)<=ts_row_data->at(i) && ts_row_data->at(i)>=ts_row_data->at(i+1) ) ||
                (ts_row_data->at(i-1)>=ts_row_data->at(i) && ts_row_data->at(i)<=ts_row_data->at(i+1) ) ){
            qDebug()<<"extremum exist"<<ts_row_data->at(i);
            extremum *extr = new extremum;
            extr->y = ts_row_data->at(i);
            extr->x = i;
            if (ts_row_data->at(i-1)<ts_row_data->at(i) && ts_row_data->at(i)>ts_row_data->at(i+1)){
                extr->type=1;
                qDebug()<<"max="<<ts_row_data->at(i);
            }
            else{
                extr->type=-1;
                qDebug()<<"min="<<ts_row_data->at(i);
            }
            ts_extremums->push_back(*extr);
        }
    }
    qDebug()<<"all max="<<getMax();
    qDebug()<<"all min="<<getMin();
}

int tsanalitics::deleteBadExtremums(){
    int i=0, max_index;
    int middle = getMax()/2+getMin()/2;
    for (i=0;i<ts_extremums->size();i++){
        if( ts_extremums->at(i).x-ts_extremums->at(i-3).x < 50 )
            if ( ( ts_extremums->at(i).type==-1 && ts_extremums->at(i-1).type==1 && ts_extremums->at(i-2).type==-1 && ts_extremums->at(i-3).type==1 )
                 && (ts_extremums->at(i).y < ts_extremums->at(i-1).y && ts_extremums->at(i).y < ts_extremums->at(i-2).y  && ts_extremums->at(i).y < ts_extremums->at(i-3).y )
                 && (ts_extremums->at(i-3).y > ts_extremums->at(i-2).y && ts_extremums->at(i-3).y > ts_extremums->at(i-1).y && ts_extremums->at(i-3).y > ts_extremums->at(i).y)
                 && (ts_extremums->at(i-1).y > ts_extremums->at(i).y && ts_extremums->at(i-1).y > ts_extremums->at(i-2).y && ts_extremums->at(i-1).y < ts_extremums->at(i-3).y)
                 && (ts_extremums->at(i-2).y > ts_extremums->at(i).y && ts_extremums->at(i-2).y < ts_extremums->at(i-1).y && ts_extremums->at(i-2).y < ts_extremums->at(i-3).y)
                 ){
                ts_extremums->remove(i-2,2);
                qDebug()<<"Need remove middle ";
            }else if ( (ts_extremums->at(i).type==1 && ts_extremums->at(i-1).type==-1 && ts_extremums->at(i-2).type==1 && ts_extremums->at(i-3).type==-1 )
                       && (ts_extremums->at(i).y > ts_extremums->at(i-1).y && ts_extremums->at(i).y > ts_extremums->at(i-2).y  && ts_extremums->at(i).y > ts_extremums->at(i-3).y )
                       && (ts_extremums->at(i-3).y < ts_extremums->at(i-2).y && ts_extremums->at(i-3).y < ts_extremums->at(i-1).y && ts_extremums->at(i-3).y < ts_extremums->at(i).y)
                       && (ts_extremums->at(i-1).y < ts_extremums->at(i).y && ts_extremums->at(i-1).y < ts_extremums->at(i-2).y && ts_extremums->at(i-1).y > ts_extremums->at(i-3).y)
                       && (ts_extremums->at(i-2).y < ts_extremums->at(i).y && ts_extremums->at(i-2).y > ts_extremums->at(i-1).y && ts_extremums->at(i-2).y > ts_extremums->at(i-3).y)
                       ){
                ts_extremums->remove(i-2,2);
                qDebug()<<"Need remove middle ";
            }
    }
}

int tsanalitics::getMax(){
    int i=0, max_index=0;
    for (i=0;i<ts_row_data->size();i++){
        if( ts_row_data->at(i)> ts_row_data->at(max_index) )
            max_index = i;
    }
    return ts_row_data->at(max_index);
}

int tsanalitics::getMin(){
    int i=0, min_index=0;
    for (i=0;i<ts_row_data->size();i++){
        if( ts_row_data->at(i)< ts_row_data->at(min_index) )
            min_index = i;
    }
    return ts_row_data->at(min_index);
}
