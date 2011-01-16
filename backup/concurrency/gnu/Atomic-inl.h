FORCE_INLINE int32_t increment(int32_t volatile *addend)
{
  return __sync_add_and_fetch(addend, 1);
}

FORCE_INLINE int32_t decrement(int32_t volatile *addend)
{
  return __sync_add_and_fetch(addend, -1);
}

FORCE_INLINE int32_t compareAndSwap(
  int32_t volatile *destination,
  int32_t exchange,
  int32_t comperand)
{
  return __sync_val_compare_and_swap(
    destination,
    comperand,
    exchange);
}

FORCE_INLINE int64_t compareAndSwap(
  int64_t volatile *destination,
  int64_t exchange,
  int64_t comperand)
{
  return __sync_val_compare_and_swap(
    destination,
    comperand,
    exchange);
}

FORCE_INLINE int32_t swap(
  int32_t volatile *target,
  int32_t value)
{
  return __sync_val_compare_and_swap(
    target,
    *target,
    value);
}

FORCE_INLINE int32_t swapAndAdd(
  int32_t volatile *addend,
  int32_t value)
{
  return __sync_add_and_fetch(addend, value);
}

FORCE_INLINE int32_t add(
  int32_t volatile *addend,
  int32_t value)
{
  return __sync_add_and_fetch(addend, value);
}

FORCE_INLINE int32_t subtract(
  int32_t volatile *addend,
  int32_t value)
{
  return __sync_add_and_fetch(addend, -value);
}
