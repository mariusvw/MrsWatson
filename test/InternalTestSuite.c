#include <stdlib.h>
#include "LinkedList.h"
#include "TestRunner.h"

extern TestSuite addAudioClockTests(void);
extern TestSuite addAudioSettingsTests(void);
extern TestSuite addCharStringTests(void);
extern TestSuite addLinkedListTests(void);
extern TestSuite addMidiSequenceTests(void);
extern TestSuite addStringUtilitiesTests(void);
extern TestSuite addTaskTimerTests(void);


static void _sumTestSuiteResults(void* item, void* extraData) {
  TestSuite testSuite = (TestSuite)item;
  TestSuite result = (TestSuite)extraData;
  result->numSuccess += testSuite->numSuccess;
  result->numFail += testSuite->numFail;
}

static LinkedList _getTestSuites(void) {
  LinkedList internalTestSuites = newLinkedList();
  appendItemToList(internalTestSuites, addAudioClockTests());
  appendItemToList(internalTestSuites, addAudioSettingsTests());
  appendItemToList(internalTestSuites, addCharStringTests());
  appendItemToList(internalTestSuites, addLinkedListTests());
  appendItemToList(internalTestSuites, addMidiSequenceTests());
  appendItemToList(internalTestSuites, addStringUtilitiesTests());
  appendItemToList(internalTestSuites, addTaskTimerTests());
  return internalTestSuites;
}

void runInternalTestSuite(void);
void runInternalTestSuite(void) {
  TestSuite suiteResults;
  LinkedList internalTestSuites = _getTestSuites();

  foreachItemInList(internalTestSuites, runTestSuite, NULL);
  // Create a new test suite to be used as the userData passed to the foreach loop
  suiteResults = newTestSuite("Suite results", NULL, NULL);
  foreachItemInList(internalTestSuites, _sumTestSuiteResults, suiteResults);

  printf("\nRan %d function tests: %d passed, %d failed\n",
    suiteResults->numSuccess + suiteResults->numFail, suiteResults->numSuccess, suiteResults->numFail);
}

TestCase findTestCase(TestSuite testSuite, char* testName);
TestCase findTestCase(TestSuite testSuite, char* testName) {
  LinkedList iterator = testSuite->testCases;
  TestCase currentTestCase = NULL;

  while(iterator != NULL) {
    if(iterator->item != NULL) {
      currentTestCase = (TestCase)iterator->item;
      if(!strncasecmp(testName, currentTestCase->name, true)) {
        return currentTestCase;
      }
    }
    iterator = iterator->nextItem;
  }

  return NULL;
}

TestSuite findTestSuite(char *testSuiteName);
TestSuite findTestSuite(char *testSuiteName) {
  LinkedList internalTestSuites = _getTestSuites();
  LinkedList iterator = internalTestSuites;
  TestSuite testSuite = NULL;

  while(iterator != NULL) {
    if(iterator->item != NULL) {
      testSuite = (TestSuite)iterator->item;
      if(!strncasecmp(testSuite->name, testSuiteName, STRING_LENGTH_DEFAULT)) {
        return testSuite;
      }
    }
    iterator = iterator->nextItem;
  }

  return NULL;
}

static void _printTestCases(void* item, void* userData) {
  TestCase testCase = (TestCase)item;
  char* testSuiteName = (char*)userData;
  printf("%s:%s\n", testSuiteName, testCase->name);
}

static void _printTestsInSuite(void* item, void* userData) {
  TestSuite testSuite = (TestSuite)item;
  foreachItemInList(testSuite->testCases, _printTestCases, testSuite->name);
}

void printInternalTests(void);
void printInternalTests(void) {
  LinkedList internalTestSuites = _getTestSuites();
  foreachItemInList(internalTestSuites, _printTestsInSuite, NULL);
}
