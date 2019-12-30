/* AUTOGENERATED FILE. DO NOT EDIT. */

/*=======Automagically Detected Files To Include=====*/
#include "unity.h"
#include "circularbuffer/circular_buffer.h"
#include <string.h>

/*=======External Functions This Runner Calls=====*/
extern void setUp(void);
extern void tearDown(void);
extern void test_initialization();
extern void test_single_put_get();
extern void test_string_put_get();
extern void test_multiple_string_put_get();
extern void test_put_get();
extern void test_overflow();
extern void test_underflow();


/*=======Mock Management=====*/
static void CMock_Init(void)
{
}
static void CMock_Verify(void)
{
}
static void CMock_Destroy(void)
{
}

/*=======Test Reset Options=====*/
void resetTest(void);
void resetTest(void)
{
  tearDown();
  CMock_Verify();
  CMock_Destroy();
  CMock_Init();
  setUp();
}
void verifyTest(void);
void verifyTest(void)
{
  CMock_Verify();
}
/*=======Test Runner Used To Run Each Test=====*/
static void run_test(UnityTestFunction func, const char* name, int line_num)
{
    Unity.CurrentTestName = name;
    Unity.CurrentTestLineNumber = line_num;
#ifdef UNITY_USE_COMMAND_LINE_ARGS
    if (!UnityTestMatches())
        return;
#endif
    Unity.NumberOfTests++;
    UNITY_CLR_DETAILS();
    UNITY_EXEC_TIME_START();
    CMock_Init();
    if (TEST_PROTECT())
    {

            setUp();
            func();

    }
    if (TEST_PROTECT())
    {
        tearDown();
        CMock_Verify();
    }
    CMock_Destroy();
    UNITY_EXEC_TIME_STOP();
    UnityConcludeTest();
}


/*=======MAIN=====*/
int main(void)
{
  UnityBegin("circularbuffer/TestCircularBuffer.c");
  run_test(test_initialization, "test_initialization", 43);
  run_test(test_single_put_get, "test_single_put_get", 50);
  run_test(test_string_put_get, "test_string_put_get", 57);
  run_test(test_multiple_string_put_get, "test_multiple_string_put_get", 70);
  run_test(test_put_get, "test_put_get", 86);
  run_test(test_overflow, "test_overflow", 97);
  run_test(test_underflow, "test_underflow", 108);

  return UnityEnd();
}
