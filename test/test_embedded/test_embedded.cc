#include "Arduino.h"
#include <unity.h>
#include <sequence.h>
#include <pieces.h>

#ifdef UNIT_TEST

void test_sequence(void) {
  sequence_t sequence;
  initSequence(&sequence, 42);

  // Sequence is deterministic for this seed value.
  TEST_ASSERT_EQUAL(I, next(&sequence));
  TEST_ASSERT_EQUAL(J, next(&sequence));
  TEST_ASSERT_EQUAL(S, next(&sequence));
  TEST_ASSERT_EQUAL(O, next(&sequence));
  TEST_ASSERT_EQUAL(L, next(&sequence));
  TEST_ASSERT_EQUAL(T, next(&sequence));
  TEST_ASSERT_EQUAL(Z, next(&sequence));

  // After all pieces have been served, they are served again
  // in a different order.
  TEST_ASSERT_EQUAL(I, next(&sequence));
  TEST_ASSERT_EQUAL(T, next(&sequence));
  TEST_ASSERT_EQUAL(S, next(&sequence));
  TEST_ASSERT_EQUAL(O, next(&sequence));
  TEST_ASSERT_EQUAL(L, next(&sequence));
  TEST_ASSERT_EQUAL(J, next(&sequence));
  TEST_ASSERT_EQUAL(Z, next(&sequence));
}

void test_reshuffle(void) {
  sequence_t sequence;
  initSequence(&sequence, 0);

  // Sequence is deterministic for this seed value.
  TEST_ASSERT_EQUAL(Z, next(&sequence));
  TEST_ASSERT_EQUAL(T, next(&sequence));
  TEST_ASSERT_EQUAL(L, next(&sequence));
  TEST_ASSERT_EQUAL(O, next(&sequence));
  TEST_ASSERT_EQUAL(S, next(&sequence));
  TEST_ASSERT_EQUAL(J, next(&sequence));
  TEST_ASSERT_EQUAL(I, next(&sequence));
}

void setup() {
  digitalWrite(13, LOW);

  UNITY_BEGIN();

  RUN_TEST(test_sequence);
  RUN_TEST(test_reshuffle);

  UNITY_END();
}

void loop() {
  digitalWrite(13, HIGH);
}

#endif
