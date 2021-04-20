#include<iostream>
#include<fstream>

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
    int mark, early;
    for (int i = 0; jobs[i].number; i++)
    {
        if(jobs[i].reach_time<=count&&jobs[i].reach_time)
        {
            if(jobs[i].reach_time<early)
            {
                early = jobs[i].reach_time;
                mark = i;
            }
        }
    }
    count += jobs[mark].need_time;
    jobs[mark].reach_time = 0;
}