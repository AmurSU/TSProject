#include "volumesolver.h"


VolumeSolver::VolumeSolver(QVector<int> volume, QVector<int> tempin, QVector<int> tempout){
    QPoint point;
    for(int i=0;i<volume.size();i++){
        point.setX(i);
        point.setY(abs(volume.at(i)));
        raw_data.push_back(point);
    }
    for(int i=0;i<tempin.size();i++){
        point.setX(i);
        point.setY(tempin.at(i));
        temp_in.push_back(point);
    }
    for(int i=0;i<tempout.size();i++){
        point.setX(i);
        point.setY(tempout.at(i));
        temp_out.push_back(point);
    }
    prepareData();
    qDebug()<<"AverageInspirationTime"<<getAverageInspirationTime();
    qDebug()<<"AverageExpirationTime"<<getAverageExpirationTime();
    qDebug()<<"InspirationFrequancyInOneMinute"<<getInspirationFrequancyInOneMinute();
    qDebug()<<"AverageCycleTime"<<getAverageCycleTime();
    qDebug()<<"TotalVentilation"<<getTotalVentilation();
    qDebug()<<"MinuteVentilation"<<getMinuteVentilation();
    qDebug()<<"AverageExpirationSpeed"<<getAverageExpirationSpeed();
    qDebug()<<"MaxExpirationSpeed"<<getMaxExpirationSpeed();
    qDebug()<<"AverageInspirationTempetature"<<getAverageInspirationTempetature();
    qDebug()<<"AverageExpirationTempetature"<<getAverageExpirationTempetature();
}

void VolumeSolver::prepareData(){
    if ( raw_data.size() == 0 )
        return;
    approximate();
    deleteEqualNoisePoints();
    //printSignal();
    takeCycles();
}

void VolumeSolver::printSignal(){
    FILE *out;
    out=fopen("output.csv","w");
    for (int i =0;i<prepared_data.size();i++)
        fprintf(out,"%d\n",prepared_data.at(i).y());
    fclose(out);
}

void VolumeSolver::approximate(){

    if ( raw_data.size() == 0 )
        return;
    qDebug()<<"Approxymate";
    /*Angle between lines formed by 3 point in sequence*/
    float angle_betw_1_2=0,angle_betw_2_3=0,angle_betw_1_3=0;
    /*Maximum value of angle between 3 lines for approximation this 3 point in line */
    float divergence_angle=30;

    prepared_data = raw_data;

    for(int i=0;i<prepared_data.size()-2;i++){
        /*Get tangens for angle between ox and line*/
        angle_betw_1_2=(float)(prepared_data.at(i+1).y()-prepared_data.at(i).y())
                /((float)(prepared_data.at(i+1).x()-prepared_data.at(i).x()));
        angle_betw_2_3=(float)(prepared_data.at(i+2).y()-prepared_data.at(i+1).y())
                /((float)(prepared_data.at(i+2).x()-prepared_data.at(i+1).x()));
        angle_betw_1_3=(float)(prepared_data.at(i+2).y()-prepared_data.at(i).y())
                /((float)(prepared_data.at(i+2).x()-prepared_data.at(i).x()));
        /*Get angle between ox and line in degees*/
        angle_betw_1_2=atan(angle_betw_1_2)*180/M_PI;
        angle_betw_2_3=atan(angle_betw_2_3)*180/M_PI;
        angle_betw_1_3=atan(angle_betw_1_3)*180/M_PI;
        /*If angle between 1 and 2, 2 and 3 lines smaller then divergence_angle*/
        if ( fabs(angle_betw_1_3-angle_betw_1_2) < divergence_angle && fabs(angle_betw_1_3-angle_betw_2_3)<divergence_angle ){
            prepared_data.remove(i+1,1);
            if ( i!=0 )
                i--;
        }
    }
}

