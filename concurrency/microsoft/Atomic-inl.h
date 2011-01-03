extern "C" {

// Make sure that the InterlockedXXX functions are generated as intrinsics:
LONG _InterlockedIncrement(LONG volatile *addend);
LONG _InterlockedDecrement(LONG volatile *addend);
LONG _InterlockedCompareExchange(LPLONG volatile destination, LONG exchange, LONG comperand);
LONGLONG _InterlockedCompareExchange64(LONGLONG volatile *destination, LONGLONG exchange, LONGLONG comperand);
LONG _InterlockedExchange(LPLONG volatile target, LONG value);
LONG _InterlockedExchangeAdd(LPLONG volatile addend, LONG value);

} /// extern "C"

FORCE_INLINE int32_t increment(int32_t volatile *addend)
{
  return (int32_t)_InterlockedIncrement((LPLONG)addend);
}

FORCE_INLINE int32_t decrement(int32_t volatile *addend)
{
  return (int32_t)_InterlockedDecrement((LPLONG)addend);
}

FORCE_INLINE int32_t compareAndSwap(
  int32_t volatile *destination,
  int32_t exchange,
  int32_t comperand)
{
  return (int32_t)_InterlockedCompareExchange(
    (LPLONG)destination,
    (LONG)exchange,
    (LONG)comperand);
}

FORCE_INLINE int64_t compareAndSwap(
  int64_t volatile *destination,
  int64_t exchange,
  int64_t comperand)
{
  return (int64_t)_InterlockedCompareExchange64(
    (LONGLONG*)destination,
    (LONGLONG)exchange,
    (LONGLONG)comperand);
}

FORCE_INLINE int32_t swap(
  int32_t volatile *target,
  int32_t value)
{
  return (int32_t)_InterlockedExchange((LPLONG)target, (LONG)value);
}

FORCE_INLINE int32_t swapAndAdd(
  int32_t volatile *addend,
  int32_t value)
{
  return (int32_t)_InterlockedExchangeAdd((LPLONG)addend, (LONG)value);
}

FORCE_INLINE int32_t add(int32_t volatile *addend, int32_t value)
{
  return (int32_t)_InterlockedExchangeAdd((LPLONG)addend, (LONG)value);
}

FORCE_INLINE int32_t subtract(int32_t volatile *addend, int32_t value)
{
  return (int32_t)_InterlockedExchangeAdd((LPLONG)addend, (LONG)-value);
}
