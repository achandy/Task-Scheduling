//Allan Chandy & Travis Wolf



#include "stdafx.h"
#include <stdlib.h>
#include <list>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <assert.h>
using namespace std;

int mainMem;
int numDevices;
int availMainMem;
int availDevices;

class Job;
list <Job*> readyQueue;
list <Job*> holdQueue1;
list <Job*> holdQueue2;
list <Job*> deviceWaitQueue;
list <Job*> jobs;

int numJob, requiredMem, maxDevices, runtime, priority, start, numDevices, devicesRequested, computingTime, turnaroundTime;
string state;

class Job {
public:
	Job(int, int, int, int, int, int);

	//Come back
	bool operator == (const Job & n) const {
		return (numJob == n.numJob);
	}

	bool operator < (const Job & n) const {
		return (runtime);
	}
};

Job::Job(int numJob, int requiredMem, int maxDevices, int runtime, int priority, int start) {

	this->numJob = numJob;
	this->requiredMem = requiredMem;
	this->maxDevices = maxDevices;
	this->runtime = runtime;
	this->priority = priority;
	this->start = start;
	computingTime = 0;
	numDevices = 0;
	devicesRequested;
	state = "not specified";
	turnaroundTime = -1;

}

void updateholdQueue1() {
	holdQueue1.sort();
}

int handleJobArrival(Job* job) {
	if (job->requiredMem > mainMem || job->maxDevices > numDevices) {
		job->state = "job rejected";
		return 0;
	}
	else if (job->requiredMem > mainMem) {
		if (job->priority == 1) {
			holdQueue1.push_back(job);
			job->state = "in Hold Queue";
			updateholdQueue1();
		}
		else {
			holdQueue2.push_back(job);
			job->state = "in Hold Queue";
		}
		return 1;
	}
	else
	{
		availMainMem = availMainMem - job->memRequired;
		readyQueue.push_back(job);
		job->state = "running on the CPU";
	}
	return 1;
}

void termJob(int currentTime) {
	availMainMem += readyQueue.front()->memRequired; // .push?

	string strState = "Finished at time ";
	strState.append(toString(currentTime));
	readyQueue.front()->turnaroundTime = currentTime - readyQueue.front()->startTime;
	readyQueue.front()->state = strState;

	if (!holdQueue1.empty()) {
		while (holdQueue1.front()->memRequired <= availMainMem) {
			availMainMem -= holdQueue1.front()->memRequired;
			Job* tempt = holdQueue1.front();
			holdQueue1.popFront(); //pop_front?
			readyQueue.push_back(tempt); //push_back?
			tempt->state = "running on the CPU";
		}
	}
	if (!holdQueue2.empty()) {
		while (holdQueue2.front()->memRequired <= availMainMem) {
			availMainMem -= holdQueue2.front()->memRequired;
			Job* tempt = holdQueue2.front();
			holdQueue2.popFront(); //pop_front?
			readyQueue.push_back(tempt); //push_back?
			tempt->state = "running on the CPU";
		}
	}

	if (readyQueue.front()->numDevices > 0) {
		availDevices += readyQueue.front()->devicesHeld;
		readyQueue.front()->devicesHeld = 0;
		if (!deviceWaitQueue.empty()) {
			for (int a = 0; a < deviceWaitQueue.begin();
				list<Job*>::iterator i = deviceWaitQueue.begin();
				advance(i, a);
				Job* tmpJob = *i;
				if (tmpJob->devicesRequested <= availDevices) {
					availDevices -= tmpJob->devicesRequested;
						deviceWaitQueue.remove(tmpJob);
						readyQueue.push_back(tmpJob);
				}
		}
	}
}
readyQueue.popFront();
}

void printDebug(int currentTime) {
	printf("***************TIME = %d***************\n", currentTime);
	printf("MainMem = %d\tavailDevices = %d\n", availMainMem, availDevices);

	for (int a = 0; a < jobs.size(); a++) {
		list<Job*>::iterator i = jobs.begin();
		advance(i, a);
		Job* tempJob = *i;
		printf("%d\t%s\t\t", tempJob->jobNumber, tempJob->state.c_str()); //NO CLUE??
		printf("\n");
	}

	printf("*********READY QUEUE********** ");
	for (int a = 0; a < readyQueue.size(); a++) {
		list<Job*>::iterator i = ready_queue.begin();
		advance(i, a);
		Job* tempJob = *i;
		printf("%d ", tempJob->jobNumber);
	}
	printf("\n");

	printf("*********HOLD QUEUE 1********** ");
	for (int a = 0; a < holdQueue1.size(); a++) {
		list<Job*>::iterator i = holdQueue1.begin();
		advance(i, a);
		Job* tempJob = *i;
		printf("%d ", tempJob->jobNumber);
	}
	printf("\n");

	printf("*********HOLD QUEUE 2********** ");
	for (int a = 0; a < holdQueue2.size(); a++) {
		list<Job*>::iterator i = holdQueue2.begin();
		advance(i, a);
		Job* tempJob = *i;
		printf("%d ", tempJob->jobNumber);
	}
	printf("\n");

	printf("*********DEVICE QUEUE********** ");
	for (int a = 0; a < deviceWaitQueue.size(); a++) {
		list<Job*>::iterator i = deviceWaitQueue.begin();
		advance(i, a);
		Job* tempJob = *i;
		printf("%d ", tempJob->jobNumber);
	}
	printf("\n");

}

