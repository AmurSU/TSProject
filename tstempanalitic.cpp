#include "tstempanalitic.h"
#include <QDebug>
tstempanalitic::tstempanalitic(QObject *parent) :QObject(parent){
    ts_extremums = new QVector<extremum>;
    ts_row_data = new QVector<int>;
}

tstempanalitic::~tstempanalitic(){
    delete ts_extremums;
    delete ts_row_data;
}

int tstempanalitic::getMaxAvgs(){
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

int tstempanalitic::getMinAvgs(){
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

int tstempanalitic::getMaxsCount(){
    int i=0, cntr=0;
    for(i=0; i<ts_extremums->size();i++)
        if( ts_extremums->at(i).type == 1 )
            cntr++;
    return cntr;
}

int tstempanalitic::getMinsCount(){
    int i=0, cntr=0;
    for(i=0; i<ts_extremums->size();i++)
        if( ts_extremums->at(i).type == -1 )
            cntr++;
    return cntr;
}

int tstempanalitic::getMinMaxAvgTime(){
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

int tstempanalitic::getMaxMinAvgTime(){
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

int tstempanalitic::setupData(QVector<int> *row_d){
    ts_row_data=row_d;
    return ts_row_data->size();
}

int tstempanalitic::findExtremums(){
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
    return 0;
}

int tstempanalitic::deleteBadExtremums(){
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
    if(ts_extremums->size()!=cntextr){
        return -1;
    }
    else{
        return 1;
    }
}

int tstempanalitic::deleteBadExtremumsVolume()
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
    //printExtremums();
    return 0;
}

void tstempanalitic::append(int n){
    ts_row_data->push_back(n);
}

void tstempanalitic::clear(){
    ts_extremums->remove(0,ts_extremums->size());
    ts_extremums->clear();
}

int tstempanalitic::getMaxsSum(){
    int sum=0, i=0;
    for(i=0;i<ts_extremums->size();i++){
        if(ts_extremums->at(i).type==1)
            sum+=ts_extremums->at(i).y;
    }
    return sum;
}

int tstempanalitic::getMinsSum(){
    int sum=0, i=0;
    for(i=0;i<ts_extremums->size();i++){
        if(ts_extremums->at(i).type==-1)
            sum+=ts_extremums->at(i).y;
    }
    return sum;
}

int tstempanalitic::getMax(){
    int i=0, max_index=0;
    for (i=0;i<ts_row_data->size();i++){
        if( ts_row_data->at(i)> ts_row_data->at(max_index) )
            max_index = i;
    }
    return ts_row_data->at(max_index);
}

int tstempanalitic::getMin(){
    int i=0, min_index=0;
    for (i=0;i<ts_row_data->size();i++){
        if( ts_row_data->at(i)< ts_row_data->at(min_index) )
            min_index = i;
    }
    return ts_row_data->at(min_index);
}


int tstempanalitic::fabs(int a){
    if( a<0 )
        return -a;
    else
        return a;
}

void tstempanalitic::deleteEqualSignExtremums(){
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

void tstempanalitic::deleteSimilarInMeaningExtremums(){
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

void tstempanalitic::deletePatternLightningExtremums(){
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
