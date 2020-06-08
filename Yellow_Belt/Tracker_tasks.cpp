#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <map>
#include <set>
#include <fstream>
#include <iomanip>
#include <exception>
#include <tuple>

using namespace std;

/*

Реализуйте класс TeamTasks, 
позволяющий хранить статистику по статусам задач команды разработчиков:

// Перечислимый тип для статуса задачи
enum class TaskStatus {
  NEW,          // новая
  IN_PROGRESS,  // в разработке
  TESTING,      // на тестировании
  DONE          // завершена
};

// Объявляем тип-синоним для map<TaskStatus, int>,
// позволяющего хранить количество задач каждого статуса
using TasksInfo = map<TaskStatus, int>;

class TeamTasks {
public:
  // Получить статистику по статусам задач конкретного разработчика
  const TasksInfo& GetPersonTasksInfo(const string& person) const;
  
  // Добавить новую задачу (в статусе NEW) для конкретного разработчитка
  void AddNewTask(const string& person);
  
  // Обновить статусы по данному количеству задач конкретного разработчика,
  // подробности см. ниже
  tuple<TasksInfo, TasksInfo> PerformPersonTasks(
      const string& person, int task_count);
};

Метод PerformPersonTasks должен реализовывать следующий алгоритм:

Рассмотрим все не выполненные задачи разработчика person.
Упорядочим их по статусам: сначала все задачи в статусе NEW, 
затем все задачи в статусе IN_PROGRESS и, наконец, задачи в статусе TESTING.
Рассмотрим первые task_count задач и переведём каждую из них в следующий статус 
в соответствии с естественным порядком: NEW → IN_PROGRESS → TESTING → DONE.
Вернём кортеж из двух элементов: информацию о статусах обновившихся задач 
(включая те, которые оказались в статусе DONE) 
и информацию о статусах остальных не выполненных задач.
В случае отсутствия разработчика с именем person метод 
PerformPersonTasks должени вернуть кортеж из двух пустых TasksInfo

Гарантируется, что task_count является положительным числом. 
Если task_count превышает текущее количество невыполненных задач разработчика, 
достаточно считать, что task_count равен количеству невыполненных задач: 
дважды обновлять статус какой-либо задачи в этом случае не нужно.

Кроме того, гарантируется, что метод GetPersonTasksInfo 
не будет вызван для разработчика, не имеющего задач.

Пример работы метода PerformPersonTasks
Предположим, что у конкретного разработчика имеется 10 задач со следующими статусами:

NEW — 3
IN_PROGRESS — 2
TESTING — 4
DONE — 1
Поступает команда PerformPersonTasks с параметром task_count = 4, 
что означает обновление статуса для 3 задач c NEW до IN_PROGRESS 
и для 1 задачи с IN_PROGRESS до TESTING. Таким образом, новые статусы задач будут следующими:

IN_PROGRESS — 3 обновлённых, 1 старая
TESTING — 1 обновлённая, 4 старых
DONE — 1 старая
В этом случае необходимо вернуть кортеж из 2 словарей:

Обновлённые задачи: IN_PROGRESS — 3, TESTING — 1.
Не обновлённые задачи, исключая выполненные: IN_PROGRESS — 1, TESTING — 4.
Ни один из словарей не должен содержать лишних элементов, то есть статусов, 
которым соответствует ноль задач.

*/

// //Перечислимый тип для статуса задачи
// enum class TaskStatus {
//   NEW,          // новая
//   IN_PROGRESS,  // в разработке
//   TESTING,      // на тестировании
//   DONE          // завершена
// };

// // Объявляем тип-синоним для map<TaskStatus, int>,
// // позволяющего хранить количество задач каждого статуса
// using TasksInfo = map<TaskStatus, int>;

void Eraser(TasksInfo & current)
{
	if(current[TaskStatus::NEW] == 0)
	{
		current.erase(TaskStatus::NEW);
	}
	if(current[TaskStatus::IN_PROGRESS] == 0)
	{
		current.erase(TaskStatus::IN_PROGRESS);
	}
	if(current[TaskStatus::TESTING] == 0)
	{
		current.erase(TaskStatus::TESTING);
	}
	if(current[TaskStatus::DONE] == 0)
	{
		current.erase(TaskStatus::DONE);
	}
}

class TeamTasks 
{
public:
	// Получить статистику по статусам задач конкретного разработчика
	const TasksInfo& GetPersonTasksInfo(const string& person) const
	{
		return Persons.at(person);	
	}
  
