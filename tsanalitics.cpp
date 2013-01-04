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
    return *movavgs;
}

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

float tsanalitics::getFrequency(){
    if(ts_exhls->size()>0)
        return 6000/((float)ts_exhls->last().end/(float)ts_exhls->size());
    else
        return 0;
    //    ts_exhls;
    //    int i=0,fst=0,lst=0;
    //    int minc = getMinsCount();

    //    for(i=0;i<ts_extremums->size();i++){
    //        if(ts_extremums->at(i).type==-1){
    //            fst =ts_extremums->at(i).x;
    //            break;
    //        }
    //    }
    //    //qDebug()<<"fst="<<fst;
    //    for(i=ts_extremums->size()-1;i>0;i--){
    //        if(ts_extremums->at(i).type==-1){
    //            lst = ts_extremums->at(i).x;
    //            break;
    //        }
    //    }
    //    //qDebug()<<"lst="<<lst;
    //    //lst=ts_extremums->last().x;
    //    if(lst!=0 && fst!=0)
    //        return minc*6000/(lst-fst);
    //    else
    //        return -1;

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

int tsanalitics::getAvgInspiratoryTime(){
    int sum=0,cntr=0,i=0;
    for(i=0;i<ts_extremums->size()-1;i++){
        if(ts_extremums->at(i).type==1 && ts_extremums->at(i+1).type==-1){
            sum+=ts_extremums->at(i+1).x-ts_extremums->at(i).x;
            cntr++;
        }
    }
    if(cntr!=0)
        return sum/cntr;
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

int tsanalitics::findExtremums(){
    for (int i = 1; i < ts_row_data->size()-1; ++i) {/*||
                (ts_row_data->at(i-1)==ts_row_data->at(i) ) || ( ts_row_data->at(i)==ts_row_data->at(i+1) )*/
        if( (ts_row_data->at(i-1)<ts_row_data->at(i) && ts_row_data->at(i)>ts_row_data->at(i+1) ) ||
                (ts_row_data->at(i-1)>ts_row_data->at(i) && ts_row_data->at(i)<ts_row_data->at(i+1) )
                ){
            extremum *extr = new extremum;
            extr->y = ts_row_data->at(i);

            extr->x = i;
            if (ts_row_data->at(i-1)<ts_row_data->at(i) && ts_row_data->at(i)>ts_row_data->at(i+1)){
                extr->type=1;
            }
            else{
                extr->type=-1;
            }
            ts_extremums->push_back(*extr);
        }
    }
}

int tsanalitics::deleteBadExtremums(){
    int i=0;
    int cntextr=ts_extremums->size();
    //    qDebug()<<"Bilo";
    //    printExtremums();
    deleteEqualSignExtremums();
    //    qDebug()<<"deleteEqualSignExtremums";
    //    printExtremums();
    deletePatternLightningExtremums();
    //    qDebug()<<"deletePatternLightningExtremums";
    //    printExtremums();
    //    qDebug()<<"No name";
    for (i=0;i<ts_extremums->size()-1;i++){
        if(ts_extremums->at(i+1).x-ts_extremums->at(i).x <50 && fabs(ts_extremums->at(i+1).y-ts_extremums->at(i).y)<600 ){
            if(ts_extremums->at(i).type==1 && ts_extremums->at(i+1).type==1){
                if(ts_extremums->at(i).y > ts_extremums->at(i+1).y)
                    ts_extremums->remove(i+1,1);
                else
                    ts_extremums->remove(i,1);
                //qDebug()<<"Need remove second method 1 ";
                i--;
            }else{
                if(ts_extremums->at(i).type==-1 && ts_extremums->at(i+1).type==-1){
                    if(ts_extremums->at(i).y < ts_extremums->at(i+1).y)
                        ts_extremums->remove(i+1,1);
                    else
                        ts_extremums->remove(i,1);
                    //qDebug()<<"Need remove second method 2 ";
                    i--;
                }
            }
        }
    }
    //    printExtremums();
    deleteSimilarInMeaningExtremums();
    //    qDebug()<<"deleteSimilarInMeaningExtremums";
    //    printExtremums();
    //    qDebug()<<"end";
    /* FILE *out;
    out = fopen("out.csv","w");
    for(i=0;i<ts_extremums->size();i++){
        fprintf(out,"%d\n",ts_extremums->at(i).y);
    }
    fclose(out);*/
    if(ts_extremums->size()!=cntextr){
        return -1;
    }
    else{
        return 1;
    }
}

int tsanalitics::deleteBadExtremumsVolume()
{
    int i;
    for(i=0;i<ts_extremums->size();i++){
        if( ts_extremums->at(i).type==1 ){
            ts_extremums->remove(i,1);
            if(i>0)
                i--;
        }
    }
    int min=getMin();
    for(i=0;i<ts_extremums->size();i++){
        if( fabs(ts_extremums->at(i).y)<0.2*fabs(min) ){
            ts_extremums->remove(i,1);
            if(i>0)
                i--;
        }
    }
    printExtremums();
}

void tsanalitics::approximate()
{
    int i=0;
    QVector<int> *df = new QVector<int>;
    for(i=0;i<ts_row_data->size()-1;i++){
        df->push_back(ts_row_data->at(i+1)-ts_row_data->at(i));
    }
    //    QVector<int> *ddf = new QVector<int>;
    //    for(i=0;i<df->size()-1;i++){
    //        ddf->push_back(df->at(i+1)-df->at(i));
    //    }
    //qDebug()<<ddf->size()<<"size -+------";
    //    for(i=0;i<ddf->size();i++){
    //        if(ddf->at(i)==0){
    //            ddf->remove(i,1);
    //            df->remove(i,1);
    //            ts_row_data->remove(i,1);
    //            if(i>0)
    //                i--;
    //        }
    //    }

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

    for(i=0;i<ts_vol_exts->size()-1;i++){
        if(ts_vol_exts->at(i).y!=0 && ts_vol_exts->at(i+1).y==0 && ts_vol_exts->at(i-1).y==0){
            exhalation *ex = new exhalation;
            ex->start=ts_vol_exts->at(i-1).x;
            ex->end=ts_vol_exts->at(i).x;
            ex->vol=ts_vol_exts->at(i).y;
            ts_exhls->push_back(*ex);
        }
    }
    qDebug()<<ts_exhls->last().end;
    for(i=0;i<ts_vol_exts->size();i++){
        qDebug()<<"aftrd["<<ts_vol_exts->at(i).x<<"]="<<ts_vol_exts->at(i).y;
    }
}
bool tsanalitics::extrValid()
{
    ;
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
    //    //qDebug()<<"BLIAT--------------------------------------------------------------";
    //    int i=0,sum_mn=0,k=0;
    //    QVector<int> exts;
    //    int max=-10000;
    //    for(i=0;i<ts_extremums->size();i++){
    //        //qDebug()<<"fuck"<<ts_extremums->at(i).y;
    //        if(fabs(ts_extremums->at(i).y)>max)
    //            max=fabs(ts_extremums->at(i).y);
    //    }
    //    for( i=0;i<ts_extremums->size();i++){
    //        if(ts_extremums->at(i).type==-1){
    //            if( ts_extremums->at(i).y!=0 && fabs(ts_extremums->at(i).y)>max/2){
    //                //                if(fabs(fabs(ts_extremums->at(i).y)-max)<2000 && fabs(ts_extremums->at(i).y)<11000 && ts_extremums->at(i).y!=0 && fabs(ts_extremums->at(i).y)>600){
    //                sum_mn+=fabs(ts_extremums->at(i).y);
    //                k++;
    //                exts.append(fabs(ts_extremums->at(i).y));
    //                //qDebug()<<"ts_extremums->at(i).y"<<ts_extremums->at(i).y<<"sum_mn"<<sum_mn;
    //            }
    //        }
    //    }
    //    /*for(i=1;i<exts.size();i++){
    //        if(fabs(fabs(ts_extremums->at(i).y)-fabs(ts_extremums->at(i-1).y)) >300 ){
    //            if(fabs(ts_extremums->at(i).y)>fabs(ts_extremums->at(i-1).y)){
    //                ts_extremums->remove(i-1,1);
    //                qDebug()<<"remove first";
    //            }
    //            else{
    //                ts_extremums->remove(i,1);
    //                qDebug()<<"remove second";
    //            }
    //        }
    //    }
    //    for(i=0;i<exts.size();i++){
    //        sum_mn+=fabs(exts.at(i));
    //        k++;
    //    }*/
    //    if(k>0){
    //        return sum_mn/k;
    //    }
    //    else
    //        return -1;
}

int tsanalitics::getAvgInspiratorySpeed(){
    int i=0,sum=0,count=0;
    //qDebug()<<"FUCK getAvgInspiratorySpeed";
    for(i=0;i<ts_extremums->size()-1;i++){
        if( ts_extremums->at(i).type==-1 && ts_extremums->at(i+1).type==1 && ts_extremums->at(i+1).x-ts_extremums->at(i).x<300 &&ts_extremums->at(i+1).x-ts_extremums->at(i).x>30){
            sum+=(fabs(ts_extremums->at(i).y))/(ts_extremums->at(i+1).x-ts_extremums->at(i).x);
            //qDebug()<<sum;
            //                qDebug()<<"getAvgInspiratorySpeed "<<ts_extremums->at(i).y;
            //                qDebug()<<"ts_extremums->at(i).x "<<ts_extremums->at(i).x<<" "<<ts_extremums->at(i+1).x;
            count++;
        }
    }
    if(count)
        return sum/count;
    else
        return -1;
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

int tsanalitics::getMaxInspiratorySpeed(){
    int i=0,speed=-10000,oldspeed=-10000;
    for(i=0;i<ts_extremums->size()-1;i++){
        if( ts_extremums->at(i).type==-1 && ts_extremums->at(i+1).type==1 ){
            speed=(fabs(ts_extremums->at(i+1).y))/(ts_extremums->at(i+1).x-ts_extremums->at(i).x);
            if(speed > oldspeed ){
                oldspeed=speed;
            }
        }
    }
    return oldspeed;
}

int tsanalitics::getMaxExpiratorySpeed(){
    int cntr=0,i=0,len=0;
    float speed=0,max=-1000000;
    for(i=0;i<ts_exhls->size();i++){
        speed=(float)ts_exhls->at(i).vol/(float)(ts_exhls->at(i).end-ts_exhls->at(i).start);
        if(speed>max)
            max=speed;
    }
    return max;
    //    int sum=0,cntr=0,i=0,max=-1000000000;
    //       for(i=0;i<ts_exhls->size();i++){
    //           if ( fabs(ts_exhls->at(i).vol/(ts_exhls->at(i).end-ts_exhls->at(i).start))>max)
    //                max=ts_exhls->at(i).vol/(ts_exhls->at(i).end-ts_exhls->at(i).start);
    //       }
    //       return max;
    //    int i=0,speed=-10000,oldspeed=-10000;
    //    for(i=0;i<ts_extremums->size()-1;i++){
    //        if( ts_extremums->at(i).type==1 && ts_extremums->at(i+1).type==-1 ){
    //            speed=(fabs(ts_extremums->at(i).y))/(ts_extremums->at(i+1).x-ts_extremums->at(i).x);
    //            if(speed > oldspeed ){
    //                oldspeed=speed;
    //            }
    //        }
    //    }
    //    return oldspeed;
}

void tsanalitics::append(int n){
    ts_row_data->push_back(n);
}

void tsanalitics::clear(){
    int i=0;
    ts_extremums->remove(0,ts_extremums->size());
    /* for(i=0;i<ts_extremums->size();i++){
        ts_extremums->erase(i);
    }*/
    ts_extremums->clear();
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

    //    int air = getMinsSum();
    //    int sum=0, i=0;
    //    for(i=0;i<ts_extremums->size();i++){
    //        if(ts_extremums->at(i).type==-1)
    //            sum+=abs(ts_extremums->at(i).y);
    //    }
    //    //int time = getTime();
    //    return sum;
    //    /*if( time!=0)
    //            return air*6000/time;
    //        else
    //            return -1;*/
}

void tsanalitics::printVec(int k){
    int i=0;
    if(k>0)
        for(i=0;i<ts_extremums->size();i++){
            if(ts_extremums->at(i).type==1)
                printf("%d ",ts_extremums->at(i).y);
        }
    else
        for(i=0;i<ts_extremums->size();i++){
            if(ts_extremums->at(i).type==-1)
                printf("%d ",ts_extremums->at(i).y);
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

QVector<extremum> *tsanalitics::getExtremums(){
    return ts_extremums;
}

void tsanalitics::printExtremums()
{
    int i=0;
    for(i=0;i<ts_extremums->size();i++){
        qDebug()<<"type="<<ts_extremums->at(i).type<<" x="<<ts_extremums->at(i).x<<" y="<<ts_extremums->at(i).y;

    }
}



int tsanalitics::fabs(int a){
    if( a<0 )
        return -a;
    else
        return a;
}

void tsanalitics::deleteEqualSignExtremums(){
    int i=0;
    for(i=0;i<ts_extremums->size()-1;i++){
        if( ts_extremums->at(i).y == ts_extremums->at(i+1).y ){
            ts_extremums->remove(i);
            if (i)
                i--;
        }
    }
    for(i=0;i<ts_extremums->size()-1;i++){
        if( ts_extremums->at(i).type == ts_extremums->at(i+1).type ){
            if(ts_extremums->at(i).type==1){
                if(ts_extremums->at(i).y < ts_extremums->at(i+1).y){
                    ts_extremums->remove(i);
                }else{
                    ts_extremums->remove(i+1);
                }
                if (i)
                    i--;
            }
            if(ts_extremums->at(i).type==-1){
                if(ts_extremums->at(i).y > ts_extremums->at(i+1).y){
                    ts_extremums->remove(i);
                }else{
                    ts_extremums->remove(i+1);
                }
                if (i)
                    i--;
            }
        }
    }
}

void tsanalitics::deleteSimilarInMeaningExtremums(){
    int i=0;
    for (i=0;i<ts_extremums->size()-1;i++){
        if(ts_extremums->at(i+1).x-ts_extremums->at(i).x <50 && fabs(ts_extremums->at(i+1).y-ts_extremums->at(i).y)<600 ){
            if ( fabs(fabs(getMax())-fabs(ts_extremums->at(i).y)) < fabs(fabs(ts_extremums->at(i).y)-fabs(getMin())) &&
                 fabs(fabs(getMax())-fabs(ts_extremums->at(i+1).y)) < fabs(fabs(ts_extremums->at(i+1).y)-fabs(getMin()))
                 ){
                if(ts_extremums->at(i).y <= ts_extremums->at(i+1).y)
                    ts_extremums->remove(i,1);
                else
                    ts_extremums->remove(i+1,1);
                if (i)
                    i--;
            }else if ( fabs(fabs(getMax())-fabs(ts_extremums->at(i).y)) > fabs(fabs(ts_extremums->at(i).y)-fabs(getMin())) &&
                       fabs(fabs(getMax())-fabs(ts_extremums->at(i+1).y)) > fabs(fabs(ts_extremums->at(i+1).y)-fabs(getMin()))
                       ){
                if(ts_extremums->at(i).y >= ts_extremums->at(i+1).y)
                    ts_extremums->remove(i,1);
                else
                    ts_extremums->remove(i+1,1);
                if (i)
                    i--;
            }
        }
    }
}

void tsanalitics::deletePatternLightningExtremums(){
    int i=0;
    for (i=3;i<ts_extremums->size();i++){
        if( ts_extremums->at(i).x-ts_extremums->at(i-3).x < 110 )
            //( ts_extremums->at(i).type==-1 && ts_extremums->at(i-1).type==1 && ts_extremums->at(i-2).type==-1 && ts_extremums->at(i-3).type==1 )
            //&&
            if (  (ts_extremums->at(i).y < ts_extremums->at(i-1).y && ts_extremums->at(i).y < ts_extremums->at(i-2).y  && ts_extremums->at(i).y < ts_extremums->at(i-3).y )
                  && (ts_extremums->at(i-3).y > ts_extremums->at(i-2).y && ts_extremums->at(i-3).y > ts_extremums->at(i-1).y && ts_extremums->at(i-3).y > ts_extremums->at(i).y)
                  && (ts_extremums->at(i-1).y > ts_extremums->at(i).y && ts_extremums->at(i-1).y > ts_extremums->at(i-2).y && ts_extremums->at(i-1).y < ts_extremums->at(i-3).y)
                  && (ts_extremums->at(i-2).y > ts_extremums->at(i).y && ts_extremums->at(i-2).y < ts_extremums->at(i-1).y && ts_extremums->at(i-2).y < ts_extremums->at(i-3).y)
                  ){
                ts_extremums->remove(i-2,2);
                if (i)
                    i--;
                //qDebug()<<"Need remove middle first figure ";
                //(ts_extremums->at(i).type==1 && ts_extremums->at(i-1).type==-1 && ts_extremums->at(i-2).type==1 && ts_extremums->at(i-3).type==-1 )
                //&&
            }else if (  (ts_extremums->at(i).y > ts_extremums->at(i-1).y && ts_extremums->at(i).y > ts_extremums->at(i-2).y  && ts_extremums->at(i).y > ts_extremums->at(i-3).y )
                        && (ts_extremums->at(i-3).y < ts_extremums->at(i-2).y && ts_extremums->at(i-3).y < ts_extremums->at(i-1).y && ts_extremums->at(i-3).y < ts_extremums->at(i).y)
                        && (ts_extremums->at(i-1).y < ts_extremums->at(i).y && ts_extremums->at(i-1).y < ts_extremums->at(i-2).y && ts_extremums->at(i-1).y > ts_extremums->at(i-3).y)
                        && (ts_extremums->at(i-2).y < ts_extremums->at(i).y && ts_extremums->at(i-2).y > ts_extremums->at(i-1).y && ts_extremums->at(i-2).y > ts_extremums->at(i-3).y)
                        ){
                ts_extremums->remove(i-2,2);
                if (i)
                    i--;
                //qDebug()<<"Need remove middle second figure ";
            }
    }
}
