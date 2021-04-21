#include <fstream>
#include <iomanip>
#include <iostream>
#include <list>

using namespace std;

class job
{
public:
    int number;      //作业号
    int reach_time;  //作业抵达时间
    int need_time;   //作业的执行时间
    int privilege;   //作业优先权
    float excellent; //响应比
    int start_time;  //作业开始时间
    int wait_time;   //等待时间
    int tr_time;     //周转时间
    double wtr_time; //带权周转时间
    int run_time;    //作业累计已执行时间
    bool visited;    //作业是否被访问过
    bool isreached;  //作业是否已抵达

    void operator=(const job &b)
    {
        number = b.number;
        reach_time = b.reach_time;
        need_time = b.need_time;
        privilege = b.privilege;
        start_time = b.wait_time;
        wait_time = b.wait_time;
        tr_time = b.tr_time;
        wtr_time = b.wtr_time;
        run_time = b.run_time;
        visited = b.visited;
        isreached = b.isreached;
    }
};

void read_Jobdata();                          //读取数据文件
void print_Jobdata();                         //打印数据
void initial_jobs();                          //初始化所有作业信息
void calculate(job jobs[]);                   //计算平均时间
void initial_rr();                            //初始化rr序列
void reach(int time);                         //判断到达与否
void early();                                 //查找最早到达
void SJF_schedulejob(job jobs[], int count);  //短作业优先算法
void FCFS_schedulejob(job jobs[], int count); //先来先服务算法
void RR_schedulejob();                        //时间片轮转算法

static job que[100], backup[100];
static list<job> rr;

int main()
{
    read_Jobdata();
    print_Jobdata();
    cout << "先来先服务：" << endl;
    FCFS_schedulejob(que, 0);

    initial_jobs();

    cout << "短作业优先服务： " << endl;
    SJF_schedulejob(que, 0);

    initial_jobs();
    initial_rr();

    cout << "时间片轮转: " << endl;
    return 0;
}

void print_Jobdata()
{
    cout << "作业ID 到达时间 执行时间 优先权" << endl;
    for (int i = 0; que[i].number; i++)
    {
        cout << que[i].number << "        "
             << que[i].reach_time << "       "
             << que[i].need_time << "       "
             << que[i].privilege << endl;
    }
}

void initial_rr()
{
    for (int count = 0;; count++)
    {
        reach(count);
        bool mark = false;
        for (int i = 0; que[i].number; i++)
        {
            if (que[i].isreached)
            {
                que[i].visited = true;
                rr.push_back(que[i]);
                mark = true;
                break;
            }
        }
        if (mark)
            break;
    }
    return;
}

void reach(int time)
{
    for (int i = 0; que[i].number; i++)
    {
        if (que[i].reach_time >= time)
            que[i].isreached = true;
        else
            que[i].isreached = false;
    }
    return;
}

void read_Jobdata()
{
    fstream file;
    file.open("data.txt", ios::in);
    int i = 0;
    while (!file.eof())
    {
        file >> que[i].number;
        backup[i].number = que[i].number;
        file >> que[i].reach_time;
        backup[i].reach_time = que[i].reach_time;
        file >> que[i].need_time;
        backup[i].need_time = que[i].need_time;
        file >> que[i].privilege;
        backup[i].privilege = que[i].privilege;
        backup[i].visited = false;
        backup[i].run_time = 0;
        i++;
    }
    que[i].number = 0;
    backup[i].number = 0;
    file.close();
    return;
}

void initial_jobs()
{
    for (int i = 0; backup[i].number; i++)
        que[i] = backup[i];
    return;
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
    cout << fixed << setprecision(2)
         << "平均等待时间: " << avg_wait / div << endl
         << "平均周转时间: " << avg_tr / div << endl
         << "平均带权周转时间: " << avg_wtr / i << endl;
    return;
}

void early()
{
    int earliest = 100000, pos;
    int flag = 0;
    for (int i = 0; que[i].number; i++)
    {
        if (que[i].isreached && !que[i].visited && que[i].reach_time <= earliest)
        {
            earliest = que[i].reach_time;
            pos = i;
            flag++;
        }
    }
    if (flag--)
    {
        que[pos].visited = true;
        rr.push_back(que[pos]);
    }
    if (flag)
        early();
    return;
}

