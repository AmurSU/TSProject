#include "tsanalitics.h"
#include <QDebug>
tsanalitics::tsanalitics(QObject *parent) :QObject(parent){
    QVector<extremum> *extrem = new QVector<extremum>;
    ts_extremums=extrem;
    QVector<int> *row_d = new QVector<int>;
    ts_row_data = row_d;
    QVector<extremum> *v_extrem = new QVector<extremum>;
    ts_vol_exts=v_extrem;
    QVector<exhalation> *exhls = new QVector<exhalation>;
    ts_exhls=exhls;
}
/*
int tsanalitics::getMaxAvgs(){
    int i=0, sum = 0, cntr=0;
    for(i=0; i<ts_extremums->size();i++){
        if( ts_extremums->at(i).type == 1 ){
            sum += fabs(ts_extremums->at(i).y);
            cntr++;
        }
    }
    if ( cntr == 0 )
        return 0;
    else
        return sum/cntr;
}

int tsanalitics::getMinAvgs(){
    int i=0, sum = 0, cntr=0;
    for(i=0; i<ts_extremums->size();i++){
        if( ts_extremums->at(i).type == -1 ){
            sum += fabs(ts_extremums->at(i).y);
            cntr++;
        }
    }
    if ( cntr == 0 )
        return 0;
    else
        return sum/cntr;
}
*/
float tsanalitics::getFrequency(){
    if(ts_exhls->size()>0)
        return 6000/((float)ts_exhls->last().end/(float)ts_exhls->size());
    else
        return 0;
}




int tsanalitics::getAvgExpiratoryTime(){
    int sum=0,i=0;
    for(i=0;i<ts_exhls->size();i++){
        sum+=ts_exhls->at(i).end-ts_exhls->at(i).start;
    }
    if (ts_exhls->size()>0)
        return sum/ts_exhls->size();
    else
        return 0;
}

int tsanalitics::setupData(QVector<int> *row_d){
    ts_row_data=row_d;
}

void tsanalitics::approximate()
{
    int i=0;
    if(ts_row_data->size()!=0){
        QVector<int> *df = new QVector<int>;
        for(i=0;i<ts_row_data->size()-1;i++){
            df->push_back(ts_row_data->at(i+1)-ts_row_data->at(i));
        }
        for(i=0;i<ts_row_data->size();i++){
            qDebug()<<i<<"="<<ts_row_data->at(i);
            extremum *extr = new extremum;
            extr->y = ts_row_data->at(i);
            extr->x = i;
            ts_vol_exts->push_back(*extr);
        }
        for(i=0;i<ts_vol_exts->size()-1;i++){
            if(ts_vol_exts->at(i).y!=0 && ts_vol_exts->at(i+1).y!=0){
                if(ts_vol_exts->at(i+1).y<ts_vol_exts->at(i).y){
                    ts_vol_exts->remove(i,1);
                    if (i>0)
                        i--;
                }else{
                    ts_vol_exts->remove(i+1,1);
                }
            }
        }
        for(i=0;i<ts_vol_exts->size();i++){//отсеиваем мелкие колебания
            if(ts_vol_exts->at(i).y!=0 && ts_vol_exts->at(i).y>-100){
                ts_vol_exts->remove(i,1);
            }
        }
        for(i=0;i<ts_vol_exts->size()-1;i++){//убираем двойные нули
            if(ts_vol_exts->at(i).y==0 && ts_vol_exts->at(i+1).y==0){
                ts_vol_exts->remove(i,1);
                if (i>0)
                    i--;
            }
        }
        qDebug()<<ts_vol_exts->size()<<"size -+------";
        if(ts_vol_exts->size()>0){
            for(i=1;i<ts_vol_exts->size()-1;i++){
                if(ts_vol_exts->at(i).y!=0 && ts_vol_exts->at(i+1).y==0 && ts_vol_exts->at(i-1).y==0){
                    qDebug()<<"fucking shirt";
                    exhalation *ex = new exhalation;
                    ex->start=ts_vol_exts->at(i-1).x;
                    ex->end=ts_vol_exts->at(i).x;
                    ex->vol=ts_vol_exts->at(i).y;
                    ts_exhls->push_back(*ex);
                }
            }
        }
    }
}

int tsanalitics::getBreathingVolume(){
    ts_vol_exts;
    int sum=0,i=0;
    for (i=0;i<ts_exhls->size();i++){
        sum+=fabs(ts_exhls->at(i).vol);
    }
    if(ts_exhls->size()>0)
        return sum/ts_exhls->size();
    else
        return 0;
}

int tsanalitics::getAvgExpiratorySpeed(){
    int time=0,i=0,len=0;
    for(i=0;i<ts_exhls->size();i++){
        time+=ts_exhls->at(i).end-ts_exhls->at(i).start;
        //qDebug()<<ts_exhls->at(i).end-ts_exhls->at(i).start;
        len+=ts_exhls->at(i).vol;
    }
    if (time==0)
        return 0;
    else
        return len*100/time;
}

int tsanalitics::getMaxExpiratorySpeed(){
    int i=0;
    float speed=0,max=-1000000;
    for(i=0;i<ts_exhls->size();i++){
        speed=(float)ts_exhls->at(i).vol/(float)(ts_exhls->at(i).end-ts_exhls->at(i).start);
        if(speed>max)
            max=speed;
    }
    return max;
}

void tsanalitics::append(int n){
    ts_row_data->push_back(n);
}

void tsanalitics::clear(){
    ts_extremums->remove(0,ts_extremums->size());
    ts_extremums->clear();
    ts_vol_exts->remove(0,ts_extremums->size());
    ts_vol_exts->clear();
    ts_exhls->remove(0,ts_exhls->size());
    ts_exhls->clear();
    ts_row_data->remove(0,ts_row_data->size());
    ts_row_data->clear();
}

int tsanalitics::getMaxsSum(){
    int sum=0, i=0;
    for(i=0;i<ts_extremums->size();i++){
        if(ts_extremums->at(i).type==1)
            sum+=ts_extremums->at(i).y;
    }
    return sum;
}

int tsanalitics::getMinsSum(){
    int sum=0, i=0;
    for(i=0;i<ts_extremums->size();i++){
        if(ts_extremums->at(i).type==-1)
            sum+=ts_extremums->at(i).y;
    }
    return sum;
}

int tsanalitics::getMVL(){
    int sum=0,i=0;
    for (i=0;i<ts_exhls->size();i++){
        sum+=ts_exhls->at(i).vol;
    }
    if(ts_exhls->size()>0)
        return sum;
    else
        return 0;
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

int tsanalitics::fabs(int a){
    if( a<0 )
        return -a;
    else
        return a;
}

int tsanalitics::getAvgInspiratoryTime(){
    int sum=0,i=0;
    for(i=0;i<ts_exhls->size()-1;i++){
        sum+=fabs(ts_exhls->at(i+1).end-ts_exhls->at(i).start);
    }
    if (ts_exhls->size()>0)
        return sum/ts_exhls->size();
    else
        return 0;
}
