#include "tsanalitics.h"
#include <QDebug>
tsanalitics::tsanalitics(QObject *parent) :QObject(parent){
    ts_extremums = new QVector<extremum>;
    ts_row_data = new QVector<int>;
    ts_vol_exts = new QVector<extremum>;
    ts_exhls = new QVector<exhalation>;
    qDebug()<<"Analitic is init";
}

tsanalitics::~tsanalitics()
{
    delete ts_extremums;
    delete ts_vol_exts;
    delete ts_exhls;
    delete ts_row_data;
}
float tsanalitics::getFrequency(){
    //qDebug()<<"getFrequency";
    if(ts_exhls->size()>0)
        return 6000/((float)ts_row_data->size()/(float)ts_exhls->size());
    else
        return 0;
}




int tsanalitics::getAvgExpiratoryTime(){
    //qDebug()<<"getAvgExpiratoryTime";
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
    //qDebug()<<"setupData";
    ts_row_data = row_d;
}

void tsanalitics::approximate(){
    //qDebug()<<"approximate";
    int i=0;
    QVector<int> vec;
    ts_extremums->remove(0,ts_extremums->size());
    ts_extremums->clear();
    ts_vol_exts->remove(0,ts_extremums->size());
    ts_vol_exts->clear();
    ts_exhls->remove(0,ts_exhls->size());
    ts_exhls->clear();
    if(ts_row_data->size()!=0){
        QVector<int> *df = new QVector<int>;
        for(i=0;i<ts_row_data->size()-1;i++){
            if ( abs(ts_row_data->at(i))>5){
                vec.push_back(i);
            }else{
                if(vec.size()>15){
                    /*qDebug()<<"["<<vec.first()<<" "<<vec.last()<<"]="<<vec.size()<<" vol="<<ts_row_data->at(vec.last())<<" speed="
                           <<ts_row_data->at(vec.last())/vec.size();*/
                    vec.remove(0,vec.size());
                    vec.clear();
                }
            }
            df->push_back(ts_row_data->at(i+1)-ts_row_data->at(i));
        }
        for(i=0;i<ts_row_data->size();i++){
            //qDebug()<<i<<"="<<ts_row_data->at(i);
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
                    if(ts_vol_exts->size()<=i+1){
                        ts_vol_exts->remove(i+1,1);
                    }
                }
            }
        }
        for(i=0;i<ts_vol_exts->size();i++){//��������� ������ ���������
            if(ts_vol_exts->at(i).y!=0 && ts_vol_exts->at(i).y>-20){
                ts_vol_exts->remove(i,1);
            }
        }
        for(i=0;i<ts_vol_exts->size()-1;i++){//������� ������� ����
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
    //qDebug()<<"getBreathingVolume";
    QVector<int> vec,spd;
    int i=0,max=0,k=0;
    for(i=0;i<ts_row_data->size();i++){
        if ( abs(ts_row_data->at(i))>5){
            vec.push_back(i);
        }else{
            if(vec.size()>15){
                /*qDebug()<<"["<<vec.first()<<" "<<vec.last()<<"]="<<vec.size()<<" vol="<<ts_row_data->at(vec.last())<<" speed="
                      <<ts_row_data->at(vec.last())/vec.size();*/
                //spd.append(ts_row_data->at(vec.last())/vec.size());
                max+=ts_row_data->at(vec.last());
                //qDebug()<<"max="<<max<<" k="<<k;
                k++;
            }
            vec.remove(0,vec.size());
            vec.clear();
        }
    }
    if (k>0){
        return max/k;
    }
    else
        return 1;
    /*

    int sum=0,i=0;
    for (i=0;i<ts_exhls->size();i++){
        sum+=fabs(ts_exhls->at(i).vol);
    }
    if(ts_exhls->size()>0)
        return sum/ts_exhls->size();
    else
        return 0;*/
}

int tsanalitics::getAvgExpiratorySpeed(){
    //qDebug()<<"getAvgExpiratorySpeed";
    int time=0,i=0,len=0;
    for(i=0;i<ts_exhls->size();i++){
        if(ts_exhls->at(i).end-ts_exhls->at(i).start>15){
            time+=ts_exhls->at(i).end-ts_exhls->at(i).start;
            qDebug()<<ts_exhls->at(i).end-ts_exhls->at(i).start<<" "<<ts_exhls->at(i).vol;
            len+=ts_exhls->at(i).vol;
        }
    }
    if (time==0)
        return 0;
    else
        return len/time;
}