void FCFS_schedulejob(job jobs[], int count)
{
    int mark = -1, early = 10000000;
    for (int i = 0; jobs[i].number; i++)
    {
        if (jobs[i].reach_time > 0)
        {
            if (jobs[i].reach_time <= early)
            {
                early = jobs[i].reach_time;
                mark = i;
            }
        }
    }
    if (mark == -1)
    {
        calculate(jobs);
        return;
    }
    if (early <= count)
    {
        jobs[mark].wait_time = count - jobs[mark].reach_time;                    //等待时间
        count += jobs[mark].need_time;                                           //执行作业
        jobs[mark].tr_time = count - jobs[mark].reach_time;                      //周转时间
        jobs[mark].wtr_time = (jobs[mark].tr_time * 1.0) / jobs[mark].need_time; //带权周转时间
        jobs[mark].reach_time = 0;
        cout << "执行完的作业是:  " << jobs[mark].number << "号作业"
             << " 等待时间为 " << jobs[mark].wait_time
             << " 周转时间为 " << jobs[mark].tr_time
             << " 带权周转时间为 " << jobs[mark].wtr_time
             << endl;
        FCFS_schedulejob(jobs, count);
        return;
    }
    else
    {
        jobs[mark].wait_time = 0;                                                //等待时间
        count = jobs[mark].reach_time + jobs[mark].need_time;                    //执行作业
        jobs[mark].tr_time = count - jobs[mark].reach_time;                      //周转时间
        jobs[mark].wtr_time = (jobs[mark].tr_time * 1.0) / jobs[mark].need_time; //带权周转时间
        jobs[mark].reach_time = 0;
        cout << "执行完的作业是:  " << jobs[mark].number << "号作业"
             << " 等待时间为 " << jobs[mark].wait_time
             << " 周转时间为 " << jobs[mark].tr_time
             << " 带权周转时间为 " << jobs[mark].wtr_time
             << endl;
        FCFS_schedulejob(jobs, count);
        return;
    }
}

void SJF_schedulejob(job jobs[], int count)
{
    int mark = -1, shortest = 10000000, early = 1000000;
    for (int i = 0; jobs[i].number; i++)
    {
        if (jobs[i].need_time > 0 && jobs[i].reach_time <= count)
        {
            if (jobs[i].need_time <= shortest)
            {
                shortest = jobs[i].need_time;
                early = jobs[i].reach_time;
                mark = i;
            }
        }
    }
    if (mark == -1)
    {
        for (int i = 0; jobs[i].number; i++)
        {
            if (jobs[i].need_time > 0)
            {
                if (jobs[i].reach_time <= early)
                {
                    early = jobs[i].reach_time;
                    mark = i;
                }
            }
        }
    }
    if (mark == -1)
    {
        calculate(jobs);
        return;
    }
    if (early <= count)
    {
        jobs[mark].wait_time = count - jobs[mark].reach_time;                    //等待时间
        count += jobs[mark].need_time;                                           //执行作业
        jobs[mark].tr_time = count - jobs[mark].reach_time;                      //周转时间
        jobs[mark].wtr_time = (jobs[mark].tr_time * 1.0) / jobs[mark].need_time; //带权周转时间
        jobs[mark].need_time = 0;
        cout << "执行完的作业是:  " << jobs[mark].number << "号作业"
             << " 等待时间为 " << jobs[mark].wait_time
             << " 周转时间为 " << jobs[mark].tr_time
             << " 带权周转时间为 " << jobs[mark].wtr_time
             << endl;
        SJF_schedulejob(jobs, count);
        return;
    }
    else
    {
        jobs[mark].wait_time = 0;                                                //等待时间
        count = jobs[mark].reach_time + jobs[mark].need_time;                    //执行作业
        jobs[mark].tr_time = count - jobs[mark].reach_time;                      //周转时间
        jobs[mark].wtr_time = (jobs[mark].tr_time * 1.0) / jobs[mark].need_time; //带权周转时间
        jobs[mark].need_time = 0;
        cout << "执行完的作业是:  " << jobs[mark].number << "号作业"
             << " 等待时间为 " << jobs[mark].wait_time
             << " 周转时间为 " << jobs[mark].tr_time
             << " 带权周转时间为 " << jobs[mark].wtr_time
             << endl;
        SJF_schedulejob(jobs, count);
        return;
    }
}

void RR_schedulejob()
{
    static const int frame = 10;
    int count = rr.begin()->reach_time;
    list<job>::iterator iter = rr.begin();
    if (iter->need_time >= frame)
    {
        iter->need_time -= frame;
        count += frame;
        iter->run_time += frame;
    }
    else
    {
        count += iter->need_time;
        iter->run_time += iter->need_time;
        iter->need_time = 0;
    }
    while (true)
    {
        reach(count);
        early();
        if (iter->need_time)
        {
            iter++;
        }
        else
        {
            iter->tr_time = count - iter->reach_time;
            iter->wait_time = iter->tr_time - iter->run_time;
            // iter->wtr_time = iter->
            cout << "执行完的作业是:  " << iter->number << "号作业"
                 << " 等待时间为 " << iter->wait_time
                 << " 周转时间为 " << iter->tr_time
                 << " 带权周转时间为 " << iter->wtr_time
                 << endl;
            list<job>::iterator temp = iter;
            iter++;
            rr.erase(temp);
        }
        if (iter == rr.end())
        {
            iter = rr.begin();
        }
        if (iter->need_time >= frame)
        {
            iter->need_time -= frame;
            count += frame;
            iter->run_time += frame;
            continue;
        }
        else
        {
            count += iter->need_time;
            iter->run_time += iter->need_time;
            iter->need_time = 0;
            continue;
        }
    }
}