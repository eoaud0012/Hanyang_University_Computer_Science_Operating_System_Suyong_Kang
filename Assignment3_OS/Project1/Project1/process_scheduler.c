#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct _process {
	int process_id;
	int burst_time;
	int waiting_time;
	int priority;
}process;

typedef struct _process_set {
	int process_count;
	int total_wait_time;
	int total_burst_time;
	float average_wait_time;
	process** pro_arr;
}process_set;

int* CopyBurstTime(process_set* process_set) {
	int id = 0;
	int* result;

	result = malloc(sizeof(int) * (process_set->process_count + 1));
	if (result == NULL) {
		printf("malloc error\n");
		return NULL;
	}

	for (id = 1; id < process_set->process_count + 1; id++) {
		result[id] = process_set->pro_arr[id]->burst_time;
	}

	return result;
}

void ChopProcessByTimeQuantum(process_set* process_set, int time_quantum) {
	int i = 0;
	int id = 1;
	int id_2 = 1;
	int* burst_time;

	burst_time = CopyBurstTime(process_set);
	
	for (i = 1; i < process_set->total_burst_time + 1; i++) {
		for (id = 1; id < process_set->process_count + 1; id++) {
			if (burst_time[id] < 1) {
				continue;
			}
			else {
				burst_time[id] -= time_quantum;
				for (id_2 = 1; id_2 < process_set->process_count + 1; id_2++) {
					if (id == id_2 || burst_time[id_2] < 1) {
						continue;
					}
					else {
						process_set->pro_arr[id_2]->waiting_time += time_quantum;
					}
				}
			}
		}
	}

	return;
}
void SortById(process_set* process_set) {
	int i = 0;
	int id = 0;
	int id_2 = 0;
	process* tmp = 0;

	for (id = 2; id < process_set->process_count + 1; id++) {
		tmp = process_set->pro_arr[id];
		for (id_2 = id - 1; id_2 > 0 && process_set->pro_arr[id_2]->process_id > tmp->process_id; id_2--) {
			process_set->pro_arr[id_2 + 1] = process_set->pro_arr[id_2];
		}
		process_set->pro_arr[id_2 + 1] = tmp;
	}

	return;
}
void SortByPriority(process_set* process_set) {
	int i = 0;
	int id = 0;
	int id_2 = 0;
	process* tmp = 0;

	for (id = 2; id < process_set->process_count + 1; id++) {
		tmp = process_set->pro_arr[id];
		for (id_2 = id - 1; id_2 > 0 && process_set->pro_arr[id_2]->priority > tmp->priority; id_2--) {
			process_set->pro_arr[id_2 + 1] = process_set->pro_arr[id_2];
		}
		process_set->pro_arr[id_2 + 1] = tmp;
	}

	return;
}

void SortByBurstTime(process_set* process_set) {
	int i = 0;
	int id = 0;
	int id_2 = 0;
	process* tmp = 0;

	for (id = 2; id < process_set->process_count + 1; id++) {
		tmp = process_set->pro_arr[id];
		for (id_2 = id - 1; id_2 > 0 && process_set->pro_arr[id_2]->burst_time > tmp->burst_time; id_2--) {
			process_set->pro_arr[id_2 + 1] = process_set->pro_arr[id_2];
		}
		process_set->pro_arr[id_2 + 1] = tmp;
	}

	return;
}

void GetAllWaitTime(process_set* process_set) {
	int id = 0;
	int total_wait_time = 0;
	int total_burst_time = process_set->total_burst_time;

	// process_set->pro_arr[i] 에서 인덱스(i) 1, 2, 3, 4 순서대로 burst_time이 오름차순 형태로 프로세스가 정렬되어 있음.
	// 따라서 i가 1인 프로세스는 wait 하지 않으므로, i가 2인 프로세스부터 wait time이 발생
	for (id = 2; id < process_set->process_count + 1; id++) {
		process_set->pro_arr[id]->waiting_time = process_set->pro_arr[id - 1]->burst_time +
			process_set->pro_arr[id - 1]->waiting_time;
	}

	return;
}

void GetAverageWaitTime(process_set* process_set) {
	int id = 0;

	for (id = 1; id < process_set->process_count + 1; id++) {
		process_set->total_wait_time += process_set->pro_arr[id]->waiting_time;
	}

	process_set->average_wait_time = (float)(process_set->total_wait_time) / process_set->process_count;

	return;
}

void PrintGanttChart_RoundRobinVersion(process_set* process_set, int time_quantum) {
	int i = 0;
	int j = 0;
	int k = 0;
	int tmp = 0;
	int digit = 0;
	int id = 1;
	int* burst_time;

	burst_time = CopyBurstTime(process_set);
	
	printf("Gantt Chart\n");

	printf(" %d", 0);
	for (i = 1; i <= process_set->total_burst_time / 5; i++) {
		if (5 * i / 10 > 0) {
			tmp = 5 * i / 10;
			for (tmp; tmp > 0;) {
				tmp /= 10;
				digit++;
			}
			for (k = 0; k < digit; k++) {
				printf("  ");
			}
			digit = 0;
		}
		printf("%8d", 5 * i);
	}
	printf("\n");

	printf(" ");
	for (j = 1; j < process_set->total_burst_time + 1; j++) {
		for (id = 1; id < process_set->process_count + 1; id++) {
			if (burst_time[id] < 1) {
				continue;
			}
			else {
				printf("%d ", process_set->pro_arr[id]->process_id);
				burst_time[id] -= time_quantum;
			}
		}
	}
	printf("\n");


	return;
}

