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

void example(PyThreadState *i_ts) { PyRun_SimpleString(BASE); }

void *threadsum(void *vargp) {
  pid_t myid = syscall(__NR_gettid);

  printf("ThreadId: %d, %s", myid, "Call python.\n");

  const PyInterpreterConfig config =
      (PyInterpreterConfig)_PyInterpreterConfig_INIT;
  PyThreadState *ts = NULL;
  PyStatus status = Py_NewInterpreterFromConfig(&ts, &config);
  PyThreadState_Swap(globalthread);
  if (PyStatus_Exception(status)) {
    _PyErr_SetFromPyStatus(status);
    PyObject *exc = PyErr_GetRaisedException();
    PyErr_SetString(PyExc_RuntimeError, "interpreter creation failed");
    _PyErr_ChainExceptions1(exc);
    return NULL;
  }
  assert(ts != NULL);
  PyThreadState_Swap(ts);

  example(ts);

  printf("ThreadId: %d end \n", myid);

  int gilstate = PyGILState_Check();
  if (!gilstate) {
    PyEval_RestoreThread(ts);
  } else {
    PyThreadState_Swap(ts);
  }
  Py_EndInterpreter(ts);
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
