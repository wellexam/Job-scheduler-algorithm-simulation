#include<iostream>
#include<fstream>
#include<windows.h>

using namespace std;

class job
{
    public:
        int number;          //作业号
        int reach_time;     //作业抵达时间
        int need_time;      //作业的执行时间
        int privilege;	  //作业优先权
        float excellent;     //响应比
        int start_time; 	  //作业开始时间
        int wait_time;      //等待时间
        int tr_time;        //周转时间
        double wtr_time;   //带权周转时间
        int run_time;       //作业累计已执行时间
        int visited;		  //作业是否被访问过
        bool isreached;	  //作业是否已抵达
};

void read_Jobdata(); //读取数据文件；
void initial_jobs(); //初始化所有作业信息
int findEarlyJob(job jobs[],int count);//找到执行时间最短的作业
void SFJschedulejob(job jobs[],int count);//短作业优先算法
void FCFSschedulejob(job jobs[],int count);//先来先服务算法
void calculate(job jobs[]);//计算平均时间

static job que[100];

int main()
{
    read_Jobdata();
    cout << "作业ID 到达时间 执行时间 优先权" << endl;
    for (int i = 0; que[i].number; i++)
    {
        cout << que[i].number << "        "
             << que[i].reach_time << "       "
             << que[i].need_time << "       "
             << que[i].privilege << endl;
    }
    cout << "先来先服务：" << endl;
    FCFSschedulejob(que, 700);
    return 0;
}

void read_Jobdata()
{
    fstream file;
    file.open("data.txt", ios::in);
    int i = 0;
    while(!file.eof())
    {
        file >> que[i].number;
        file >> que[i].reach_time;
        file >> que[i].need_time;
        file >> que[i].privilege;
        i++;
    }
    que[i].number = 0;
    return;
}

void FCFSschedulejob(job jobs[],int count)
{
    int mark=-1, early = 10000000;
    // cout << "作业ID 到达时间 执行时间 优先权 等待时间 周转时间 带权周转时间" << endl;
    // for (int i = 0; jobs[i].number; i++)
    // {
    //     cout << jobs[i].number << "        "
    //          << jobs[i].reach_time << "       "
    //          << jobs[i].need_time << "       "
    //          << jobs[i].privilege << "        "
    //          << jobs[i].wait_time << "       "
    //          << jobs[i].tr_time << "       "
    //          << jobs[i].wtr_time << endl;
    // }
    for (int i = 0; jobs[i].number; i++)
    {
        if(jobs[i].reach_time>0)
        {
            if(jobs[i].reach_time<=early)
            {
                early = jobs[i].reach_time;
                mark = i;
                // cout  << "early: " << early << endl;
                // cout << "count: " << count << endl;
                // cout << "i: " << i << endl;
            }
        }
    }
    if(mark==-1)
    {
        calculate(jobs);
        return;
    }
    if(early<=count)
    {
        jobs[mark].wait_time = count - jobs[mark].reach_time;//等待时间
        count += jobs[mark].need_time;//执行作业
        jobs[mark].tr_time = count - jobs[mark].reach_time;//周转时间
        jobs[mark].wtr_time = (jobs[mark].tr_time*1.0) / jobs[mark].need_time;//带权周转时间
        jobs[mark].reach_time = 0;
        cout << "执行完的作业是:  " << jobs[mark].number << "号作业，"
            << " 等待时间为 " << jobs[mark].wait_time
            << " 周转时间为 " << jobs[mark].tr_time
            << " 带权周转时间为 " << jobs[mark].wtr_time
            << endl;
        FCFSschedulejob(jobs, count);
        return;
    }
    else
    {
        jobs[mark].wait_time = 0;//等待时间
        count = jobs[mark].reach_time + jobs[mark].need_time;//执行作业
        jobs[mark].tr_time = count - jobs[mark].reach_time;//周转时间
        jobs[mark].wtr_time = (jobs[mark].tr_time*1.0) / jobs[mark].need_time;//带权周转时间
        jobs[mark].reach_time = 0;
        cout << "执行完的作业是:  " << jobs[mark].number << "号作业，"
            << " 等待时间为 " << jobs[mark].wait_time
            << " 周转时间为 " << jobs[mark].tr_time
            << " 带权周转时间为 " << jobs[mark].wtr_time
            << endl;
        FCFSschedulejob(jobs, count);
        return;
    }
}

void calculate(job jobs[])
{
    int avg_wait = 0, avg_tr = 0;
    double avg_wtr = 0;
    int i;
    for (i = 0; jobs[i].number; i++)
    {
        avg_wait += jobs[i].wait_time;
        avg_tr += jobs[i].tr_time;
        avg_wtr += jobs[i].wtr_time;
    }
    double div = i;
    cout << "平均等待时间: " << avg_wait/div << endl
         << "平均周转时间: " << avg_tr/div << endl
         << "平均带权周转时间: " << avg_wtr/i << endl;
    return;
}