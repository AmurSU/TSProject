#include "tsanalitics.h"
#include <QDebug>
tsanalitics::tsanalitics(QObject *parent) :QObject(parent){
    QVector<extremum> *extrem = new QVector<extremum>;
    ts_extremums=extrem;
    QVector<int> *row_d = new QVector<int>;
    ts_row_data = row_d;
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

int tsanalitics::getFrequency(){
    int i=0,fst=0,lst=0;
    int minc = getMinsCount();


    for(i=0;i<ts_extremums->size();i++){
        if(ts_extremums->at(i).type==-1){
            fst =ts_extremums->at(i).x;
            break;
        }
    }
    qDebug()<<"fst="<<fst;
    for(i=ts_extremums->size()-1;i>0;i--){
        if(ts_extremums->at(i).type==-1){
            lst = ts_extremums->at(i).x;
            break;
        }
    }
    qDebug()<<"lst="<<lst;
    //lst=ts_extremums->last().x;
    if(lst!=0 && fst!=0)
        return minc*6000/(lst-fst);
    else
        return -1;

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
    int sum=0,cntr=0,i=0;
    for(i=0;i<ts_extremums->size()-1;i++){
        if(ts_extremums->at(i).type==-1 && ts_extremums->at(i+1).type==1){
            sum+=ts_extremums->at(i+1).x-ts_extremums->at(i).x;
            cntr++;
        }
    }
    if(cntr!=0)
        return sum/cntr;
    else
        return 0;
}

int tsanalitics::setupData(QVector<int> *row_d){
    ts_row_data=row_d;
}

int tsanalitics::findExtremums(){


    qDebug()<<ts_row_data->size();
    for (int i = 1; i < ts_row_data->size()-1; ++i) {
        if( (ts_row_data->at(i-1)<=ts_row_data->at(i) && ts_row_data->at(i)>=ts_row_data->at(i+1) ) ||
                (ts_row_data->at(i-1)>=ts_row_data->at(i) && ts_row_data->at(i)<=ts_row_data->at(i+1) ) ||
                (ts_row_data->at(i-1)==ts_row_data->at(i) ) || ( ts_row_data->at(i)==ts_row_data->at(i+1) )
                ){
            //qDebug()<<"extremum exist"<<ts_row_data->at(i);
            extremum *extr = new extremum;
            extr->y = ts_row_data->at(i);

            extr->x = i;
            if (ts_row_data->at(i-1)<ts_row_data->at(i) && ts_row_data->at(i)>ts_row_data->at(i+1)){
                extr->type=1;
                //qDebug()<<"max="<<ts_row_data->at(i);
            }
            else{
                extr->type=-1;
                //qDebug()<<"min="<<ts_row_data->at(i);
            }
            ts_extremums->push_back(*extr);
        }
    }
    //qDebug()<<"all max="<<getMax();
    //qDebug()<<"all min="<<getMin();
}

int tsanalitics::deleteBadExtremums(){
    int i=0;
    int cntextr=ts_extremums->size();
    deleteEqualSignExtremums();
    deletePatternLightningExtremums();

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

    deleteSimilarInMeaningExtremums();
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

int tsanalitics::getBreathingVolume(){
    int i=0,sum_mn=0,k=0;
    QVector<int> exts;
    qDebug()<<"Fuck";
    int max=-10000;
    for(i=0;i<ts_extremums->size();i++){
        if(fabs(ts_extremums->at(i).y)>max)
            max=fabs(ts_extremums->at(i).y);
    }
    for( i=0;i<ts_extremums->size();i++){
        if(ts_extremums->at(i).type==-1){
            if(fabs(fabs(ts_extremums->at(i).y)-max)<400 && fabs(ts_extremums->at(i).y)<9000 && ts_extremums->at(i).y!=0 && fabs(ts_extremums->at(i).y)>600){
                sum_mn+=fabs(ts_extremums->at(i).y);
                k++;
                exts.append(fabs(ts_extremums->at(i).y));
                qDebug()<<"ts_extremums->at(i).y"<<ts_extremums->at(i).y<<"sum_mn"<<sum_mn;
            }
        }
    }
    /*for(i=1;i<exts.size();i++){
        if(fabs(fabs(ts_extremums->at(i).y)-fabs(ts_extremums->at(i-1).y)) >300 ){
            if(fabs(ts_extremums->at(i).y)>fabs(ts_extremums->at(i-1).y)){
                ts_extremums->remove(i-1,1);
                qDebug()<<"remove first";
            }
            else{
                ts_extremums->remove(i,1);
                qDebug()<<"remove second";
            }
        }
    }
    for(i=0;i<exts.size();i++){
        sum_mn+=fabs(exts.at(i));
        k++;
    }*/
        if(k>0){
            //return fabs(sum_mn/k);
            return max;
        }
        else
            return -1;
    }

    int tsanalitics::getAvgInspiratorySpeed(){
        int i=0,sum=0,count=0;
        for(i=0;i<ts_extremums->size()-1;i++){
            if( ts_extremums->at(i).type==-1 && ts_extremums->at(i+1).type==1 ){
                sum+=(fabs(ts_extremums->at(i).y))/(ts_extremums->at(i+1).x-ts_extremums->at(i).x);
                count++;
            }
        }
        if(count)
            return sum/count;
        else
            return -1;
    }

    int tsanalitics::getAvgExpiratorySpeed(){
        int i=0,sum=0,count=0;
        for(i=0;i<ts_extremums->size()-1;i++){
            if( ts_extremums->at(i).type==1 && ts_extremums->at(i+1).type==-1 ){
                sum+=(fabs(ts_extremums->at(i).y))/(ts_extremums->at(i+1).x-ts_extremums->at(i).x);
                count++;
            }
        }
        if(count)
            return sum/count;
        else
            return -1;
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
        int i=0,speed=-10000,oldspeed=-10000;
        for(i=0;i<ts_extremums->size()-1;i++){
            if( ts_extremums->at(i).type==1 && ts_extremums->at(i+1).type==-1 ){
                speed=(fabs(ts_extremums->at(i).y))/(ts_extremums->at(i+1).x-ts_extremums->at(i).x);
                if(speed > oldspeed ){
                    oldspeed=speed;
                }
            }
        }
        return oldspeed;
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
        int air = getMinsSum();
        int time = getTime();
        return air;
        if( time!=0)
            return air*6000/time;
        else
            return -1;
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
