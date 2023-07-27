#include <stdio.h>
#define PY_SSIZE_T_CLEAN
#include "Python.h"
#include "config.h"
#include <pthread.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>

// any independent python code
const char *BASE = "from func import get_sum\n"
                   "get_sum()\n";

PyThreadState *globalthread;

void example() {
  // create sub interpretator
  PyThreadState *ts = PyThreadState_New(globalthread->interp);

  int gilstate = PyGILState_Check();
  if (!gilstate) {
    PyEval_RestoreThread(ts);
  } else {
    PyThreadState_Swap(ts);
  }

  PyRun_SimpleString(BASE);

  PyThreadState_Clear(ts);
  PyThreadState_DeleteCurrent();
}

void *threadsum(void *vargp) {
  pid_t myid = syscall(__NR_gettid);

  printf("ThreadId: %d, %s", myid, "Call python.\n");
  example();
  printf("ThreadId: %d end \n", myid);
  return NULL;
}

int main(int argc, char **argv) {
  pthread_t tid;
  Py_Initialize();
  globalthread = PyThreadState_Get();
  PyEval_SaveThread();

  for (int i = 0; i < THREADS; i++) {
    pthread_create(&tid, NULL, threadsum, (void *)&tid);
  }
  pthread_exit(NULL);

  PyEval_RestoreThread(globalthread);
  if (Py_FinalizeEx() < 0) {
    exit(120);
  }
  return 0;
}
