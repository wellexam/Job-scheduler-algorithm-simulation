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

void read_Jobdata();                            //读取数据文件
void print_Jobdata();                           //打印数据
void initial_jobs();                            //初始化所有作业信息
void calculate(job jobs[]);                     //计算平均时间
void initial_rr();                              //初始化rr序列
void reach(int time);                           //判断到达与否
void early();                                   //查找最早到达的作业
void early(list<job>::iterator &it, int count); //将新到达的作业插入队列
void SJF_schedulejob(job jobs[], int count);    //短作业优先算法
void FCFS_schedulejob(job jobs[], int count);   //先来先服务算法
void RR_schedulejob();                          //时间片轮转算法

static job que[100], backup[100];
static list<job> rr;

int main()
{
    read_Jobdata();
    initial_jobs();
    print_Jobdata();

    cout << "先来先服务：" << endl;
    FCFS_schedulejob(que, 0);

    initial_jobs();

    cout << "短作业优先服务： " << endl;
    SJF_schedulejob(que, 0);

    initial_jobs();

    cout << "时间片轮转: " << endl;
    RR_schedulejob();
    getchar();
    return 0;
}

void print_Jobdata()//打印数据
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

void initial_rr()//初始化rr序列
{
    for (int count = 0;; count++)
    {
        reach(count);
        early();
        if (!rr.empty())
            return;
    }
}

void reach(int time)//判断到达与否
{
    for (int i = 0; que[i].number; i++)
    {
        if (que[i].reach_time <= time)
            que[i].isreached = true;
        else
            que[i].isreached = false;
    }
    return;
}

void read_Jobdata()//读取数据文件
{
    fstream file;
    file.open("data.txt", ios::in);
    int i = 0;
    while (!file.eof())
    {
        file >> backup[i].number;
        file >> backup[i].reach_time;
        file >> backup[i].need_time;
        file >> backup[i].privilege;
        backup[i].visited = false;
        backup[i].run_time = 0;
        i++;
    }
    backup[i].number = 0;
    file.close();
    return;
}

void initial_jobs()//初始化所有作业信息
{
    for (int i = 0; backup[i].number; i++)
        que[i] = backup[i];
    return;
}

void calculate(job jobs[])//计算平均时间
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
         << "平均带权周转时间: " << avg_wtr / div << endl;
    return;
}

void early()//查找最早到达的作业
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
    if (flag)
    {
        que[pos].visited = true;
        rr.push_back(que[pos]);
        flag--;
    }
    if (flag)
        early();
    return;
}

void early(list<job>::iterator &it, int count)//将新到达的作业插入队列
{
    int earliest = 100000, pos;
    int flag = 0;
    for (int i = 0; que[i].number; i++)
    {
        if (que[i].isreached && !que[i].visited && que[i].reach_time <= count)
        {
            if (que[i].reach_time <= earliest)
            {
                earliest = que[i].reach_time;
                pos = i;
            }
            flag++;
        }
    }
    if (flag)
    {
        que[pos].visited = true;
        rr.insert(it, que[pos]);
        flag--;
    }
    if (flag)
    {
        early(it, count);
    }
    return;
}

void FCFS_schedulejob(job jobs[], int count)//先来先服务算法
{
    int mark = -1, early = 10000000;
    for (int i = 0; jobs[i].number; i++)//找出当前已到达的作业中最早的作业
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
    if (mark == -1)//如果已没有为执行的作业，计算平均时间并返回
    {
        calculate(jobs);
        return;
    }
    if (early <= count)//当前作业先于中断时刻到达
    {
        jobs[mark].wait_time = count - jobs[mark].reach_time;                    //等待时间
        count += jobs[mark].need_time;                                           //执行作业
        jobs[mark].tr_time = count - jobs[mark].reach_time;                      //周转时间
        jobs[mark].wtr_time = (jobs[mark].tr_time * 1.0) / jobs[mark].need_time; //带权周转时间
        jobs[mark].reach_time = 0;
        cout << fixed << setprecision(2)
             << "执行完的作业是:  " << jobs[mark].number << "号作业"
             << " 等待时间为 " << jobs[mark].wait_time
             << " 周转时间为 " << jobs[mark].tr_time
             << " 带权周转时间为 " << jobs[mark].wtr_time
             << endl;
        FCFS_schedulejob(jobs, count);//递归查找
        return;
    }
    else//当前作业晚于中断时刻到达
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
        FCFS_schedulejob(jobs, count);//递归查找
        return;
    }
}

