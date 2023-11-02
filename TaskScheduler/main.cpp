#include <bits/stdc++.h>
using namespace std;
using namespace chrono;

class Task {
    public:
        static int max_task_id;
        int task_id = 0;
        int duration = 0;
        int delay = 0;
        int  status = 0;
        Task() {
            this->task_id = ++max_task_id;
        }
        virtual void run() {
            cout << "Running task " << task_id << " for " << duration << " seconds " << endl;
            this_thread::sleep_for(seconds(duration));
        }
};

int Task::max_task_id = 0;


class OneTimeTask: public Task {
    public:
        OneTimeTask(int delay, int duration) {
            this->delay = delay;
            this->duration = duration;
        }

        void run() {
            cout << "Runing one time task ";
            Task::run();
        }
};

class RecurringTask: public Task {
    public:
        RecurringTask(int delay, int duration, int recurring_delay) {
            this->delay = delay;
            this->duration = duration;
            this->recurring_delay = recurring_delay;
        }

        void run() {
            cout << "Running Delay Task ";
            Task::run();
            this->status = -1;
        }
    private:
        int recurring_delay = 0;
};


typedef pair<system_clock::time_point, Task*> pr_task;

class SchedulerService {
    public:
        SchedulerService() {};

        void addTask(Task* task) {
            auto now = system_clock::now();
            auto new_time = now + seconds(task->delay);
            pq.push({new_time, task});
            task_map[task->task_id] = task;
            cout << "Task Added: task_id: " <<  task->task_id << " time_to_run " << " duration: " << task->duration << endl;
        }

        void removeTask(int task_id) {
            if (task_map.find(task_id) != task_map.end()) {
                task_map.erase(task_id);
            }
        }

        map<int, Task*> getTaskList() {
            return task_map;
        }

        void run() {
            while (true) {
                auto now = system_clock::now();
                cout << "Current Time: " <<  system_clock::to_time_t(now) << endl;
                auto pr = pq.top();

                auto tm = pr.first;
                auto task = pr.second;
                if (task_map.find(task->task_id) == task_map.end()) {
                    cout << "Skipping removed task " << task->task_id << endl;
                    continue;
                }
                if (tm <= now) {
                    pq.pop();
                    task->run();
                    if (task->status == -1) {
                        removeTask(task->task_id);
                        addTask(task);
                    }
                } else {
                    cout << " waiting for " << duration_cast<milliseconds>(tm - now).count() << " milliseconds" << endl;
                    this_thread::sleep_for(milliseconds(duration_cast<milliseconds>(tm - now).count() + 10));
                }
            }
        }
    private:
        priority_queue<pr_task, vector<pr_task>, greater<pr_task>> pq;
        map<int, Task*> task_map;
};


class SchedulerDemo {
    public:
        void run() {
            Task* task1 = new OneTimeTask(10, 1);
            Task* task2 = new OneTimeTask(13, 1);
            Task* task3 = new RecurringTask(15, 1, 3);
            Task* task4 = new RecurringTask(5, 1, 5);
            SchedulerService* service = new SchedulerService();
            service->addTask(task1);
            service->addTask(task2);
            service->addTask(task3);
            service->addTask(task4);
            service->run();
        }
};

int main() {
    SchedulerDemo* sdemo = new SchedulerDemo();
    sdemo->run();
}