int tsanalitics::getMaxExpiratorySpeed(){
    //qDebug()<<"getMaxExpiratorySpeed";
    QVector<int> vec,spd;
    int i=0,max=0;
    for(i=0;i<ts_row_data->size();i++){
        if ( abs(ts_row_data->at(i))>10){
            vec.push_back(i);
        }else{
            if(vec.size()>15){
                //qDebug()<<"["<<vec.first()<<" "<<vec.last()<<"]="<<vec.size()<<" vol="<<ts_row_data->at(vec.last())<<" speed="
                //       <<ts_row_data->at(vec.last())/vec.size();
                spd.append(ts_row_data->at(vec.last())/vec.size());
            }
            vec.remove(0,vec.size());
            vec.clear();
        }
    }
    for(i=0;i<spd.size();i++){
        if (abs(spd.at(i))>abs(max))
            max=spd.at(i);
    }
    return max;
    /*  int i=0;
    float speed=0,max=-1000000;
    for(i=0;i<ts_exhls->size();i++){
        speed=(float)ts_exhls->at(i).vol/(float)(ts_exhls->at(i).end-ts_exhls->at(i).start);
        if(speed>max)
            max=speed;
    }
    return max;*/
}

void tsanalitics::append(int n){
    qDebug()<<"append "<<n;
    int k = n;
    try{
        //n= ts_row_data->size();
        //ts_row_data[n] = k;
        ts_row_data->append(10);
        //ts_row_data->push_back(k);
    }
    catch(...){
        qDebug()<<"Push bach error";
    }

    qDebug()<<"End appending";
}

void tsanalitics::clear(){
    //qDebug()<<"clear";
    ts_extremums->remove(0,ts_extremums->size());
    ts_extremums->clear();
    ts_vol_exts->remove(0,ts_extremums->size());
    ts_vol_exts->clear();
    ts_exhls->remove(0,ts_exhls->size());
    ts_exhls->clear();
    ts_row_data->remove(0,ts_row_data->size());
    ts_row_data->clear();
}

int tsanalitics::getMVL(){
    QVector<int> vec;
    int i=0,max=0;
    for(i=0;i<ts_row_data->size();i++){
        if ( abs(ts_row_data->at(i))>5){
            vec.push_back(i);
        }else{
            if(vec.size()>15){
                //qDebug()<<"["<<vec.first()<<" "<<vec.last()<<"]="<<vec.size()<<" vol="<<ts_row_data->at(vec.last())<<" speed="
                //       <<ts_row_data->at(vec.last())/vec.size();
                //spd.append(ts_row_data->at(vec.last())/vec.size());
                max+=ts_row_data->at(vec.last());
            }
            vec.remove(0,vec.size());
            vec.clear();
        }
    }
    return max;
    /*  for(i=0;i<spd.size();i++){
        if (abs(spd.at(i))>abs(max))
            max=spd.at(i);
    }*/
    //qDebug()<<"getMVL";
    /*int sum=0,i=0;
    for (i=0;i<ts_exhls->size();i++){
        sum+=ts_exhls->at(i).vol;
    }
    if(ts_exhls->size()>0)
        return sum;
    else
        return 0;*/
}

int tsanalitics::getMax(){
    //qDebug()<<"getMax";
    int i=0, max_index=0;
    for (i=0;i<ts_row_data->size();i++){
        if( ts_row_data->at(i)> ts_row_data->at(max_index) )
            max_index = i;
    }
    if ( ts_row_data->size()>0){//ts_row_data->size()<=max_index &&
        ////qDebug()<<"max_index"<<max_index<<" ts_row_data->at(max_index)"<<ts_row_data->at(max_index);
        return ts_row_data->at(max_index);
    }
    else
        return 0;
}

int tsanalitics::getMin(){
    //qDebug()<<"getMin";
    int i=0, min_index=0;
    for (i=0;i<ts_row_data->size();i++){
        if( ts_row_data->at(i)< ts_row_data->at(min_index) )
            min_index = i;
    }

    if(ts_row_data->size()>0 ){//&& ts_row_data->size()<= min_index
        ////qDebug()<<"min_index"<<min_index<<"ts_row_data->at(min_index)"<<ts_row_data->at(min_index);
        return ts_row_data->at(min_index);
    }
    else
        return 0;
}

int tsanalitics::fabs(int a){
    //qDebug()<<"fabs";
    if( a<0 )
        return -a;
    else
        return a;
}

int tsanalitics::getAvgInspiratoryTime(){
    //qDebug()<<"getAvgInspiratoryTime";
    int sum=0,i=0;
    for(i=0;i<ts_exhls->size()-1;i++){
        sum+=fabs(ts_exhls->at(i+1).end-ts_exhls->at(i).start);
    }
    if (ts_exhls->size()>0)
        return sum/ts_exhls->size();
    else
        return 0;
}
