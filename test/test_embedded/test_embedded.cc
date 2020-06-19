#include "Arduino.h"
#include <sequence.h>
#include <piece.h>
#include <unity.h>

#ifdef UNIT_TEST

void test_random(void) {
  Random random = Random(42);

  // Deterministic for this seed.
  TEST_ASSERT_EQUAL(4, random.choice(0, 10));
}

void test_sequence(void) {
  Sequence sequence = Sequence(42);

  // Sequence is deterministic for this seed value.
  TEST_ASSERT_EQUAL(I, sequence.next());
  TEST_ASSERT_EQUAL(Z, sequence.next());
  TEST_ASSERT_EQUAL(T, sequence.next());
  TEST_ASSERT_EQUAL(O, sequence.next());
  TEST_ASSERT_EQUAL(S, sequence.next());
  TEST_ASSERT_EQUAL(L, sequence.next());
  TEST_ASSERT_EQUAL(J, sequence.next());

  // After all pieces have been served, they are served again
  // in a different order.
  TEST_ASSERT_EQUAL(I, sequence.next());
  TEST_ASSERT_EQUAL(Z, sequence.next());
  TEST_ASSERT_EQUAL(L, sequence.next());
  TEST_ASSERT_EQUAL(S, sequence.next());
  TEST_ASSERT_EQUAL(O, sequence.next());
  TEST_ASSERT_EQUAL(T, sequence.next());
  TEST_ASSERT_EQUAL(J, sequence.next());
}

void setup() {
  digitalWrite(13, LOW);

  UNITY_BEGIN();

  RUN_TEST(test_random);
  RUN_TEST(test_sequence);

  UNITY_END();
}

void loop() {
  digitalWrite(13, HIGH);
}

#endif