void SJF_schedulejob(job jobs[], int count)//短作业优先算法
{
    int mark = -1, shortest = 10000000, early = 1000000;
    for (int i = 0; jobs[i].number; i++)//查找当前到达作业中耗时最短的
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
    if (mark == -1)//若当前时刻没有到达的作业，则查找下一个最早到达的作业
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
    if (mark == -1)//所有作业已执行完毕，计算平均时间并返回
    {
        calculate(jobs);
        return;
    }
    if (early <= count)//当前作业先于中断时刻到达
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
        SJF_schedulejob(jobs, count);//递归查找
        return;
    }
    else//当前作业晚于中断时刻到达
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
        SJF_schedulejob(jobs, count);//递归查找
        return;
    }
}

void RR_schedulejob()//时间片轮转算法
{
    initial_rr();//初始化rr序列
    static const int frame = 10;//设置时间片
    int count = rr.begin()->reach_time;//时间设为rr序列首个作业的到达时间
    list<job>::iterator iter = rr.begin();//迭代器指向rr序列队首
    //执行首个作业
    if (iter->need_time > frame)
    {
        iter->need_time -= frame;
        count += frame;
        iter->run_time += frame;
        cout << iter->number << "号作业执行了" << frame << endl;
    }
    else
    {
        count += iter->need_time;
        iter->run_time += iter->need_time;
        cout << iter->number << "号作业执行了" << iter->need_time << endl;
        iter->need_time = 0;
    }
    //开始轮转调度
    while (true)
    {
        //防止死循环
        if (count >= 2000)
        {
            break;
        }
        reach(count);       //查找当前到达的作业
        early(iter, count); //插入到达作业
        //判断上一作业是否执行完毕
        if (iter != rr.end() && iter->need_time)
        {
            iter++;
        }
        else if (iter != rr.end() && !iter->need_time)
        {
            iter->tr_time = count - iter->reach_time;
            iter->wait_time = iter->tr_time - iter->run_time;
            iter->wtr_time = iter->tr_time / (iter->run_time * 1.0);
            cout << "执行完的作业是:  " << iter->number << "号作业"
                 << " 等待时间为 " << iter->wait_time
                 << " 周转时间为 " << iter->tr_time
                 << " 带权周转时间为 " << iter->wtr_time
                 << endl;
            int pos = iter->number;
            que[pos].tr_time = iter->tr_time;
            que[pos].wait_time = iter->wait_time;
            que[pos].wtr_time = iter->wtr_time;
            list<job>::iterator temp = iter;
            iter++;
            rr.erase(temp);
        }
        //如果指向队尾，移动迭代器到队首
        if (iter == rr.end())
        {
            iter = rr.begin();
        }
        //判断是否空转
        if (!rr.size())
        {
            count++;
            continue;
        }
        if (!iter->need_time)
        {
            if (rr.size() == 1)
            {
                count++;
                continue;
            }
        }
        //执行本轮作业
        if (iter->need_time >= frame)
        {
            iter->need_time -= frame;
            count += frame;
            iter->run_time += frame;
            cout << iter->number << "号作业执行了" << frame << endl;
            // cout << "need_time=" << iter->need_time << " run_time=" << iter->run_time;
            // cout << " count: " << count << endl;
            continue;
        }
        else
        {
            count += iter->need_time;
            iter->run_time += iter->need_time;
            cout << iter->number << "号作业执行了" << iter->need_time << endl;
            iter->need_time = 0;
            // cout << "count: " << count << endl;
            continue;
        }
    }
    calculate(que);
    return;
}