void PrintWaitTime(process_set* process_set) {
	int id = 0;

	SortById(process_set);

	for (id = 1; id < process_set->process_count + 1; id++) {
		printf("Process %d waiting time = %d msec\n", id, process_set->pro_arr[id]->waiting_time);
	}
	printf("Average waiting time = %.2f\n", process_set->average_wait_time);

	return;
}

void PrintGanttChart(process_set* process_set) {
	int i = 0;
	int j = 0;
	int k = 0;
	int tmp = 0;
	int digit = 0;
	int id = 1;
	printf("Gantt Chart\n");

	printf(" %d", 0);
	for (i = 1; i <= process_set->total_burst_time / 5; i++) {
		if (5 * i / 10 > 0) {
			tmp = 5 * i / 10;
			for (tmp; tmp > 0;) {
				tmp /= 10;
				digit++;
			}
			for (k = 0; k < digit; k++) {
				printf("  ");
			}
			digit = 0;
		}
		printf("%8d", 5 * i);
	}
	printf("\n");

	printf(" ");
	for (id; id < process_set->process_count + 1; id++) {
		for (j = 0; j < process_set->pro_arr[id]->burst_time; j++) {
				printf("%d ", process_set->pro_arr[id]->process_id);
		}
	}
	printf("\n");

	return;
}

int FCFS(process_set* process_set) {

	GetAllWaitTime(process_set);
	GetAverageWaitTime(process_set);
	PrintGanttChart(process_set);
	PrintWaitTime(process_set);

	return 0;
}

int SJF(process_set* process_set) {

	SortByBurstTime(process_set);
	GetAllWaitTime(process_set);
	GetAverageWaitTime(process_set);
	PrintGanttChart(process_set);
	PrintWaitTime(process_set);

	return 0;
}

int PS(process_set* process_set) {

	SortByPriority(process_set);
	GetAllWaitTime(process_set);
	GetAverageWaitTime(process_set);
	PrintGanttChart(process_set);
	PrintWaitTime(process_set);

	return 0;
}

int RR(process_set* process_set) {
	int time_quantum = 1;
	
	ChopProcessByTimeQuantum(process_set, time_quantum);
	GetAverageWaitTime(process_set);
	PrintGanttChart_RoundRobinVersion(process_set, time_quantum);
	PrintWaitTime(process_set);

	return 0;
}

void Delete(process_set* process_set) {
	int i = 0;
	for (i = 1; i < process_set->process_count; i++) {
		free(process_set->pro_arr[i]);
	}
	free(process_set->pro_arr);
	free(process_set);

	return;
}
process_set* Init(char* file_name) {

	int i = 0;
	int process_count = 0;
	int process_id = 0;
	int burst_time = 0;
	int priority = 0;
	int file_check = 0;
	FILE* fp;
	process_set* pro_set;

	fopen_s(&fp, file_name, "rt");
	if (fp == NULL) {
		printf("file open fail\n");
		return NULL;
	}

	fscanf_s(fp, "%d", &process_count);

	pro_set = (process_set*)malloc(sizeof(process_set));
	if (pro_set == NULL) {
		printf("malloc error\n");
		return NULL;
	}
	pro_set->pro_arr = (process**)malloc(sizeof(process*) * (process_count + 1));
	if (pro_set->pro_arr == NULL) {
		printf("malloc error\n");
		return NULL;
	}
	for (i = 1; i < process_count + 1; i++) {
		pro_set->pro_arr[i] = (process*)malloc(sizeof(process));
		if(pro_set->pro_arr[i] == NULL) {
			printf("malloc error\n");
			return NULL;
		}
	}

	pro_set->process_count = process_count;
	pro_set->total_wait_time = 0;
	pro_set->total_burst_time = 0;
	pro_set->average_wait_time = 0;

	while (1) {

		fscanf_s(fp, "%d", &process_id);

		if (feof(fp)) break;

		fscanf_s(fp, "%d", &burst_time);
		fscanf_s(fp, "%d", &priority);


		pro_set->pro_arr[process_id]->process_id = process_id;
		pro_set->pro_arr[process_id]->burst_time = burst_time;
		pro_set->pro_arr[process_id]->waiting_time = 0;
		pro_set->pro_arr[process_id]->priority = priority;
		pro_set->total_burst_time += pro_set->pro_arr[process_id]->burst_time;

	}


	file_check = fclose(fp);
	if (file_check != EOF) {
		//printf("file close success\n");
		return pro_set;
	}
	else {
		printf("file close fail\n");
		return NULL;
	}

	return pro_set;
}

int main(int argc, char* argv[]) {

	char* file_name = 0;
	char* simulate_mode = 0;
	process_set* pro_set;

	if (argc == 1) {
		printf("error! please input argument. terminate program\n");
		return -1;

	}

	file_name = argv[1];
	simulate_mode = argv[2];

	pro_set = Init(file_name);
	
	if (!strcmp(simulate_mode, "FCFS")) {
		FCFS(pro_set);
	}
	else if (!strcmp(simulate_mode, "SJF")) {
		SJF(pro_set);
	}
	else if (!strcmp(simulate_mode, "PS")) {
		PS(pro_set);
	}
	else if (!strcmp(simulate_mode, "RR")) {
		RR(pro_set);
	}
	else {
		printf("mode not exist\n");
		return -1;
	}

	Delete(pro_set);

	return 0;
}