void VolumeSolver::deleteEqualNoisePoints(){

    qDebug()<<"deleteEqualNoisePoints";
    printSignal();
    if ( raw_data.size() < 3 )
        return;
    int koef = 20;
    //for(int i=0;i<prepared_data.size()-1;i++)
    int i=0;
    while ( i < prepared_data.size()-1 ){
        if ( fabs(prepared_data.at(i+1).y() - prepared_data.at(i).y()) < koef  && prepared_data.at(i+1).y() > 100 && prepared_data.at(i).y()>100 ){//
            qDebug()<<"y1="<<prepared_data.at(i+1).y()<<"y2="<<prepared_data.at(i).y();
            prepared_data.remove(i,1);
            if ( i!=0)
                i--;
        }else
            i++;
    }
    i=0;
    while ( i < prepared_data.size()-2 ){
        if ( prepared_data.at(i).y()==0 && prepared_data.at(i+1).y()==0 && prepared_data.at(i+2).y()==0 ){
            prepared_data.remove(i,1);
            if ( i!=0)
                i--;
        }else
            i++;
    }
}

void VolumeSolver::takeCycles(){
    if ( prepared_data.size() == 0 )
        return;
    cycle cl;
    int count_cl=0;
    while( count_cl+3< prepared_data.size() ){
        cl.in.setX(prepared_data.at(count_cl).x());
        cl.in.setY(prepared_data.at(count_cl).y());
        cl.mid.setX(prepared_data.at(count_cl+1).x());
        cl.mid.setY(prepared_data.at(count_cl+1).y());
        cl.ext.setX(prepared_data.at(count_cl+2).x());
        cl.ext.setY(prepared_data.at(count_cl+2).y());
        cl.out.setX(prepared_data.at(count_cl+3).x());
        cl.out.setY(prepared_data.at(count_cl+3).y());
        /*cl.in = prepared_data.at(count_cl);
        cl.mid = prepared_data.at(count_cl+1);
        cl.ext = prepared_data.at(count_cl+2);
        cl.ext = prepared_data.at(count_cl+3);*/
        cycles.push_back(cl);
        count_cl+=3;
    }
    for(int i=0;i<cycles.size();i++){
        qDebug()<<"in("<<cycles.at(i).in.x()<<";"<<cycles.at(i).in.y()<<") mid("<<cycles.at(i).mid.x()<<";"<<cycles.at(i).mid.y()<<") ext("<<cycles.at(i).ext.x()<<";"<<cycles.at(i).ext.y()<<")"<<" out("<<cycles.at(i).out.x()<<";"<<cycles.at(i).out.y()<<")";
    }
}

QVector<QPoint> VolumeSolver::getSignal(){
    QVector<QPoint> ret;
    QPoint point;
    ret = prepared_data;
    //    qDebug()<<"raw size ="<<raw_data.size();
    //    qDebug()<<"prepared size="<<prepared_data.size();
    //    for (int i=0;i<prepared_data.at(i).x();i++){
    //        point.setX(i);
    //        point.setY(prepared_data.at(i).y());
    //        ret.push_back(point);
    //    }
    //    for (int i=0;i<prepared_data.size()-1;i++ ){
    //        for (int j=prepared_data.at(i).x();j<prepared_data.at(i+1).x();j++){
    //            point.setX(j);
    //            point.setY(prepared_data.at(i).y());
    //            ret.push_back(point);
    //        }
    //    }
    //    ret.push_back(prepared_data.last());

    return ret;
}

float VolumeSolver::getAverageInspirationTime(){
    if ( cycles.size() == 0 )
        return 0;
    float timesumm=0;
    for(int i=0;i<cycles.size();i++)
        timesumm+=cycles.at(i).mid.x()-cycles.at(i).in.x();
    if ( cycles.size()>0 )
        return timesumm/cycles.size()/FREQUANCY;
    else
        return 0;
}