int main(int argc char *argv[]) {
	char* inputFile = argv[1];
	char command;
	int t;
	char c0[5], c1[5], c2[5], c3[5], c4[5];
	int pt_in_time = 0;
	int switchJob = 0;
	int Q = -1;
	int J;
	int P;
	int M;
	int R;
	int requested;
	int released;
	int maxDemand;
	char* r = (char*)malloc(LENGTH * sizeof(char));
	char line[LENGTH];
	int currentTime = -1;
	int atTime;
	int remainingTime;
	int weightedTurnaroundTime = 0;
	int turnarooundCount = 0;
	int systemTurnaroundTime = 0;
	FILE *infile = fopen(inputFile, "r");
	assert(inputFile);

	r = fgets(line, LENGTH, inputFile);
	while (r != NULL) {
		currentTime++;
		switchJob++;

		if (switchJob == Q) {
			switchJob = 0;
			if (!ready_queue.empty()) {
				Job *jp = ready_queue.front();
				ready_queue.pop_front();
				ready_queue.push_back(jp);
			}
		}

		if (!ready_queue.empty()) {
			ready_queue.front()->time_computing++;
			if (ready_queue.front()->r == ready_queue.front()->time_computing) {
				terimatejob(current_time);
			}
		}

		sscanf(line, "%c %d ", &command, &t);
		if (command == 'C') {
			if (currentTime > t) {
				r = fgets(line, LENGTH, inputFile);
			}
			else if (currentTime == t) {
				sscanf(line, "%c %d %s %s %s\n", &command, &t, c0, c1, c2, c3, c4);
				strcpy(c0, &c0[2]);
				TOTAL_MAIN_MEMORY = strtou1(c1, NULL, 10);
				available_main_memory = strtou1(c0, NULL, 10);
				strcpy(c1, &c1[2]);
				TOTAL_NUM_DEVICES = strtou1(c1, NULL, 10);
				available_devices = strtou1(c1, NULL, 10);
				strcpy(c3, &c3[2]);
				Q = strtou1(c2, NULL, 10);
				
				r = fgets(line, LENGTH, inputFile);
			}
			else if (command == 'A') {
				if (currentTime > t{
					r = fgets(line, LENGTH, inputFile);
				}
				else if (currentTime == t) {
					sscanf(line, "%c %d %s %s %s %s %s\n", &command, &t, c0, c1, c2, c3, c4);
					strcpy(c0, &c0[2]);
					switchJob = strtou(c0, NULL, 10);
					strcpy(c1, &c1[2]);
					M = strtou1(c1, NULL, 10);
					strcpy(c2 &c2[2]);
					A = strtou1(c2, NULL, 10);
					strcpy(c3, %c3[2]);
					R = strtou1(c3, NULL, 10);
					strcpy(c4, &c4[2]);
					P = strtou1(c4, NULL, 10);
					JOB* jobpt = new Job(switchJob, M, maxDemand, R, P, t);
					handle_job_arrival(jobpt);
					jobs.push_back(jobpt);

					r = fgets(line, LENGTH, inputFile);
				}
				else if (command == 'Q){
					if (current_time > t) {
						r = fgets(line, LENGTH, inputFile);
					}
					else if (currentTime == t) {
						sscanf(line, "%c %d %s %s\n", &command, &t, c0, c1);
						atTime = t;
						strcpy(c0, &c0[2]);
						switchJob(c0, NULL, 10);
						strcpy(c1, &c1[2]);
						requested = strtou1(c1, NULL, 10);
						if (switchJob = ready_queue.front()->jobnumber)
							Job* tmppt = ready_queue.front();
						if (requested <= available_deveices) {
							ready_queue.pop_front();
							ready_queue.push_back(tmppt);
							tmppt->devicesHeld = requested;
							available_devices -= requested;
						}
						else {
							ready_queue.pop_front(0);
							device_wait_queue.push_back(tmppt);
							tmppt->devicesRequested = devicesRequested;
						}
					}

					r = fgets(line, LENGTH, inputFile);
			}
			else if (command == 'L'){
				if (currentTime > t) {
					r = fgets(line, LENGTH, inputFile);
				}
				else if (currentTIme == t){
					sscanf(line, "%c %d %s %s\n", &command, &t, c0, c1);
					atTime = t;
					stsrcpy(c0, &c0[2]);
					switchJob = strtou1(c0, NULL, 10);
					strcpy(c1, &c1[2]);
					released = strtou1(c1, NULL, 10);

					if (switchJob == ready_queue.front()->switchJob) {
						available_devices += ready_queue.front()->devicesHeld;
						ready_queue.front()->devicesHeld = 0;
						if (!device_wait_queue.empty()) {
							for (int a = 0; a < device_wait_queue.size(); a++) {
								printf("INSIDE\n");
								list<Job*>::iterator i = device_wait_queue.begin();
								advance(i, a);
								Job* jp = *i;
								if (jp->devicesRequested <= available_devices) {
									available_devices -= jp->devicesRequested;
									device_wait_queue.remove(jp);
									ready_queue.push_back(jp);
								}
							}
						}
					}

					r = fgets(line, LINELENGTH, infile);
				}
				else if (command == 'D') {
					// A display of the current system status in Readable format (with
					// headings and properly aligned):
					if (current_time > t) {    // will never happen so get a new line
						r = fgets(line, LINELENGTH, infile);
					}
					else if (current_time != t) continue;

					sscanf(line, "%c %d\n", &command, &t);
					atTime = t;

					// The following should be printed:
					// 1. A list of each job that has entered the system; for each job,
					printf("Job#\tJobState\t\tRemainingServiceTime\tTurnaroundTime\n");
					for (int a = 0; a < jobs.size(); a++) {
						// print the state of the job (e.g. running on the CPU, in the
						// Hold Queue, or finished at time _), the remaining service
						// time for unfinished jobs and the turnaround time and weighted
						// turnaround time for finished jobs.
						list<Job*>::iterator i = jobs.begin();
						advance(i, a);
						Job* jp = *i;
						remainingServiceTime = jp->r - jp->time_computing;
						printf("%d\t%s\t\t", jp->jobnumber, jp->state.c_str());
						if (remainingServiceTime > 0) {
							printf("%d", remainingServiceTime);
						}
						printf("\t\t\t");
						if (jp->TaT > -1) {
							printf("%d", jp->TaT);
							weightedTAT += jp->TaT;
							wTATcount++;
						}
						printf("\n");
					}
					// (I am interpreting weighted turnaround time as average turnaround
					// time)
					printf("Weighted Turnaround Time for finished jobs: %f\n", \
						weightedTAT / (double)wTATcount);
					// 2. The contents of each queue.
					printf("The contents of the ready queue: ");
					for (int a = 0; a < ready_queue.size(); a++) {
						list<Job*>::iterator i = ready_queue.begin();
						advance(i, a);
						Job* jp = *i;
						printf("%d ", jp->jobnumber);
					}
					printf("\nThe contents of hold queue 1: ");
					for (int a = 0; a < hold_queue1.size(); a++) {
						list<Job*>::iterator i = hold_queue1.begin();
						advance(i, a);
						Job* jp = *i;
						printf("%d ", jp->jobnumber);
					}
					printf("\nThe contents of hold queue 2: ");
					for (int a = 0; a < hold_queue2.size(); a++) {
						list<Job*>::iterator i = hold_queue2.begin();
						advance(i, a);
						Job* jp = *i;
						printf("%d ", jp->jobnumber);
					}
					printf("\nThe contents of the device wait queue: ");
					for (int a = 0; a < device_wait_queue.size(); a++) {
						list<Job*>::iterator i = device_wait_queue.begin();
						advance(i, a);
						Job* jp = *i;
						printf("%d ", jp->jobnumber);
					}printf("\n");


					if (t == 9999) { // only at the last display,
									 // 3. The system turnaround time and
									 // (I'm interpreting system turnaround time as total TaT)
						printf("The system turnaround time: %d\n", \
							weightedTAT);
						// system weighted turnaround.
						// (I'm interpreting system weighted turnaround as average TaT)
						printf("The system turnaround time: %f\n", \
							weightedTAT / (double)wTATcount);
					}

					r = fgets(line, LINELENGTH, infile);
				}
			}
			fclose(infile);
			free(r);
		}
		}
	}
}