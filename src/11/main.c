#include <stdio.h>
#include <time.h>
#define PY_SSIZE_T_CLEAN
#include "Python.h"
#include <pthread.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>

// any independent python code
const char *BASE = "from func import get_sum\n"
                   "get_sum(%d, %d)\n";

const int N = 100;
const int N_TH = 5;
PyThreadState *globalthread;

void example(int a, int b) {

  PyThreadState *ts = PyThreadState_New(globalthread->interp);

  int gilstate = PyGILState_Check();
  if (!gilstate) {
    PyEval_RestoreThread(ts);
  } else {

    PyThreadState_Swap(ts);
  }

  char *result;
  asprintf(&result, BASE, a, b);
  PyRun_SimpleString(result);

  PyThreadState_Clear(ts);
  PyThreadState_DeleteCurrent();
}

void *threadsum(void *vargp) {
  pid_t myid = syscall(__NR_gettid);

  printf("ThreadId: %d, %s", myid, "Call python.\n");

  double time_taken = 0.0;

  for (int i = 0; i < N; i++) {
    clock_t t;
    t = clock();

    example(0, 10000);
    t = clock() - t;
    time_taken += ((double)t) / CLOCKS_PER_SEC / N;
  }

  printf("ThreadId: %d, python sum %f seconds \n", myid, time_taken);
  return NULL;
}

int main(int argc, char **argv) {
  pthread_t tid;
  Py_Initialize();
  globalthread = PyThreadState_Get();
  PyEval_SaveThread();

  for (int i = 0; i < N_TH; i++) {
    pthread_create(&tid, NULL, threadsum, (void *)&tid);
  }
  pthread_exit(NULL);

  PyEval_RestoreThread(globalthread);
  if (Py_FinalizeEx() < 0) {
    exit(120);
  }
  return 0;
}