float VolumeSolver::getAverageExpirationTime(){
    if ( cycles.size() == 0 )
        return 0;
    float timesumm=0;
    for(int i=0;i<cycles.size();i++)
        timesumm+=cycles.at(i).ext.x()-cycles.at(i).mid.x();
    if ( cycles.size() != 0 && timesumm != 0)
        return timesumm/cycles.size()/FREQUANCY;
    else
        return 0;
}

float VolumeSolver::getMaxExpirationSpeed(){
    if ( cycles.size() == 0 )
        return 0;
    float ventilation=0;
    float timesumm=0;
    float maxspeed=0;
    for(int i=0;i<cycles.size();i++){
        timesumm=cycles.at(i).ext.x()-cycles.at(i).mid.x();
        ventilation=cycles.at(i).ext.y()-cycles.at(i).mid.y();
        if ( maxspeed < ventilation/timesumm/FREQUANCY )
            maxspeed = ventilation/(timesumm/FREQUANCY);
    }
    return maxspeed;
}

float VolumeSolver::getAverageExpirationSpeed(){
    if ( cycles.size() == 0 )
        return 0;
    float ventilation = getTotalVentilation();
    float timesumm=0;
    for(int i=0;i<cycles.size();i++)
        timesumm+=cycles.at(i).out.x()-cycles.at(i).mid.x();
    if ( timesumm != 0 && ventilation != 0 )
        return ventilation/(timesumm/FREQUANCY);
    return 0;
}

float VolumeSolver::getInspirationFrequancyInOneMinute(){
    if ( cycles.size() == 0 )
        return 0;
    float inspiration_counter = cycles.size();
    float full_time = cycles.last().out.x();
    if ( full_time != 0 && inspiration_counter != 0 )
        return 60*inspiration_counter/(full_time/FREQUANCY);
    else
        return 0;
}

float VolumeSolver::getAverageCycleTime(){
    if ( cycles.size() == 0 )
        return 0;
    float inspiration_counter = cycles.size();
    float full_time = cycles.last().out.x();
    if ( inspiration_counter != 0 && full_time !=0 )
        return full_time/inspiration_counter/FREQUANCY;
    else
        return 0;
}

float VolumeSolver::getTotalVentilation(){
    if ( cycles.size() == 0 )
        return 0;
    float total=0;
    for(int i=0;i<cycles.size();i++)
        total+=cycles.at(i).ext.y();
    return total;
}

float VolumeSolver::getMinuteVentilation(){
    if ( cycles.size() == 0 )
        return 0;
    float total=0;
    float full_time = cycles.last().out.x();
    for(int i=0;i<cycles.size();i++)
        total+=cycles.at(i).ext.y();
    if ( total != 0 && full_time != 0 )
        return total/(full_time/FREQUANCY/60);
    return 0;
}

float VolumeSolver::getAverageInspirationTempetature(){
    if ( cycles.size() == 0 )
        return 0;
    float tempsumm=0;
    for(int i=0;i<cycles.size();i++){
        tempsumm+=temp_in.at(cycles.at(i).mid.x()).y();
        //qDebug()<<"insp"<<temp_in.at(cycles.at(i).mid.x()).y();
    }
    if ( cycles.size()!= 0 && tempsumm != 0 )
        return tempsumm/cycles.size();
    return 0;
}

float VolumeSolver::getAverageExpirationTempetature(){
    if ( cycles.size() == 0 )
        return 0;
    float tempsumm=0;
    for(int i=0;i<cycles.size();i++){
        tempsumm+=temp_out.at(cycles.at(i).ext.x()).y();
        //qDebug()<<"exp"<<temp_out.at(cycles.at(i).ext.x()).y();
    }
    if ( cycles.size()!= 0 && tempsumm != 0 )
        return tempsumm/cycles.size();
    return 0;
}

float VolumeSolver::getAverageInspiratonVentilation(){
    float minvent = getMinuteVentilation();
    float freq= getInspirationFrequancyInOneMinute();
    if ( minvent != 0 && freq != 0 )
        return minvent/freq;
    return 0;
}



