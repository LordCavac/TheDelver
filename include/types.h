#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>

// Rename types for clarity and typing reduction
#define u8 uint8_t
#define u16 uint16_t
#define u32 uint32_t
#define u64 uint64_t
#define i8 int8_t
#define i16 int16_t
#define i32 int32_t
#define i64 int64_t
#define f32 float
#define f64 double

typedef struct Vec2i { i32 x; i32 y; } Vec2i;


#endif