	// Добавить новую задачу (в статусе NEW) для конкретного разработчитка
	void AddNewTask(const string& person)
	{
		Persons[person][TaskStatus :: NEW]++;
	}
  
	// Обновить статусы по данному количеству задач конкретного разработчика,
	// подробности см. ниже
	tuple<TasksInfo, TasksInfo> PerformPersonTasks(const string& person, int task_count)
	{
		if(Persons.count(person) == 0)
		{
			return {};
		}
		int cur = 0, cur_new = 0, cur_in = 0, cur_test = 0;
		TasksInfo updated_tasks = {};
		TasksInfo untouched_tasks = Persons[person];
		TasksInfo nums = Persons[person];
		for (auto & x : nums)
		{
			cur += x.second;
		}
		cur = min(task_count, cur);
		{
			cur_new = max(nums[TaskStatus::NEW] - cur, 0);		//TO, SKOL'KO OSTANETSYA
			cur = max(cur - nums[TaskStatus::NEW], 0);
		}
		{
			cur_in = max(nums[TaskStatus::IN_PROGRESS] - cur, 0);
			cur = max(cur - nums[TaskStatus::IN_PROGRESS], 0);
		}
		{
			cur_test = max(nums[TaskStatus::TESTING] - cur, 0);
			cur = 0;
		}
		// SDVIG
		{
			updated_tasks[TaskStatus::IN_PROGRESS] = nums[TaskStatus::NEW] - cur_new;
			untouched_tasks[TaskStatus::NEW] = cur_new;
			Persons[person][TaskStatus::NEW] = cur_new;
		}
		{
			updated_tasks[TaskStatus::TESTING] = nums[TaskStatus::IN_PROGRESS] - cur_in;
			untouched_tasks[TaskStatus::IN_PROGRESS] = cur_in;
			Persons[person][TaskStatus::IN_PROGRESS] = cur_in + updated_tasks[TaskStatus::IN_PROGRESS];
		}
		{
			updated_tasks[TaskStatus::DONE] = nums[TaskStatus::TESTING] - cur_test;
			untouched_tasks[TaskStatus::TESTING] = cur_test;
			Persons[person][TaskStatus::TESTING] = cur_test + updated_tasks[TaskStatus::TESTING];
		}
		{
			Persons[person][TaskStatus::DONE] += updated_tasks[TaskStatus::DONE];
			untouched_tasks[TaskStatus::DONE] = 0;
		}
		Eraser(updated_tasks);
		Eraser(untouched_tasks);
		Eraser(Persons[person]);
		return {updated_tasks, untouched_tasks};
	}
private:
map <string, TasksInfo> Persons;
};



// Принимаем словарь по значению, чтобы иметь возможность
// обращаться к отсутствующим ключам с помощью [] и получать 0,
// не меняя при этом исходный словарь
void PrintTasksInfo(TasksInfo tasks_info) {
  cout << tasks_info[TaskStatus::NEW] << " new tasks" <<
      ", " << tasks_info[TaskStatus::IN_PROGRESS] << " tasks in progress" <<
      ", " << tasks_info[TaskStatus::TESTING] << " tasks are being tested" <<
      ", " << tasks_info[TaskStatus::DONE] << " tasks are done" << endl;
}

int main() {
  TeamTasks tasks;
  tasks.AddNewTask("Ilia");
  for (int i = 0; i < 3; ++i) {
    tasks.AddNewTask("Ivan");
  }
  cout << "Ilia's tasks: ";
  PrintTasksInfo(tasks.GetPersonTasksInfo("Ilia"));
  cout << "Ivan's tasks: ";
  PrintTasksInfo(tasks.GetPersonTasksInfo("Ivan"));
  
  TasksInfo updated_tasks, untouched_tasks;
  
  tie(updated_tasks, untouched_tasks) =
      tasks.PerformPersonTasks("Ivan", 2);
  cout << "Updated Ivan's tasks: ";
  PrintTasksInfo(updated_tasks);
  cout << "Untouched Ivan's tasks: ";
  PrintTasksInfo(untouched_tasks);
  
  tie(updated_tasks, untouched_tasks) =
      tasks.PerformPersonTasks("Ivan", 2);
  cout << "Updated Ivan's tasks: ";
  PrintTasksInfo(updated_tasks);
  cout << "Untouched Ivan's tasks: ";
  PrintTasksInfo(untouched_tasks);

  return 0;
}